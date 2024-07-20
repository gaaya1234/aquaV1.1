/*
  Created on Thu May 25 2023

  The MIT License (MIT)
  Copyright (c) 2023 HoustonAsh
  See the LICENSE file in this project for the full license terms.
*/

#ifndef LOADING_LED_RING_ANIMATION
#define LOADING_LED_RING_ANIMATION

#include "LedRingAnimation.h"

class LoadLedRingAnimation : public LedRingAnimation {
  CRGB color;
  uint8_t taleScale;
  uint8_t it = 0;

  CRGB* ring;
  uint16_t ringSize;
public:
  void setup(CRGB rgb, int32_t duration, uint8_t taleScale);
  void process();

  void setLed(CRGB* ring, uint16_t ringSize) {
    this->ring = ring;
    this->ringSize = ringSize;
  }
};

#endif