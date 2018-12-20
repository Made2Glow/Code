// FastXmas a twinkle animation of red/green/white colors randomly.
// By Chris Alcock (Aka mBlade/Made2Glow) Dec 19, 2018
// Tested on a Teensy 3.2 and Nano v3 (Atmega 328p)

// You will require the FastLED library from http://fastled.io/
// This code is found at https://github.com/Made2Glow/Code/tree/master/SmallAnimations/FastXmas

// Modify or change this code as you see fit. It is for free to use. 
// Credit would be nice if you post on social media but not required.
// Twitter: @made2glow
// Facebook: https://www.facebook.com/made2glow/ (I don't post here at all, but it exists)

#include <FastLED.h>

#define DATA_PIN          2
#define NUM_LEDS          26
#define BRIGHTNESS        64
#define NUM_XMAS_PIXELS   NUM_LEDS/4 // Change this to a lower value to add more pixels!

bool _runonce =           true;
CRGB leds[NUM_LEDS];

// Helper function that returns a free/unused position pixel. 
uint16_t freepos() {
  uint16_t randpos;
  while(1) { // Search for a new position.
    randpos = random(0,NUM_LEDS);
    if (leds[randpos].r+leds[randpos].g+leds[randpos].b == 0)
      return randpos; // once found, return it.
  }
}

void xmasmode() {
  
  // values/intensity/color of each pixel session.
  static uint8_t xmas_val[NUM_XMAS_PIXELS];
  static uint16_t xmas_pos[NUM_XMAS_PIXELS];
  static uint8_t xmas_color[NUM_XMAS_PIXELS];

  if (_runonce) { // This is to randomize the values/positions/colors at the start once.
    _runonce = false; 
    for (uint16_t i = 0;i < NUM_XMAS_PIXELS; i++) {
      xmas_val[i] = random8();
      xmas_pos[i] = random(0,NUM_LEDS);
      xmas_color[i] = random(3);
    }
  }

  for (uint16_t i = 0;i < NUM_XMAS_PIXELS; i++) {
    if (xmas_val[i] == 192) { // 192 is 1 for sin8() which siginifies the end of the glow cycle and choose a new place to glow.
      leds[xmas_pos[i]] = CRGB(0,0,0); // the sin8() function doesn't go right to zero. This ensures we don't go into an infinate loop in the freepos() function.
      xmas_pos[i] = freepos(); // Choose new open position.
      xmas_color[i] = random(3); // randomize the new color.
    }
    xmas_val[i] ++; // Add 1 for ever. After 255 it will wrap back to 0. 
    
    
    switch (xmas_color[i]) {
      case 0: leds[xmas_pos[i]] = CHSV(0,255,sin8(xmas_val[i])); break;
      case 1: leds[xmas_pos[i]] = CHSV(96,255,sin8(xmas_val[i])); break;
      case 2: leds[xmas_pos[i]] = CHSV(0,0,map(sin8(xmas_val[i]),0,255,0,240)); break;
      default: break;
    }
  }
}


void setup() {
  FastLED.addLeds<WS2812,DATA_PIN,GRB>(leds,NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show(); // Clear the strip first.
}

void loop() {
  xmasmode(); // run the function.

  FastLED.show();
  delay(10); // delay 10ms (100 fps)
}
