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
constexpr int VELOCITY_RANDOM_MAX = 100;

// Matrix Constants
constexpr int ROW_COUNT = 4;
constexpr int COL_COUNT = 4;
constexpr int PADS_COUNT = ROW_COUNT * COL_COUNT;

// Preset Constants
constexpr int SLOT_COUNT = 4;

// Define an array of chords as preset using standard C++ initialization
const Chord initialPreset[16] = {
    {
        // Chord 1
        0,                                                    // degree
        {0, 4, 7, 11, 14, 17, 21, 0},                         // intervals
        {0, 1, 0, 0, 0, 0, 0, -1},                            // octaveModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // semitoneModifiers
        {true, true, true, true, false, false, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                             // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                              // channel
    },
    {
        // Chord 2
        1,                                                    // degree
        {2, 5, 9, 12, 16, 19, 23, 2},                         // intervals
        {0, 1, 0, 0, 0, 0, 0, -1},                            // octaveModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // semitoneModifiers
        {true, true, true, true, false, false, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                             // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                              // channel
    },
    {
        // Chord 3
        2,                                                    // degree
        {4, 7, 11, 14, 17, 21, 24, 4},                        // intervals
        {0, 1, 0, 0, 0, 0, 0, -1},                            // octaveModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // semitoneModifiers
        {true, true, true, true, false, false, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                             // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                              // channel
    },
    {
        // Chord 4
        3,                                                    // degree
        {5, 9, 12, 16, 19, 23, 26, 5},                        // intervals
        {0, 1, 0, 0, 0, 0, 0, -1},                            // octaveModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // semitoneModifiers
        {true, true, true, true, false, false, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                             // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                              // channel
    },
    {
        // Chord 5
        4,                                                    // degree
        {7, 11, 14, 17, 21, 24, 28, 7},                       // intervals
        {-1, 0, 0, 0, 0, -1, 0, -1},                          // octaveModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // semitoneModifiers
        {true, false, false, true, true, true, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                             // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                              // channel
    },
    {
        // Chord 6
        3,                                                   // degree
        {5, 9, 12, 16, 19, 23, 26, 5},                       // intervals
        {0, 0, 0, 0, 0, 0, 0, -1},                           // octaveModifiers
        {0, 0, -1, 0, 1, 0, 0, 0},                           // semitoneModifiers
        {true, false, true, true, true, true, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                            // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                            // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                             // channel
    },
    {
        // Chord 7
        3,                                                  // degree
        {5, 9, 12, 16, 19, 23, 26, 5},                      // intervals
        {0, 0, 0, 0, -2, 0, -1, -1},                        // octaveModifiers
        {0, -1, -1, 0, 0, -4, 0, 0},                        // semitoneModifiers
        {true, true, true, true, true, true, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                           // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                           // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                            // channel
    },
    {
        // Chord 8
        0,                                                    // degree
        {0, 4, 7, 11, 14, 17, 21, 0},                         // intervals
        {0, 0, 0, 0, 0, 0, 0, -1},                            // octaveModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // semitoneModifiers
        {true, false, true, true, true, false, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                             // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                              // channel
    },
    {
        // Chord 9
        0,                                                    // degree
        {0, 4, 7, 11, 14, 17, 21, 0},                         // intervals
        {0, 1, 1, 0, 0, 0, 0, -1},                            // octaveModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // semitoneModifiers
        {true, true, true, true, false, false, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                             // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                             // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                              // channel
    },
    {
        // Chord 10
        3,                                                   // degree
        {5, 9, 12, 16, 19, 23, 26, 5},                       // intervals
        {0, 0, 0, 0, 0, -1, 0, -1},                          // octaveModifiers
        {0, -1, 0, -1, 0, -1, 0, 0},                         // semitoneModifiers
        {true, true, false, true, true, true, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                            // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                            // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                             // channel
    },
    {
        // Chord 11
        6,                                                  // degree
        {11, 14, 17, 21, 24, 28, 31, 11},                   // intervals
        {-1, 0, 0, -1, -1, -1, -1, -1},                     // octaveModifiers
        {-1, 1, 0, -1, 0, -1, 0, 0},                        // semitoneModifiers
        {true, true, false, true, true, true, true, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                           // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                           // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                            // channel
    },
    {
        // Chord 12
        0,                                                   // degree
        {0, 4, 7, 11, 14, 17, 21, 0},                        // intervals
        {0, 1, 0, 0, 0, 0, 0, -1},                           // octaveModifiers
        {0, 0, 0, 0, 0, 2, 0, 0},                            // semitoneModifiers
        {true, true, true, true, false, true, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                            // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                            // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                             // channel
    },
    {
        // Chord 13
        1,                                                  // degree
        {2, 5, 9, 12, 16, 19, 23, 2},                       // intervals
        {0, 0, 0, 0, 0, 0, 0, -1},                          // octaveModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                           // semitoneModifiers
        {true, true, true, true, true, true, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                           // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                           // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                            // channel
    },
    {
        // Chord 14
        3,                                                  // degree
        {5, 9, 12, 16, 19, 23, 26, 5},                      // intervals
        {0, 0, 0, 0, -2, -1, -1, -1},                       // octaveModifiers
        {0, 0, 0, 1, 0, 0, 0, 0},                           // semitoneModifiers
        {true, true, true, true, true, true, false, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                           // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                           // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                            // channel
    },
    {
        // Chord 15
        5,                                                   // degree
        {9, 12, 16, 19, 23, 26, 29, 9},                      // intervals
        {-1, 0, -1, -1, 0, 0, -1, -1},                       // octaveModifiers
        {0, 1, 0, 1, 0, 0, -1, 0},                           // semitoneModifiers
        {true, true, true, true, false, false, true, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                            // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                            // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                             // channel
    },
    {
        // Chord 16
        5,                                                 // degree
        {9, 12, 16, 19, 23, 26, 29, 9},                    // intervals
        {0, 1, 1, 0, 0, -1, -1, -1},                       // octaveModifiers
        {0, 0, 0, 0, -2, 0, 0, 0},                         // semitoneModifiers
        {true, true, true, true, true, true, true, false}, // isActive
        {0, 0, 0, 0, 0, 0, 0, 0},                          // velocityModifiers
        {0, 0, 0, 0, 0, 0, 0, 0},                          // timing
        {0, 0, 0, 0, 0, 0, 0, 1}                           // channel
    }};

#endif