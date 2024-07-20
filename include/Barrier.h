#ifndef BARRIER_H
#define BARRIER_H

#include <Arduino.h>

typedef void (*BarrierCB)();

class Barrier {
  static const uint8_t openCmdLen = 1;
  static const uint8_t openCmd[openCmdLen];
  static const uint8_t openRespLen = 1;
  static const uint8_t openResp[openRespLen];

  static const uint8_t closeCmdLen = 1;
  static const uint8_t closeCmd[closeCmdLen];
  static const uint8_t closeRespLen = 1;
  static const uint8_t closeResp[closeRespLen];

  static const uint8_t getStatusCmdLen = 1;
  static const uint8_t getStatusCmd[getStatusCmdLen];

  static const uint8_t statusRespLen = 1;
  static const uint8_t statusOpenedNoCarResp[statusRespLen];
  static const uint8_t statusOpenedWithCarResp[statusRespLen];
  static const uint8_t statusCloseddNoCarResp[statusRespLen];
  static const uint8_t statusCloseddWithCarResp[statusRespLen];

public:
  void setup();
  void process();

  void open();
  void close();

  // void open(BarrierCB onSuccess);
  // void close(BarrierCB onSuccess);
};

#endif