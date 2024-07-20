/*
  Created on Thu May 25 2023

  The MIT License (MIT)
  Copyright (c) 2023 HoustonAsh
  See the LICENSE file in this project for the full license terms.
*/

#include "Animations/FadeLedRingAnimation.h"

void FadeLedRingAnimation::setup(int32_t duration) {
  LedRingAnimation::setup();
  this->finished = false;
  this->updateInterval = duration / DEFAULT_BRIGHTNESS / 2;
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}

void FadeLedRingAnimation::process() {
  long int now = millis();
  if (now - lastUpdateTime < updateInterval) return;
  lastUpdateTime = now;

  long int t = (now - startAnimation) / updateInterval;

  uint8_t currentB = abs(DEFAULT_BRIGHTNESS - t) % (DEFAULT_BRIGHTNESS + 1);
  if (currentB == DEFAULT_BRIGHTNESS && t > DEFAULT_BRIGHTNESS)
    startAnimation = now;

  FastLED.setBrightness(currentB);
  FastLED.show();
}