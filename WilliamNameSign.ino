#include "FastLED.h"
#include "OneButton.h"
#define NUM_LEDS      39   //LED 0-8 underline, 9-14 m, 15-18 a, 19-21 i,22-24 l, 25-27 l, 28-30 i, 31-38 W.
#define LED_TYPE   WS2812B
#define COLOR_ORDER   GRB
#define DATA_PIN        2
#define BTN_PIN 3
CRGBArray<NUM_LEDS> leds;
OneButton btn = OneButton(BTN_PIN, true, true);
boolean demomode = false;
boolean up = true;
boolean hold = false;
int pattern = 8;
int k = 0;
int holdcount=0;
uint8_t hue = 0;
uint8_t brightness = 0;
int numberofpatterns = 8;
/*int line []={0,1,2,3,4,5,6,7,8};
  int m [] = {9,10,11,12,13,14};
  int a [] = {15,16,17,18};
  int i2 [] = {19,20,21};
  int l2[] = {22,23,24};
  int l1[] = {25,26,27};
  int i1[] = {28,29,30};
  int w[] = {31,32,33,34,35,36,37,38};*/
int letters [8][9] {
  {31, 32, 33, 34, 35, 36, 37, 38}, //w
  {28, 29, 30}, //i1
  {25, 26, 27}, //l1
  {22, 23, 24}, //l2
  {19, 20, 21}, //i2
  {15, 16, 17, 18}, //a
  {9, 10, 11, 12, 13, 14}, //m
  {0, 1, 2, 3, 4, 5, 6, 7, 8}, //underline
};
int characters [] {8, 3, 3, 3, 3, 4, 6, 9};
int lines [4][12] {
  {36, 35, 31, 30, 25, 24, 19},
  {37, 34, 32, 29, 26, 23, 20, 18, 15, 14, 11, 10},
  {38, 33, 28, 27, 22, 21, 17, 16, 13, 12, 9},
  {0, 1, 2, 3, 4, 5, 6, 7, 8},
};
int numberlines [] {7, 12, 11, 9};
/*int topline[] = {36,35,31,30,25,24,19};
  int middleline[]={37,34,32,28,26,23,20,18,15,14,11,10};
  int bottomline[]={38,33,28,27,22,21,17,16,13,12,9};*/
void setup() {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setCorrection (Tungsten100W);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite (LED_BUILTIN, demomode);
  btn.attachClick(nextPattern);
  btn.attachLongPressStart(demo);
  btn.attachDoubleClick(restartpattern);
  for (int i = 0; i < NUM_LEDS; i++)
  { leds[i] = CRGB::Red,
              FastLED.show();
  }
  for (int i = 0; i < NUM_LEDS; i++)
  { leds[i] = CRGB::Green,
              FastLED.show();
  }
  for (int i = 0; i < NUM_LEDS; i++)
  { leds[i] = CRGB::Blue,
              FastLED.show();
  }
}

void loop() {
  btn.tick();
  if (demomode == true) {
    EVERY_N_SECONDS(300) {
      FastLED.setBrightness (255);
      pattern++;
      if (pattern > numberofpatterns) pattern = 0;
    }
  };
  switch (pattern) {
    case 0:
      btn.tick();
      if (up == true) {
        btn.tick();
        EVERY_N_MILLISECONDS(10) {
          brightness++;
          if (brightness > 200)
          {
            hold = true;
          }
        }
      };
      while (hold == true) {
        btn.tick();
        EVERY_N_SECONDS(30) 
          hold = false;
          up=false;     
        }
      
      if (up == false) {
        btn.tick();
        EVERY_N_MILLISECONDS(10) {
          brightness--;
          if (brightness < 15) {
            up = true;
            hue = hue + 20;
          }
        }
      };
      for (int i = 0; i < NUM_LEDS; i++) {
        btn.tick();
        FastLED.setBrightness (brightness);
        leds[i] = CHSV(hue, 255, brightness);
      }

      break;
    case 1:
      btn.tick();
      for (int i = 0; i < 8; i++) {
        for (int j = 0; j < characters[i]; j++) {
          btn.tick();
          leds[letters[i][j]] = CHSV(hue + (i * 255 / 9), 255, 100);
        }
      }
      EVERY_N_SECONDS(10)
      {btn.tick();
        hue = hue + 255 / 9;
      }
      btn.tick();
      break;
    case 2:
      btn.tick();
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < numberlines[i]; j++) {
          btn.tick();
          leds[lines[i][j]] = CHSV(hue + (i * 255 / 4), 255, 100);
        }
      }
      break;
    case 3:
      btn.tick();
      EVERY_N_MILLISECONDS (1000)
      { k++;
        hue = hue + 255 / 9;
        btn.tick();
        if (k > 7) {
          k = 0;
        };
      }

      for (int j = 0; j < characters[k]; j++) {
        leds[letters[k][j]] = CHSV(hue, 255, 100);
        btn.tick();
        FastLED.show();
      }
      break;
    case 4:
      btn.tick();
      EVERY_N_MILLISECONDS (1000)
      { k++;
        hue = hue + 255 / 4;
        btn.tick();
        if (k > 3) {
          k = 0;
        };
      }

      for (int j = 0; j < numberlines[k]; j++) {
        leds[lines[k][j]] = CHSV(hue, 255, 100);
        btn.tick();
        FastLED.show();
      }
      break;
    case 5:
      if (up == true) {
        btn.tick();
        EVERY_N_MILLISECONDS(100) {
          btn.tick();
          brightness++;
          if (brightness > 200) {
            up = false;
          }
        }
      };
      if (up == false) {
        btn.tick();
        EVERY_N_MILLISECONDS(100) {
          brightness--;
          if (brightness < 15) {
            up = true;
          }
        }
      };
      for (int i = 0; i < NUM_LEDS; i++) {
        FastLED.setBrightness (brightness);
        leds[i] = CRGB:: White;
      }
      break;
    case 6:
      fill_rainbow( leds, NUM_LEDS, hue, 5);
      EVERY_N_SECONDS(5) {
        hue = hue + 10;
      }
      btn.tick();
      break;

    case 7:
      fadeToBlackBy( leds, NUM_LEDS, 10);
      leds[random8(NUM_LEDS)] += CHSV(hue + random16(255), 200, 100);
      btn.tick();
      break;
    
    case 8:
    FastLED.setBrightness(150);
     fill_rainbow( leds, NUM_LEDS, hue, 5);
      EVERY_N_SECONDS(5) {
        hue = hue + 10;
      }
       if( random8() < 40) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
      btn.tick();

      break;
  }
  FastLED.show();
  btn.tick();
}

void nextPattern () {
  FastLED.setBrightness (255);
  pattern++;
  hold = false;
  if (pattern > numberofpatterns) pattern = 0;
}

void restartpattern () {
  hue = hue + 10;
  hold = false;

}

void demo () {
  demomode = !demomode;
  digitalWrite (LED_BUILTIN, demomode);
  hue = 0;
  k = 0;
  pattern = 0;
  hold = false;
}
