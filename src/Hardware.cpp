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

  // Serial Interrupt variables
  volatile uint8_t status = 0;
  volatile uint8_t data1 = 0;
  volatile uint8_t data2 = 0;
  volatile MIDIState currentState = WAITING_FOR_STATUS;
  volatile bool midiMessageReady = false;

  void initHardware(Settings settings)
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
    pixels.setBrightness(settings.ledBrightness);
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
    display.setTextColor(WHITE);
    display.display();

    usb_midi.begin();
  }

  // Interrupt function for the encoder
  // http://adam-meyer.com/arduino/Rotary_Encoder
  void updateEncoder()
  {
    // Read both A and B pin states
    int MSB = digitalRead(ENCODER_A); // Most Significant Bit
    int LSB = digitalRead(ENCODER_B); // Least Significant Bit

    // Create a binary representation of the state
    // converting the 2 pin value to single number
    int encoded = (MSB << 1) | LSB;
    // combining previous encoded value with current value
    int combination = (lastEncoded << 2) | encoded;

    // Spinning the encoder clockwise outputs 10, 11, 01, 00 repeatedly
    // When spinning clockwise (for example: 10 -> 11), the encoder value is increased
    if (combination == 0b1011 ||
        combination == 0b1101 ||
        combination == 0b0100 ||
        combination == 0b0010)
    {
      encoderValue++;
    }
    if (combination == 0b1000 ||
        combination == 0b1110 ||
        combination == 0b0111 ||
        combination == 0b0001)
    {
      encoderValue--;
    }

    lastEncoded = encoded; // store this value for next time
  }

  void midiInterruptHandler()
  {
    uint8_t incomingByte = Serial1.read(); // Read incoming byte

    switch (currentState)
    {
    case WAITING_FOR_STATUS:
      if ((incomingByte >= 128 && incomingByte <= 143) ||
          (incomingByte >= 144 && incomingByte <= 159))
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

  int readEncoder(int current, int itemCount)
  {
    if (encoderValue >= ENCODER_DETENT_STEPS)
    {
      encoderValue = 0;
      return (current + 1) % itemCount;
    }
    if (encoderValue <= -ENCODER_DETENT_STEPS)
    {
      encoderValue = 0;
      return (current - 1 + itemCount) % itemCount;
    }
    return current;
  }

  int readEncoderConstrained(int current, int step, int min, int max)
  {
    if (encoderValue >= ENCODER_DETENT_STEPS)
    {
      encoderValue = 0;
      return constrain(current + step, min, max);
    }
    if (encoderValue <= -ENCODER_DETENT_STEPS)
    {
      encoderValue = 0;
      return constrain(current - step, min, max);
    }
    return current;
  }

  float readEncoderFast(float current, float step, float min, float max)
  {
    float nextValue = current;
    if (encoderValue >= ENCODER_DETENT_STEPS) // fast clockwise
    {
      nextValue = constrain(current + step * ENCODER_FAST_FACTOR, min, max);
    }
    else if (encoderValue > 0) // precise and slow clockwise
    {
      nextValue = constrain(current + step, min, max);
    }
    if (encoderValue <= -ENCODER_DETENT_STEPS) // fast counterclockwise
    {
      nextValue = constrain(current - step * ENCODER_FAST_FACTOR, min, max);
    }
    else if (encoderValue < 0) // precise and slow counterclockwise
    {
      nextValue = constrain(current - step, min, max);
    }
    encoderValue = 0;
    return nextValue;
  }
}
