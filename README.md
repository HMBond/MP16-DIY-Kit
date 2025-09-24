# PATS MP-16 DIY kit

This is the main repository for the PATS audio MP-16 DIY kit. Check out PATS audio here: patsaudio.com

You can find the manual to the PATS MP-16 DIY kit here: https://docs.google.com/document/d/1Koq2ymRy1WZg_HTG9N3nUoAihR9oNaCTS2vanQYqxgY/
The manual has 3D printing and assembly instructions, as well as a description of how the main chord player firmware works.

# Dependencies

The PATS MP-16 DIY kit is based on the Waveshare RP2040 Zero module.
Add the "Raspberry Pi Pico/RP2040/RP2350" set of boards to the Arduino IDE (install it through the boards manager or get it from https://github.com/earlephilhower/arduino-pico)

Secondly, the firmware is dependent on the following libraries.
Install these through the Arduino IDE Library manager or using the GitHub repositories:

- Adafruit TinyUSB Library (https://github.com/adafruit/Adafruit_TinyUSB_Arduino)
- Adafruit Neopixel Library (https://github.com/adafruit/Adafruit_NeoPixel)
- Adafruit SSD1306 Library (https://github.com/adafruit/Adafruit_SSD1306) (this will also install the Adafruit GFX library when done through the library manager)
- LittleFS_Mbed_RP2040 Library (https://github.com/khoih-prog/LittleFS_Mbed_RP2040)

# Uploading the code

1. Select the "Waveshare RP2040 Zero" as the board (Tools > Board: > Raspberry Pi Pico/RP2040/RP2350 > Waveshare RP2040 Zero)
2. Set the flash size to 512KB (Tools > Flash Size: > 2MB (Sketch: 1536KB, FS: 512KB))
3. Set the Upload Method to Picotool (Tools > Upload Method > Picotool)
4. Set the USB stack to Adafruit TinyUSB (Tools > USB Stack: > Adafruit TinyUSB)
5. Plug in the PATS MP-16 PCB into your computer.
6. Press and hold the BOOT button on the Waveshare RP2040 Zero module and then press and release RESET, then let go of BOOT. (Your PC should now show the module as a drive connected to your computer)
7. In the Arduino IDE, click Upload
8. Enjoy the MP-16!

Check out the manual to see how the MP-16 firmware works.

# Contributing

The PATS MP-16 firmware is still a work in progress and there's many other functions I'd like to add.
If you feel like contributing to this project, please feel free to do so.
If you want to PATS MP-16 DIY kit yourself, shoot me a message over on the [PATS audio Instagram](https://www.instagram.com/patsaudio/).
