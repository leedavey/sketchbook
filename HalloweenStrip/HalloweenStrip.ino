#include "LPD8806.h"
#include "SPI.h"

// Simple test for 160 (5 meters) of LPD8806-based RGB LED strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 160;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
int clockPin = 3;
int speed = 5;
const int lightNumb = 40;
const int FADEVAL = 1;
const int SLEEPMAX = 100;
const int MAXBRIGHTNESS = 127;
const int DELAY = 20;

int position[lightNumb];
int fade[lightNumb];
boolean fadeIn[lightNumb];
int sleep[lightNumb];
boolean red[lightNumb];

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
//LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

// You can optionally use hardware SPI for faster writes, just leave out
// the data and clock pin parameters.  But this does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
LPD8806 strip = LPD8806(nLEDs);

void setup() {
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  randomSeed(analogRead(0));
  for (int i=0; i<lightNumb; i++) {
    fadeIn[i] = true;
    fade[i] = 0;
    position[i] = random(nLEDs);
    sleep[i]=random(SLEEPMAX);
    if (random(2)) {
      red[i] = true;
    } else {
      red[i] = false;
    }
  }
}

void loop() {
  for (int i = 0; i < lightNumb; i++) {
    if (sleep[i] > 0) {
      sleep[i]--;
    } else {
      if (fadeIn[i] == true) {
        // light is ready to fade in
        fade[i] = fade[i] + 1;
        if (fade[i] > MAXBRIGHTNESS) {
          fade[i] = MAXBRIGHTNESS;
          fadeIn[i] = false;
        }
        strip.setPixelColor(position[i], strip.Color(fade[i],0,fade[i]/8));
      } else if (fadeIn[i] == false) {
        fade[i] = fade[i] - FADEVAL;
        if (fade[i] < 0) {
          fade[i] = 0;
        }
        strip.setPixelColor(position[i], strip.Color(fade[i],0,fade[i]/8));
      }
      if ((fadeIn[i] == false) && (fade[i] == 0)) {
        position[i]=random(nLEDs);
        fadeIn[i] = true;
        sleep[i]=random(SLEEPMAX);
        if (random(2)) {
          red[i] = true;
        } else {
          red[i] = false;
        }
      }
    }
  }
  strip.show();              // Refresh LED states
  delay(DELAY);
//  rainbowCycle(10);
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}
uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}
