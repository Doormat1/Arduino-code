#include <OneButton.h>
#include <FastLED.h>
#define NUM_LEDS  14
#define LED_PIN   2
#define BTN_PIN   3
OneButton btn = OneButton(BTN_PIN, true, true);
boolean demomode = false;
uint8_t hue = 0;
uint8_t jump = 0;
int runs = 0;
int rocketstart = 5;
int h = rocketstart - 1;
int i = rocketstart - 1;
int j = rocketstart - 1;
int f = NUM_LEDS;
int filled = 0;
CRGB leds[NUM_LEDS];
uint8_t colorIndex[NUM_LEDS];
uint8_t paletteIndex = 200;
uint8_t redandwhiteindex = 0;
int whichpattern = 0;
long timeforpattern = 30000; //time you want each pattern to display in milliseconds
long jumptime = 0;
int numberofpatterns = 9; //number of different patterns outlined in cases
DEFINE_GRADIENT_PALETTE (heatmap_gp) {
  0,   255,   0,   0,   //red
  128, 252,   177,   3,   //bright Orange
  200, 255, 255,   0,   //bright yellow
  255, 255, 255, 255    //full white
};
CRGBPalette16 redwhite = CRGBPalette16 (
CRGB::Red,  CRGB::White,  CRGB::Red,  CRGB::White,
CRGB::Red,  CRGB::White,  CRGB::Red,  CRGB::White,
CRGB::Red,  CRGB::White,  CRGB::Red,  CRGB::White,
CRGB::Red,  CRGB::White,  CRGB::Red,  CRGB::White);

CRGBPalette16 rocketflames = CRGBPalette16 (
CRGB::Red,  CRGB::Red,  CRGB::OrangeRed,  CRGB::DarkOrange,
CRGB::DarkOrange, CRGB::Orange, CRGB:: Gold,  CRGB::Yellow,
CRGB::Red,  CRGB::Red,  CRGB::OrangeRed,  CRGB::DarkOrange,
CRGB::DarkOrange, CRGB::Orange, CRGB:: Gold,  CRGB::Yellow);

CRGBPalette16 firePalette = HeatColors_p;

CRGBPalette16 myPal = heatmap_gp;

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  btn.attachClick(nextPattern);
  btn.attachLongPressStart(demo);
  btn.attachDoubleClick(restartpattern);
  Serial.begin(57600);
}

void loop() {
  btn.tick();
  Serial.println(whichpattern);
  if (demomode == true) {
    jumptime = timeforpattern * numberofpatterns / 256;

    whichpattern = jump / (256 / numberofpatterns);
    EVERY_N_MILLISECONDS(jumptime) {
      jump++;
      if (jump > (256 * 2 / numberofpatterns))
      {
        runs = 0;
      }
    }
  }

  startpattern(whichpattern);
}
void startpattern(int k) {
  switch (k) {
    case 0: rainbow();
      break;
    case 1: launch();
      break;
    case 2: launchandfill();
      break;
    case 3: wavey();
      break;
    case 4: justwhite();
      break;
    case 5: redandwhite();
      break;
    case 6: colourcycle();
      break;
    case 7: rocketcandycane();
      break;
    case 8: fire();
      break;
  }
}
void restartpattern() {
  whichpattern = 0;
    hue = 0;
  filled=0;
  jump = 0;
  runs=0;
}

void demo() {
  demomode = !demomode;
  whichpattern=0;
  hue = 0;
  filled=0;
  jump = 0;
  runs=0;
}

void nextPattern() {
  whichpattern++;
  hue = 0;
  filled=0;
  jump = 0;
  runs=0;
  if (whichpattern == numberofpatterns)whichpattern = 0;
}
void rainbow() {
  for (int i = 0; i < NUM_LEDS; i++) {
    //leds[i] = CHSV(hue, 255, 255);
    leds[i] = CHSV(hue + (i * 10), 255, 255);
  }

  EVERY_N_MILLISECONDS(15) {
    hue++;
  }

  FastLED.show();
}
void launch() {
  for (int i = rocketstart; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 0, 0);
  }
  if (runs == 0) {
    for (int i = 0; i < rocketstart; i++) {
      leds[i] = ColorFromPalette(myPal, i * 4, 255, LINEARBLEND);
    }
    runs++;
  }
  FastLED.show();

  EVERY_N_MILLISECONDS(200) {
    if (j < -1)j = rocketstart - 1;
    leds[j] = CRGB::White;
    FastLED.show();
    leds[j + 1] = ColorFromPalette(myPal, (j + 1) * 4, 255, LINEARBLEND);
    j--;
  }
  EVERY_N_MILLISECONDS(100) {
    if (i < -1)i = rocketstart - 1;
    leds[i] = CRGB(250, 250, 250);
    FastLED.show();
    leds[i + 1] = ColorFromPalette(myPal, (i + 1) * 4, 255, LINEARBLEND);
    i--;
  }
  EVERY_N_MILLISECONDS(150) {
    if ( h < -1)h = rocketstart - 1;
    leds[h] = CRGB::White;
    FastLED.show();
    leds[h + 1] = ColorFromPalette(myPal, (h + 1) * 4, 255, LINEARBLEND);
    h--;
  }
}

void launchandfill() {
  for (int i = rocketstart; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 255);
  }
  FastLED.show();
  EVERY_N_MILLISECONDS(200) {
    if (j <= filled) {
      j = rocketstart - 1;
      filled++;

    }
    if (filled == 0) {
      for (int l = 0; l < rocketstart; l++) {
        leds[l] = ColorFromPalette(myPal, l * 4, 255, LINEARBLEND);
      }
      FastLED.show();
    }
    leds[j] = CRGB::White;
    leds[j + 1] = ColorFromPalette(myPal, (j + 1) * 4, 255, LINEARBLEND);
    FastLED.show();
    j--;
    if (filled == rocketstart)
    {
      filled = 0;
    }

    if (filled != 0) {
      for (int i = 0; i < filled; i++) {
        leds[i] = CRGB(240, 240, 240);
      }
      FastLED.show();
    }
  }

}
void wavey() {
  uint8_t sinBeat1 = beatsin8(40, 0, 255, 0, 0);
  uint8_t sinBeat2 = beatsin8(15, 0, NUM_LEDS, 0, 0);
  leds[sinBeat2] = CHSV(sinBeat1, 255, 240);
  blur1d(leds, NUM_LEDS, 10);
  FastLED.show();
}
void justwhite() {
  uint8_t sinBeat1 = beatsin8(10, 10, 100, 0, 0);
  fill_solid(leds, NUM_LEDS, CRGB::White);
  FastLED.setBrightness(sinBeat1);
  FastLED.show();
}
void redandwhite() {
  for (int f = 0; f < NUM_LEDS; f++) {
    leds[f] = ColorFromPalette(redwhite, f * 16 + redandwhiteindex * 16, 255);
  }
  FastLED.show();
  EVERY_N_MILLISECONDS(500) {
    redandwhiteindex++;
  }
}

void colourcycle() {
  fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 200));
  FastLED.show();
  EVERY_N_MILLISECONDS(250) {
    hue = hue + 5;
  }

}
void rocketcandycane() {
  for (int i = rocketstart; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 255);
  }
  for (int f = 0; f < rocketstart; f++) {
    leds[f] = ColorFromPalette(rocketflames, f * 16 + redandwhiteindex * 7, 255);
  }
  FastLED.show();
  EVERY_N_MILLISECONDS(150) {
    redandwhiteindex++;
  }
  filled = 0;
  runs = 0;
}

void fire(){
  leds[1]=CRGB::White;
  for (int i = rocketstart; i < NUM_LEDS; i++) {
    leds[i] = CRGB(255, 0, 255);
  }
  EVERY_N_MILLISECONDS(5) {
  int  a = millis();
  
  for (int i = 0; i < rocketstart; i++) {

    // 3D noise, x is constant, we move through time on y and z axis
    // The 60 here will change the scale of the effect, lower is smoother
    // higher is more flickery. The time value for z was in the original code
    // as that was a 2D matrix version. I've left it in here as it looks 
    // nice in 1D too!
    uint8_t noise = inoise8 (0 , i * 60 + a , a / 3);

    // Divides 255 by (NUM_LEDS - 1) and subtracts that amount away from 255 to return 
    // a decreasing number each time e.g. for NUM_LEDS = 18, difference between
    // each point is 15, so:
    // i = 0, math = 255
    // i = 1, math = 240
    // i = 2, math = 225
    // ...
    // i = NUM_LEDS, math =  0
    uint8_t math = abs8(i - (rocketstart-1)) * 255 / (rocketstart-1);

    // Take the noise function from earlier and subtract math from it,
    // so the further along the strip you are, the higher palette index you
    // are likely to get. This results in the base of the fire (white, yellow)
    // being at the far end of the strip
    uint8_t index = qsub8 (noise, math);

    // Set the LED color from the palette
    leds[i] = ColorFromPalette (firePalette, index, 255);    
  }  
}
FastLED.show();
}
