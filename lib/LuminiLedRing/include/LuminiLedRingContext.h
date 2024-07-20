/*
  Created on Thu May 25 2023

  The MIT License (MIT)
  Copyright (c) 2023 HoustonAsh
  See the LICENSE file in this project for the full license terms.
*/

#ifndef LUMINI_LED_RING_CONTEXT_H
#define LUMINI_LED_RING_CONTEXT_H

#include "LedRingAnimation.h"

class LuminiLedRingContext {
  LedRingAnimation* animation;
public:
  void process() {
    if (animation == nullptr) return;
    if (animation->isFinished()) return;

    animation->process();
  }

  void setAnimation(LedRingAnimation* a) { animation = a; }
  LedRingAnimation* getAnimation() { return animation; }
};

#endif