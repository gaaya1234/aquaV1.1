/*
  Created on Thu May 25 2023

  The MIT License (MIT)
  Copyright (c) 2023 HoustonAsh
  See the LICENSE file in this project for the full license terms.
*/

#include "LuminiLedRing.h"
#include "Arduino.h"
#include "FastLED.h"


BlinkLedRingAnimation blinkLedRingAnimation;
LoadLedRingAnimation loadLedRingAnimation;
FadeLedRingAnimation fadeLedRingAnimation;

void LuminiLedRing::setup(LuminiLedRingContext* ctx) {
  FastLED.setMaxRefreshRate(0, false);
  FastLED.addLeds<ESPIChipsets::DOTSTAR, LUMINI_DATA_PIN, LUMINI_CLK_PIN, EOrder::BGR>(ring, NUM_LEDS);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
  context = ctx;

  loadLedRingAnimation.setLed(ring, NUM_LEDS);
}

/// @brief Turn on given percentage of lights
/// show progress bar 0-100% by "percentage" parameter and "rgb" value
/// @param rgb 
/// @param percentage [0; 100]
void LuminiLedRing::progress(CRGB rgb, uint8_t percentage) {
  context->setAnimation(nullptr);
  percentage = map(percentage, 0, 100, 0, NUM_LEDS);
  for (int i = 0; i < NUM_LEDS; ++i) ring[i] = CRGB::Black;
  for (int i = 0; i < percentage; ++i) ring[i] = rgb;
  // memset(ring, CRGB::Black, sizeof(CRGB) * NUM_LEDS);
  // memset(ring, rgb, sizeof(CRGB) * percentage);
  FastLED.show();
}

/// @brief Turn on all lights
/// @param rgb color
void LuminiLedRing::enable(CRGB rgb) {
  context->setAnimation(nullptr);
  for (int i = 0; i < NUM_LEDS; ++i) ring[i] = rgb;
  // memset(ring, rgb, sizeof(CRGB) * NUM_LEDS);
  FastLED.show();
}

/// @brief Start blink animation.
/// @param duration duration time in ms
/// @param interval blink time
/// @param count number of blinks
void LuminiLedRing::blink(int32_t interval, int count) {
  context->setAnimation((LedRingAnimation*)&blinkLedRingAnimation);
  blinkLedRingAnimation.setup(interval, count);
}

/// @brief Start load animation
/// @param rgb color
/// @param duration time of full rotation in ms
void LuminiLedRing::load(CRGB rgb, uint32_t duration, uint8_t taleScale) {
  context->setAnimation((LedRingAnimation*)&loadLedRingAnimation);
  loadLedRingAnimation.setup(rgb, duration, taleScale);
}

/// @brief Start fade animation
/// @param duration duration time in ms
void LuminiLedRing::fade(uint32_t duration) {
  context->setAnimation((LedRingAnimation*)&fadeLedRingAnimation);
  fadeLedRingAnimation.setup(duration);
}