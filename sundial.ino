#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "RTClib.h"


RTC_DS1307 rtc;
int stripLen = 48;
int PIN = 10;
int led = 13;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(stripLen, PIN, NEO_GRB + NEO_KHZ800);
int timezone_modifier = -2; // BRC: -2 during bm

void setup () {
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);

  digitalWrite(17, HIGH);
  digitalWrite(16, LOW);
  
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  
  Serial.begin(9600);

#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); // Shield I2C pins connect to alt I2C bus on Arduino Due
#endif
  rtc.begin();

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}


void blink() {
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(10);
}

void loop () {
  blink();

  DateTime now = rtc.now();
  float h_adjusted = (now.hour() + timezone_modifier) % 24;
  float h = (h_adjusted + now.minute() / 60.0);
//  Serial.println(now.minute());
//  Serial.println(h);
    
  if(h > 6 & h <= 18) { // daytime
    for(int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0,0,0));
    }
    strip.show();
  } 
  // night time
  else {
    int active_led;

    if(h > 18) { // evening
      active_led = (h - 18) * 4 + 0.5;  // add 0.5 to round properly
    } 
    else { // morning
      active_led = h * 4 + 24 + 0.5; // add 0.5 to round properly
    }
//    Serial.println(active_led);
    for(int i=0; i< strip.numPixels(); i++) {
      uint32_t c = 0;
      if(i == active_led) {
        c = strip.Color(255,150,70);
      }
      strip.setPixelColor(i, c);
    }
    strip.show();
  }
    delay(5000);
}

