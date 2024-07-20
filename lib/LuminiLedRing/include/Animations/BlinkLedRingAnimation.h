/*
  Created on Thu May 25 2023

  The MIT License (MIT)
  Copyright (c) 2023 HoustonAsh
  See the LICENSE file in this project for the full license terms.
*/

#ifndef BLINK_LED_RING_ANIMATION_H
#define BLINK_LED_RING_ANIMATION_H

#include "LedRingAnimation.h"

class BlinkLedRingAnimation : public LedRingAnimation {
  int32_t blinkCnt;
public:
  void setup(int32_t updateInterval, int32_t blinkCnt = INT_FAST32_MAX);
  void process();
};

#endif