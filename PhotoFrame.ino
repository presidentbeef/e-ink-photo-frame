// Adafruit_ImageReader "photo frame" for Adafruit E-Ink displays.
// Loads series of images from SD card to the screen.
//
// Based on https://github.com/adafruit/Adafruit_ImageReader/blob/master/examples/EInkBreakouts/EInkBreakouts.ino

#include <Adafruit_GFX.h>  // Core graphics library
#include "Adafruit_ThinkInk.h"
#include <SdFat.h>  // SD card & FAT filesystem library
#include <Adafruit_ImageReader_EPD.h>  // Image-reading functions

#define EPD_DC 33     // can be any pin, but required!
#define EPD_CS 27     // can be any pin, but required!
#define SRAM_CS 15    // can set to -1 to not use a pin (uses a lot of RAM!)
#define EPD_BUSY -1   // can set to -1 to not use a pin (will wait a fixed delay)
#define EPD_RESET -1  // can set to -1 and share with chip Reset (can't deep sleep)
#define SD_CS 32      // SD card chip select

// Tri-Color Display
ThinkInk_213_Tricolor_RW display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

SdFat SD;                             // SD card filesystem
Adafruit_ImageReader_EPD reader(SD);  // Image-reader object, pass in SD filesys

void setup(void) {
  Serial.begin(115200);

  display.begin();

  // The Adafruit_ImageReader constructor call (above, before setup())
  // accepts an uninitialized SdFat or FatVolume object. This MUST
  // BE INITIALIZED before using any of the image reader functions!
  Serial.print(F("Initializing filesystem..."));

  // SD card is pretty straightforward, a single call...
  if (!SD.begin(SD_CS, SD_SCK_MHZ(10))) {  // Breakouts require 10 MHz limit due to longer wires
    Serial.println(F("SD begin() failed"));
    for (;;)
      ;  // Fatal error, do not continue
  }

  Serial.println(F("OK!"));
}

void loop() {
  int num_images = 4; // Update with number of images

  // List image paths
  char *images[num_images] = {
    "/image1.bmp",
    "/image2.bmp",
    "/image3.bmp",
    "/image4.bmp",
  };

  for (int i = 0; i < num_images; i++) {
    display.setRotation(2);  // Set rotation - 0 is "normal", 2 is "upside-down"
    display.fillScreen(0);   // and clear screen
    display.clearBuffer();

    Serial.printf("Loading image %s to canvas...", images[i]);
    ImageReturnCode stat = reader.drawBMP(images[i], display, 0, 0);
    reader.printStatus(stat);  // Did it work?
    display.display();

    delay(3 * 60000);  // Pause 5 minutes.
  }
}