#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>
#include <CardReader.h>
#include <LuminiLedRing.h>
#include <Display.h>
#include "config.h"
#include "EspSerial.h"
#include "Buzzer/BuzzerContext.h"
#include "AnimatedLcd.h"

class Device {
  CardReader cardReader;
  LuminiLedRingContext luminiContext;
  LuminiLedRing ledRing;
  AnimatedLcd animatedLcd;
  BuzzerTrack buzzer;
  BuzzerContext buzzerContext;
  EspSerial espSerial;
  static uint8_t versionBuf[CommandLen::GET_VERSION_RESP_LEN + CMD_LEN_DELTA];
  uint8_t listeningCardReaderCmdId = 0xFF;

public:
  Device();
  void setup();
  void loop();

  void cardReaden(const uint8_t* cardUID);
  void espReaden(Packet& p);

  void printRow(uint8_t row, const char* text);
  void printRows(const char* text1, const char* text2, const char* text3);
  void printAnimatedRow(uint8_t row, const char* text, TextAnimation animation, uint16_t duration);
  void controlLedRing(CRGB color, LedRingAnimationEnum animation, uint16_t duration);
  void controlBuzzer(const uint16_t* buzzTones, uint8_t buzzTonesLen, const uint16_t* buzzTimings, uint8_t buzzTimingsLen);
  void listenCardReader(uint8_t id);
  void valveControl();
  int watermeter();
};

extern Device device;

#endif