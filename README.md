# PATS MP-16 DIY kit
This is the main repository for the PATS audio MP-16 DIY kit.

# Dependencies
To upload the code to the PATS MP-16 PCB you'll need the following board and library settings in Arduino IDE:

First add the RP2040 set of boards to the Arduino IDE (install it through the boards manager or get it from https://github.com/earlephilhower/arduino-pico) 
The PATS MP-16 DIY kit is based on the Waveshare RP2040 Zero module

Secondly, install the following libraries through the Arduino IDE Library manager or using the GitHub repositories:
* Adafruit TinyUSB Library (https://github.com/adafruit/Adafruit_TinyUSB_Arduino)
* Adafruit Neopixel Library (https://github.com/adafruit/Adafruit_NeoPixel)
* Adafruit SSD1306 Library (https://github.com/adafruit/Adafruit_SSD1306) (this will also install the Adafruit GFX library when done through the library manager)
* LittleFS_Mbed_RP2040 Library (https://github.com/khoih-prog/LittleFS_Mbed_RP2040)

# Uploading the code
1. Select the "Waveshare RP2040 Zero" as the board (Tools > Board: > Raspberry Pi Pico/RP2040/RP2350 > Waveshare RP2040 Zero)
2. Set the flash size to 512KB (Tools > Flash Size: > 2MB (Sketch: 1536KB, FS: 512KB))
3. Set the USB stack to Adafruit TinyUSB (Tools > USB Stack: > Adafruit TinyUSB)
4. Plug in the PATS MP-16 PCB into your computer.
5. Press and hold the BOOT button on the Waveshare RP2040 Zero module and then press and release RESET, then let go of BOOT. (Your PC should now show the module as a drive connected to your computer)
6. In the Arduino IDE, click Upload
7. Enjoy the MP-16!
