#ifndef Hardware_h
#define Hardware_h

#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <Wire.h>

#include "Constants.h"
#include "Definitions.h"

extern Adafruit_NeoPixel pixels;
extern Adafruit_SSD1306 display;
extern Adafruit_USBD_MIDI usb_midi;

// Encoder state variables
extern volatile int encoderValue;
extern volatile int lastEncoded;
extern volatile int stepCounter;

// Serial Interrupt variables
extern volatile uint8_t status;
extern volatile uint8_t data1;
extern volatile uint8_t data2;
extern volatile MIDIState currentState;
extern volatile bool midiMessageReady;

void initHardware(Settings settings);
void updateEncoder();
void midiInterruptHandler();
int readEncoder(int current, int itemCount);
int readEncoderConstrained(int current, int step, int min, int max);
float readEncoderFast(float current, float step, float min, float max);
uint32_t dimColor(uint32_t color, float factor);

#endif