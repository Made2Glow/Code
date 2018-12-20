// FastXmas twinkle of red/green/white colors randomly.
// By Chris Alcock (Aka mBlade/Made2Glow) Dec 19, 2018

// This animation was taken from Gem Raptor code.

#include <FastLED.h>

#define DATA_PIN          2
#define NUM_LEDS          26
#define NUM_XMAS_PIXELS   NUM_LEDS/4
#define BRIGHTNESS        64

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
    if (xmas_val[i] == 192) {
      leds[xmas_pos[i]] = CRGB(0,0,0); // the sin8() function doesn't go right to zero. This ensures we don't go into an infinate loop in the freepos() function.
      xmas_pos[i] = freepos(); // Choose new open position.
      xmas_color[i] = random(3); // randomize the new color.
    }
    xmas_val[i]++;

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
  FastLED.show();
}

void loop() {
  xmasmode();

  FastLED.show();
  delay(10);
}
