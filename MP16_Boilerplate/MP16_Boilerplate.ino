#include <Adafruit_TinyUSB.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LittleFS.h>

// Pin Definitions
#define SHIFT_PIN 3
#define NEOPIXEL_PIN 2
#define NUM_PIXELS 17
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C
#define TX_PIN 0
#define RX_PIN 1

#define COL0_PIN 9
#define COL1_PIN 10
#define COL2_PIN 11
#define COL3_PIN 12

#define ROW0_PIN 13
#define ROW1_PIN 14
#define ROW2_PIN 15
#define ROW3_PIN 26

#define ENCODER_S 6
#define ENCODER_A 7
#define ENCODER_B 8


//Pixel & Display objects
Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//USB MIDI object
Adafruit_USBD_MIDI usb_midi;

// Key state arrays
bool shiftState = false;
bool previousShiftState = false;
bool encoderState = false;
bool previousEncoderState = false;
bool keyStates[16] = { false };
bool previousKeyStates[16] = { false };
bool shiftPressedAtKeyDown[16] = { false };
bool padStates[16] = { false };
bool previousPadStates[16] = { false };
bool midiStates[16] = { false };
bool previousMidiStates[16] = { false };

// Variables for encoder handling
volatile int encoderValue = 0;
volatile int lastEncoded = 0;
volatile int stepCounter = 0;
const int encoderStates[16] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

//How much to dim the LED colors
float dimFactor = 0.1;


#include "musicTheory.h"

//Setup function
void setup() {
  initHardware();

  usb_midi.begin();

  pinMode(1, INPUT_PULLUP);
  Serial1.begin(31250);
}

//Loop function
void loop() {
  checkKeys();
  updateMIDI();
  updateVisuals();
}


//********************************** Setup Functions ************************************

//Hardware initialization for keymatrix, shift key, encoder, neopixels and OLED display
void initHardware() {
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

  attachInterrupt(digitalPinToInterrupt(ENCODER_A), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_B), updateEncoder, CHANGE);

  pixels.begin();
  pixels.setBrightness(100);
  pixels.clear();
  pixels.show();

  display.setRotation(2);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

//Interrupt function for the encoder
void updateEncoder() {
  // Read both A and B pin states
  int MSB = digitalRead(ENCODER_A);  // Most Significant Bit
  int LSB = digitalRead(ENCODER_B);  // Least Significant Bit

  // Create a binary representation of the state
  int encoded = (MSB << 1) | LSB;          // Combine A and B signal states
  int sum = (lastEncoded << 2) | encoded;  // Create a unique sum to match the state transitions

  // Update step counter based on encoder state transition
  stepCounter += encoderStates[sum];

  // Check if full step (detent) is reached (4 steps per detent)
  if (abs(stepCounter) >= 4) {
    if (stepCounter > 0) {
      encoderValue++;  // Clockwise rotation
    } else {
      encoderValue--;  // Counter-clockwise rotation
    }
    stepCounter = 0;  // Reset counter after full step
  }

  lastEncoded = encoded;  // Store the current state for next transition
}



// ********************************** Loop Functions ************************************

//Checking all keys to update their input
void checkKeys() {
  //Set all previous states to current states
  previousShiftState = shiftState;
  previousEncoderState = encoderState;
  for (int i = 0; i < 16; i++) {
    previousKeyStates[i] = keyStates[i];
  }

  //Now update all current states
  shiftState = !digitalRead(SHIFT_PIN);
  encoderState = !digitalRead(ENCODER_S);

  //Read through the keymatrix to update key states
  for (int row = 0; row < 4; row++) {
    digitalWrite(ROW0_PIN, row == 0 ? LOW : HIGH);
    digitalWrite(ROW1_PIN, row == 1 ? LOW : HIGH);
    digitalWrite(ROW2_PIN, row == 2 ? LOW : HIGH);
    digitalWrite(ROW3_PIN, row == 3 ? LOW : HIGH);
    for (int col = 0; col < 4; col++) {
      int keyIndex = row * 4 + col;
      bool currentKeyState = !digitalRead(COL0_PIN + col);
      keyStates[keyIndex] = currentKeyState;
    }
  }
  //set last row high again after scanning
  digitalWrite(ROW3_PIN, HIGH);
}



//Main MIDI update function
void updateMIDI() {
  for (int i = 0; i < 16; i++) {
    previousPadStates[i] = padStates[i];
  }

  for (int i = 0; i < 16; i++) {  //Go through all pads
    //Check for rising or falling edges in the keys
    if (keyStates[i] && !previousKeyStates[i]) {  // Key is pressed
      if (shiftState) {                           // Check if we're using a SHIFT shortcut
        shiftPressedAtKeyDown[i] = true;
      } else {  //Change pad state
        shiftPressedAtKeyDown[i] = false;
        padStates[i] = true;
      }
    } else if (!keyStates[i] && previousKeyStates[i]) {  // Key is released
      if (!shiftPressedAtKeyDown[i]) {
        padStates[i] = false;
      }
    }

    //Finally check rising or falling edges in pad playing status
    if (padStates[i] && !previousPadStates[i]) {
      //DO SOMETHING AT KEYDOWN
    } else if (!padStates[i] && previousPadStates[i]) {
      //DO SOMETHING AT KEYUP
    }
  }
}


void sendNoteOn(int note, int velocity, int channel) {
  if (channel < 0 || channel > 15) return;
  uint8_t status = 0x90 | (channel);  // 0x90 is "Note On", and channel is adjusted to 0-based
  uint8_t usb_packet[] = { status, (uint8_t)note, (uint8_t)velocity };
  usb_midi.write(usb_packet, 3);
  Serial1.write(status);
  Serial1.write(note);
  Serial1.write(velocity);
}

void sendNoteOff(int note, int velocity, int channel) {
  if (channel < 0 || channel > 15) return;
  uint8_t status = 0x80 | (channel);
  uint8_t usb_packet[] = { status, (uint8_t)note, (uint8_t)velocity };
  usb_midi.write(usb_packet, 3);
  Serial1.write(status);
  Serial1.write(note);
  Serial1.write(velocity);
}

//Updating the visuals!
void updateVisuals() {
  updatePixels();
  updateDisplay();
}

//main function for updating the neopixels
void updatePixels() {
  pixels.clear();

  if (shiftState) {
    pixels.setPixelColor(0, 0xFFFFFF);  // Shift key lighting
  } else {
    pixels.setPixelColor(0, dimColor(0xFFFFFF, dimFactor));  // Shift key lighting dimly
  }

  for (int i = 0; i < 16; i++) {
    if (keyStates[i]) {
      pixels.setPixelColor(i + 1, 0xFFFFFF);
    } else {
      pixels.setPixelColor(i + 1, dimColor(0xFFFFFF, dimFactor));
    }
  }
  pixels.show();
}

//Helper function for dimming a pixel color into a dimmer color
uint32_t dimColor(uint32_t color, float factor) {
  // Extract the red, green, and blue components
  uint8_t red = (color >> 16) & 0xFF;   // Extract red
  uint8_t green = (color >> 8) & 0xFF;  // Extract green
  uint8_t blue = color & 0xFF;          // Extract blue

  // Scale each component by the factor
  red = static_cast<uint8_t>(red * factor);
  green = static_cast<uint8_t>(green * factor);
  blue = static_cast<uint8_t>(blue * factor);

  // Reassemble the dimmed color
  uint32_t dimmedColor = (red << 16) | (green << 8) | blue;
  return dimmedColor;
}

//Main function for updating the OLED display
void updateDisplay() {
  display.clearDisplay();
  //SHOW SOMETHING ON THE DISPLAY
  display.display();
}