//LIBRARIES
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>
#include <FastLED.h>
#include <SimpleTimer.h>

//LED STRIP PIN
#define LED_PIN     A0

//LED STRIP INFORMATION
#define NUM_LEDS    60
#define CHIPSET     WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int fadeAmount = 2;
int brightness = 0;

//SD CARD+ MP3 SHIELD SETUP
SdFat sd;
SFEMP3Shield MP3player;

//GLOBAL "SimpleTimer" OBJECT
SimpleTimer timer;

void fadeall() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(1000);
  }
}


void underWaterslow() {
   for(int i = 0; i < NUM_LEDS; i++ )
   {
   leds[i].setRGB(0,0,255); 
   leds[i].fadeLightBy(brightness);
  }
  FastLED.show();
  brightness = brightness + fadeAmount;
  if(brightness == 0 || brightness == 220)
  {
    fadeAmount = -fadeAmount ;
  }   
  delay(20); 
}

//PRINT CURRENT ARDUINO UP-TIME ON THE SERIAL PORT
void DigitalClockDisplay() {
  int h, m, s;
  s = millis() / 1000;
  m = s / 60;
  h = s / 3600;
  s = s - m * 60;
  m = m - h * 60;
  Serial.print(h);
  printDigits(m);
  printDigits(s);
  Serial.println();
}

void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}



void setup() {
  
  //SERIAL+LEDSTRIP SETUP
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  pinMode(A0, OUTPUT);

  //ACTIVATING THE SD CARD AND START RUNNING
  sd.begin(SD_SEL, SPI_HALF_SPEED);
  MP3player.begin();

  //CALLING TRACK 1 TO PLAY, HIGHEST VOLUME
  MP3player.playTrack(1);
  MP3player.setVolume(0, 0);

  //TIMED ACTIONS - VISUAL NARRATIVE SETUP
  timer.setInterval(1000, DigitalClockDisplay);
    //timer.setTimeout(5000, underWaterfast);
   // timer.setTimeout(15000, underWaterslow);

}

void loop() {
  //WHERE THE "POLLING" OCCURS
  timer.run();
  underWaterslow();
}
