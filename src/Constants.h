#ifndef Constants_h
#define Constants_h

#include "Definitions.h"

// Pin Definitions
constexpr int SHIFT_PIN = 3;
constexpr int NEOPIXEL_PIN = 2;
constexpr int NUM_PIXELS = 17;
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr int OLED_RESET = -1;
constexpr int OLED_ADDR = 0x3C;
constexpr int TX_PIN = 0;
constexpr int RX_PIN = 1;
constexpr int COL0_PIN = 9;
constexpr int COL1_PIN = 10;
constexpr int COL2_PIN = 11;
constexpr int COL3_PIN = 12;
constexpr int ROW0_PIN = 13;
constexpr int ROW1_PIN = 14;
constexpr int ROW2_PIN = 15;
constexpr int ROW3_PIN = 26;
constexpr int ENCODER_S = 6;
constexpr int ENCODER_A = 7;
constexpr int ENCODER_B = 8;

// Encoder
constexpr int ENCODER_DETENT_STEPS = 4;
constexpr int ENCODER_FAST_FACTOR = 4;

// MIDI Constants
constexpr int MIDI_CHANNELS = 16;
constexpr int NOTE_COUNT = 128;
constexpr int VELOCITY_MAX = 99;

// Matrix Constants
constexpr int ROW_COUNT = 4;
constexpr int COL_COUNT = 4;
constexpr int PADS_COUNT = ROW_COUNT * COL_COUNT;

// Preset Constants
constexpr int SLOT_COUNT = 4;

// Define an array of chords as preset
const Chord initialPreset[16] = {{// Chord 1
                            .degree = 0,
                            .intervals = {0, 4, 7, 11, 14, 17, 21, 0},
                            .octaveModifiers = {0, 1, 0, 0, 0, 0, 0, -1},
                            .semitoneModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .isActive = {1, 1, 1, 1, 0, 0, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 2
                            .degree = 1,
                            .intervals = {2, 5, 9, 12, 16, 19, 23, 2},
                            .octaveModifiers = {0, 1, 0, 0, 0, 0, 0, -1},
                            .semitoneModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .isActive = {1, 1, 1, 1, 0, 0, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 3
                            .degree = 2,
                            .intervals = {4, 7, 11, 14, 17, 21, 24, 4},
                            .octaveModifiers = {0, 1, 0, 0, 0, 0, 0, -1},
                            .semitoneModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .isActive = {1, 1, 1, 1, 0, 0, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 4
                            .degree = 3,
                            .intervals = {5, 9, 12, 16, 19, 23, 26, 5},
                            .octaveModifiers = {0, 1, 0, 0, 0, 0, 0, -1},
                            .semitoneModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .isActive = {1, 1, 1, 1, 0, 0, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 5
                            .degree = 4,
                            .intervals = {7, 11, 14, 17, 21, 24, 28, 7},
                            .octaveModifiers = {-1, 0, 0, 0, 0, -1, 0, -1},
                            .semitoneModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .isActive = {1, 0, 0, 1, 1, 1, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 6
                            .degree = 3,
                            .intervals = {5, 9, 12, 16, 19, 23, 26, 5},
                            .octaveModifiers = {0, 0, 0, 0, 0, 0, 0, -1},
                            .semitoneModifiers = {0, 0, -1, 0, 1, 0, 0, 0},
                            .isActive = {1, 0, 1, 1, 1, 1, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 7
                            .degree = 3,
                            .intervals = {5, 9, 12, 16, 19, 23, 26, 5},
                            .octaveModifiers = {0, 0, 0, 0, -2, 0, -1, -1},
                            .semitoneModifiers = {0, -1, -1, 0, 0, -4, 0, 0},
                            .isActive = {1, 1, 1, 1, 1, 1, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 8
                            .degree = 0,
                            .intervals = {0, 4, 7, 11, 14, 17, 21, 0},
                            .octaveModifiers = {0, 0, 0, 0, 0, 0, 0, -1},
                            .semitoneModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .isActive = {1, 0, 1, 1, 1, 0, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 9
                            .degree = 0,
                            .intervals = {0, 4, 7, 11, 14, 17, 21, 0},
                            .octaveModifiers = {0, 1, 1, 0, 0, 0, 0, -1},
                            .semitoneModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .isActive = {1, 1, 1, 1, 0, 0, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 10
                            .degree = 3,
                            .intervals = {5, 9, 12, 16, 19, 23, 26, 5},
                            .octaveModifiers = {0, 0, 0, 0, 0, -1, 0, -1},
                            .semitoneModifiers = {0, -1, 0, -1, 0, -1, 0, 0},
                            .isActive = {1, 1, 0, 1, 1, 1, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 11
                            .degree = 6,
                            .intervals = {11, 14, 17, 21, 24, 28, 31, 11},
                            .octaveModifiers = {-1, 0, 0, -1, -1, -1, -1, -1},
                            .semitoneModifiers = {-1, 1, 0, -1, 0, -1, 0, 0},
                            .isActive = {1, 1, 0, 1, 1, 1, 1, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 12
                            .degree = 0,
                            .intervals = {0, 4, 7, 11, 14, 17, 21, 0},
                            .octaveModifiers = {0, 1, 0, 0, 0, 0, 0, -1},
                            .semitoneModifiers = {0, 0, 0, 0, 0, 2, 0, 0},
                            .isActive = {1, 1, 1, 1, 0, 1, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 13
                            .degree = 1,
                            .intervals = {2, 5, 9, 12, 16, 19, 23, 2},
                            .octaveModifiers = {0, 0, 0, 0, 0, 0, 0, -1},
                            .semitoneModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .isActive = {1, 1, 1, 1, 1, 1, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 14
                            .degree = 3,
                            .intervals = {5, 9, 12, 16, 19, 23, 26, 5},
                            .octaveModifiers = {0, 0, 0, 0, -2, -1, -1, -1},
                            .semitoneModifiers = {0, 0, 0, 1, 0, 0, 0, 0},
                            .isActive = {1, 1, 1, 1, 1, 1, 0, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 15
                            .degree = 5,
                            .intervals = {9, 12, 16, 19, 23, 26, 29, 9},
                            .octaveModifiers = {-1, 0, -1, -1, 0, 0, -1, -1},
                            .semitoneModifiers = {0, 1, 0, 1, 0, 0, -1, 0},
                            .isActive = {1, 1, 1, 1, 0, 0, 1, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}},
                           {// Chord 16
                            .degree = 5,
                            .intervals = {9, 12, 16, 19, 23, 26, 29, 9},
                            .octaveModifiers = {0, 1, 1, 0, 0, -1, -1, -1},
                            .semitoneModifiers = {0, 0, 0, 0, -2, 0, 0, 0},
                            .isActive = {1, 1, 1, 1, 1, 1, 1, 0},
                            .velocityModifiers = {0, 0, 0, 0, 0, 0, 0, 0},
                            .timing = {0, 0, 0, 0, 0, 0, 0, 0},
                            .channel = {0, 0, 0, 0, 0, 0, 0, 1}}};

#endif