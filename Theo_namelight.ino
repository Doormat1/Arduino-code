#include "FastLED.h"
#include "OneButton.h"
#define NUM_LEDS      25  //LED 0-7 underline, 8-11 o, 12-15 e, 16-20 h , 21-24 T
#define LED_TYPE   WS2812B
#define COLOR_ORDER   GRB
#define DATA_PIN        2
#define BTN_PIN 3
CRGBArray<NUM_LEDS> leds;
OneButton btn = OneButton(BTN_PIN, true, true);
boolean demomode = false;
boolean up = true;
boolean hold = false;
int pattern = 0;
int k = 0;
uint8_t hue = 0;
uint8_t brightness = 0;
uint8_t speed = 255;
int numberofpatterns = 9; //how many cases below
const int numberofletters = 5;
int letters [numberofletters][8] {
  {21,22,23,24}, //T
  {16,17,18,19,20}, //h
  {12,13,14,15}, //e
  {8,9,10,11}, //o
  {0, 1, 2, 3, 4, 5,6,7}, //underline
};
int characters [] {4, 5, 4, 4, 8}; //how many leds in each character from left to right with line last
const int numberoflines =4;
int lines [numberoflines][8] {
  {24,23,18}, //Top line
  {22,19,17,14,13,10,9}, //middle letters
  {21,20,16,15,12,11,8}, //bottom letters
  {0, 1, 2, 3, 4, 5, 6,7}, //underline
};
int numberlines [] {3, 7, 7, 8}; // how many leds on each line

void setup() {
  Serial.begin(115200);
  Serial.println("Reset");
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
    EVERY_N_SECONDS(30) {
      FastLED.setBrightness (255);
      pattern++;
      Serial.println (pattern);
      if (pattern > numberofpatterns) pattern = 0;
    }
  };
  switch (pattern) {
    case 0:  //cycle colours 
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
        EVERY_N_SECONDS(30) {
          hold = false;
          up=false;
        }
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
    case 1:  //each letter different colour
      btn.tick();
      for (int i = 0; i < numberofletters; i++) {
        for (int j = 0; j < characters[i]; j++) {
          btn.tick();
          leds[letters[i][j]] = CHSV(hue + (i * 255 / (numberofletters)), 255, 100);
        }
      }
      EVERY_N_SECONDS(10)
      {
        hue = hue + 255 / (numberofletters);
      }
      break;
    case 2: // each line different colour
      btn.tick();
      for (int i = 0; i < numberoflines; i++) {
        for (int j = 0; j < numberlines[i]; j++) {
          btn.tick();
          leds[lines[i][j]] = CHSV(hue + (i * 255 / numberoflines), 255, 100);
        }
      }
      break;
    case 3: //each letter different colour cycle colours
      btn.tick();
      EVERY_N_MILLISECONDS (1000)
      { k++;
        hue = hue + 255 / numberofletters;
        btn.tick();
        if (k > numberofletters) {
          k = 0;
        };
      }

      for (int j = 0; j < characters[k]; j++) {
        leds[letters[k][j]] = CHSV(hue, 255, 100);
        btn.tick();
        FastLED.show();
      }
      break;
    case 4: //each line different colour cycle colours
      btn.tick();
      EVERY_N_MILLISECONDS (1000)
      { k++;
        hue = hue + 255 / numberoflines;
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
    case 5: //white fade up and down
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
    case 6: //raninbow fill
      fill_rainbow( leds, NUM_LEDS, hue, 3);
      EVERY_N_SECONDS(5) {
        hue = hue + 10;
      }
      btn.tick();
      break;

    case 7: //rainbow glitter
      fadeToBlackBy( leds, NUM_LEDS, 10);
      leds[random8(NUM_LEDS)] += CHSV(hue + random16(255), 200, 100);
      btn.tick();
      break;
    
    case 8: //rainbow random sparkle
     fill_rainbow( leds, NUM_LEDS, hue, 5);
      EVERY_N_SECONDS(5) {
        hue = hue + 10;
      }
       if( random8() < 80) {
      leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
      btn.tick();

      break;
    case 9: //one by one light up
      btn.tick();
      if (speed<1){
        speed=255;
      }
      EVERY_N_MILLISECONDS(speed){
        fadeToBlackBy( leds, NUM_LEDS, 100);
        btn.tick();
        leds[k]=CHSV(hue, 255, 100);
        k++;
        if (k>NUM_LEDS){k=0;
        speed=speed/2;
        Serial.println(speed);}
        
        
      }
      EVERY_N_SECONDS(5) {
        hue = hue + 10;
      }
      break;
  }
  FastLED.show();
  btn.tick();
}

void nextPattern () {
  FastLED.setBrightness (255);
  pattern++;
  k=0;
  hold = false;
  if (pattern > numberofpatterns) pattern = 0;
  Serial.println(pattern);
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
  Serial.println("demo");
}
