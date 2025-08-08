#ifndef Definitions_h
#define Definitions_h

#include <Arduino.h>

// Global settings
struct Settings
{
  int rootNote = 48;
  int scaleIndex = 0;
  int midiRecChannel = 0;
  int midiTrigChannel = 1;
  int midiOutputAChannel = 0;
  int midiOutputBChannel = 2;
  int midiOutputCChannel = 3;
  int midiOutputDChannel = 15;
  bool midiThru = false;
  bool tempoSync = true;
  float tempo = 120;
  float velocityScaling = 1.0;
  int defaultVelocity = 65;
  uint32_t shiftColor = 0xFF0000;
  int ledBrightness = 100;
};

// Chord struct
struct Chord
{
  int degree = 0;                 // Chord degree
  int intervals[8] = {0};         // Intervals for the chord
  int octaveModifiers[8] = {0};   // Octave modifications for each note
  int semitoneModifiers[8] = {0}; // Semitone modifications for each note
  bool isActive[8] = {false};     // Whether each note is active or not
  int velocityModifiers[8] = {0}; // Velocity values for each note modifying the main pad velocity
  unsigned long timing[8] = {0};  // Delay timing for each note
  int channel[8] = {0};
};

// Pad settings
struct Pad
{
  uint32_t color = 0xFFAA00;
  int triggerNote = 0;
  int chokeGroup = 0;
  bool hold = false;
  int arpType = 0; // 0 = off, 1 = up, 2 = down, 3 = both up & down, 4 = random, 5 = brownian
  int arpSpeed = 1;
  float arpGate = 1.0;
  int arpOctaves = 1;
  int padVelocity = 65;
  int velocityVariation = 0;
  unsigned long timingVariation = 0;
  Chord chord;
};

// MIDI state machine enum for Serial reading
enum MIDIState
{
  WAITING_FOR_STATUS, // Waiting for the first byte (status byte)
  WAITING_FOR_DATA1,  // Waiting for the first data byte (note number)
  WAITING_FOR_DATA2   // Waiting for the second data byte (velocity)
};

#endif
