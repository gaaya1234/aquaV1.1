#ifndef BUZZER_TRACK_H
#define BUZZER_TRACK_H

#include <Arduino.h>
#include "config.h"

class BuzzerTrack {
protected:
  uint16_t temp[128];
  uint16_t freq[128];
  int64_t startTime;
  int it = 0;
  int tempLen = 0;
  int freqLen = 0;
  bool finished = true;

public:
  bool looped;
  void process() {
    if (finished) return;
    if (millis() - startTime > int(temp[it])) {
      ++it;
      if (it >= tempLen) {
        if (!looped) {
          noTone(BUZZER_PIN);
          finished = true;
          return;
        }
        it = 0;
      }
      if (it & 1) noTone(BUZZER_PIN);
      else tone(BUZZER_PIN, freq[min(it >> 1, freqLen - 1)]);
      startTime = millis();
    }
  };

  void start() {
    finished = false;
    startTime = millis();
    it = 0;
    tone(BUZZER_PIN, freq[it]);
  }

  inline bool isFinished() { return finished; }

  void setTrack(const uint16_t* trackBuf, int trackBufSize, const uint16_t* freqBuf, int freqBufSize) {
    memcpy(temp, trackBuf, sizeof(int16_t) * trackBufSize);
    memcpy(freq, freqBuf, sizeof(int16_t) * freqBufSize);
    tempLen = trackBufSize;
    freqLen = freqBufSize;
  }
};

#endif