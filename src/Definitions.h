#ifndef Definitions_h
#define Definitions_h

#include <Arduino.h>

// Global settings
struct Settings {
  int rootNote = 48;
  int scaleIndex = 0;
  int midiInChannel = 0;
  int midiOutputAChannel = 0;
  int midiOutputBChannel = 1;
  int midiOutputCChannel = 2;
  int midiOutputDChannel = 3;
  bool midiThru = true;
  float velocityScaling = 1.0;
  uint32_t shiftColor = 0xFF0000;
  int ledBrightness = 100;
  bool mute = false;
  bool sustain = false;
};

// Chord struct
struct Chord {
  int degree = 0;                  // Chord degree
  int intervals[8] = {0};          // Intervals for the chord
  int octaveModifiers[8] = {0};    // Octave modifications for each note
  int semitoneModifiers[8] = {0};  // Semitone modifications for each note
  bool isActive[8] = {false};      // Whether each note is active or not
  int velocityModifiers[8] = {
      0};  // Velocity values for each note modifying the main pad velocity
  unsigned long timing[8] = {0};  // Delay timing for each note
  int channel[8] = {0};           // MIDI channel for each note
};

// Pad settings
struct Pad {
  uint32_t color = 0xFFAA00;
  int triggerNote = 0;
  int arpType = 0;  // 0 = off, 1 = up, 2 = down, 3 = both up & down, 4 =
                    // random, 5 = brownian
  int arpSpeed = 1;
  float arpGate = 1.0;
  int arpOctaves = 1;
  int padVelocity = 65;
  int velocityRandom = 0;
  int timingVariation = 0;
  Chord chord;
};

// MIDI state machine enum for Serial reading
enum MIDIState {
  WAITING_FOR_STATUS,  // Waiting for the first byte (status byte)
  WAITING_FOR_DATA1,   // Waiting for the first data byte (note number)
  WAITING_FOR_DATA2    // Waiting for the second data byte (velocity)
};

// Array to track note counts for each MIDI note
struct ActiveNotes {
  int channelA[128] = {0};
  int channelB[128] = {0};
  int channelC[128] = {0};
  int channelD[128] = {0};
};

#endif
