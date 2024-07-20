#ifndef BUZZER_CONTEXT_H
#define BUZZER_CONTEXT_H

#include "BuzzerTrack.h"

class BuzzerContext {
  BuzzerTrack* track;

public:
  void process() {
    if (track == nullptr) return;
    if (track->isFinished()) return;
    track->process();
  }

  void setTrack(BuzzerTrack* t) {
    track = t;
    if (t != nullptr)
      track->start();
  }

  bool isBusy() { return track != nullptr && !track->isFinished(); }
  bool isNotBusy() { return track == nullptr || track->isFinished(); }
};

#endif