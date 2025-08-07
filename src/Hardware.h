#ifndef Hardware_h
#define Hardware_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_TinyUSB.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

namespace Hardware
{
  extern Adafruit_NeoPixel pixels;
  extern Adafruit_SSD1306 display;
  extern Adafruit_USBD_MIDI usb_midi;

  void initHardware();
  void updateEncoder();
  void midiInterruptHandler();
}

#endif