/*
  Created on Thu May 25 2023

  The MIT License (MIT)
  Copyright (c) 2023 HoustonAsh
  See the LICENSE file in this project for the full license terms.
*/

#include "Animations/BlinkLedRingAnimation.h"

void BlinkLedRingAnimation::setup(int32_t updateInterval, int32_t blinkCnt) {
  LedRingAnimation::setup();
  this->updateInterval = updateInterval;
  this->blinkCnt = (blinkCnt << 1);
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}

void BlinkLedRingAnimation::process() {
  long int now = millis();
  if (now - startAnimation < updateInterval) return;
  startAnimation = now;
  FastLED.setBrightness((blinkCnt & 1) ? 0 : DEFAULT_BRIGHTNESS);
  FastLED.show();
  --blinkCnt;
  if (blinkCnt == 0)
    finished = true;
}