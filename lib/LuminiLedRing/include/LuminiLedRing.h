/*
  This is a library for Lumini Led Ring 1 inch.
           uno 5V POWER pin max current 450mA
           uno SCK - 13, MISO - 12, MOSI - 11, SS - 10
           lumini DI -> uno MOSI
           lumini CI -> uno SCLK
  reference site https://github.com/FastLED/FastLED/wiki/Pixel-reference

  Created on Thu May 25 2023

  The MIT License (MIT)
  Copyright (c) 2023 HoustonAsh
  See the LICENSE file in this project for the full license terms.
*/


#ifndef LUMINI_LED_RING_H
#define LUMINI_LED_RING_H

#include <Arduino.h>
#include <FastLED.h>
#include "LuminiLedRingContext.h"
#include "Animations/BlinkLedRingAnimation.h"
#include "Animations/LoadLedRingAnimation.h"
#include "Animations/FadeLedRingAnimation.h"

//CRGB::HTMLColorCode::

#ifndef LUMINI_DATA_PIN
#define LUMINI_DATA_PIN 8
#endif

#ifndef LUMINI_CLK_PIN
#define LUMINI_CLK_PIN 9
#endif

#define NUM_LEDS 20

class LuminiLedRing {
private:
  CRGB ring[NUM_LEDS];
  LuminiLedRingContext* context;

public:
  void setup(LuminiLedRingContext* ctx);

  void progress(CRGB rgb, uint8_t percentage);
  void enable(CRGB rgb);

  void blink(int32_t interval, int count = -1);
  void load(CRGB rgb, uint32_t duration, uint8_t taleScale = 40);
  void fade(uint32_t duration);
};
#endif
