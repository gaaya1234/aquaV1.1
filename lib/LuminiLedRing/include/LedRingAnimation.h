/*
  Created on Thu May 25 2023

  The MIT License (MIT)
  Copyright (c) 2023 HoustonAsh
  See the LICENSE file in this project for the full license terms.
*/

#ifndef LED_RING_ANIMATION
#define LED_RING_ANIMATION

#include <Arduino.h>
#include <FastLED.h>

#define DEFAULT_BRIGHTNESS 40

class LedRingAnimation {
protected:
  int64_t startAnimation;
  bool finished;
  int32_t durationStep;
  int32_t updateInterval;
public:
  virtual void process() = 0;
  void setup() {
    finished = false;
    startAnimation = millis();
  }
  inline bool isFinished() { return finished; }
};

#endif