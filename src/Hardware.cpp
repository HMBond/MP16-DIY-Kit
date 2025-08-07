#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include <Wire.h>
#include "Constants.h"
#include "Definitions.h"
#include "Hardware.h"

namespace Hardware
{
  Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
  Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  Adafruit_USBD_MIDI usb_midi;

  // Encoder state variables
  volatile int encoderValue = 0;
  volatile int lastEncoded = 0;
  volatile int stepCounter = 0;
  const int encoderStates[PADS_COUNT] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};

  // Serial Interrupt variables
  volatile uint8_t status = 0;
  volatile uint8_t data1 = 0;
  volatile uint8_t data2 = 0;
  volatile MIDIState currentState = WAITING_FOR_STATUS; // Start by waiting for the status byte
  volatile bool midiMessageReady = false;

  void initHardware()
  {
    // Configure pin modes
    pinMode(ROW0_PIN, OUTPUT);
    pinMode(ROW1_PIN, OUTPUT);
    pinMode(ROW2_PIN, OUTPUT);
    pinMode(ROW3_PIN, OUTPUT);
    pinMode(COL0_PIN, INPUT_PULLUP);
    pinMode(COL1_PIN, INPUT_PULLUP);
    pinMode(COL2_PIN, INPUT_PULLUP);
    pinMode(COL3_PIN, INPUT_PULLUP);
    pinMode(SHIFT_PIN, INPUT_PULLUP);
    pinMode(ENCODER_S, INPUT_PULLUP);
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);

    // Setup interrupts
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B), updateEncoder, CHANGE);
    attachInterrupt(digitalPinToInterrupt(RX_PIN), midiInterruptHandler, FALLING);

    // Initialize NeoPixels
    pixels.begin();
    pixels.setBrightness(100);
    pixels.clear();
    pixels.show();

    // Initialize Display
    display.setRotation(2);
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
    {
      for (;;)
        ; // Don't proceed if display initialization fails
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.display();
  }

  // Interrupt function for the encoder
  void updateEncoder()
  {
    // Read both A and B pin states
    int MSB = digitalRead(ENCODER_A); // Most Significant Bit
    int LSB = digitalRead(ENCODER_B); // Least Significant Bit

    // Create a binary representation of the state
    int encoded = (MSB << 1) | LSB;         // Combine A and B signal states
    int sum = (lastEncoded << 2) | encoded; // Create a unique sum to match the state transitions

    // Update step counter based on encoder state transition
    stepCounter += encoderStates[sum];

    // Check if full step (detent) is reached (4 steps per detent)
    if (abs(stepCounter) >= 4)
    {
      if (stepCounter > 0)
      {
        encoderValue++; // Clockwise rotation
      }
      else
      {
        encoderValue--; // Counter-clockwise rotation
      }
      stepCounter = 0; // Reset counter after full step
    }

    lastEncoded = encoded; // Store the current state for next transition
  }

  void midiInterruptHandler()
  {
    uint8_t incomingByte = Serial1.read(); // Read incoming byte

    switch (currentState)
    {
    case WAITING_FOR_STATUS:
      if ((incomingByte >= 128 && incomingByte <= 143) || (incomingByte >= 144 && incomingByte <= 159))
      {
        status = incomingByte;
        currentState = WAITING_FOR_DATA1; // Next, expect data1 (note number)
      }
      break;

    case WAITING_FOR_DATA1:
      data1 = incomingByte;             // Store the note number
      currentState = WAITING_FOR_DATA2; // Next, expect data2 (velocity)
      break;

    case WAITING_FOR_DATA2:
      data2 = incomingByte;              // Store the velocity
      midiMessageReady = true;           // Complete message received
      currentState = WAITING_FOR_STATUS; // Reset to waiting for the next status byte
      break;
    }
  }
}
