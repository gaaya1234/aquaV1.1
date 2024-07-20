#ifndef ESP_SERIAL_H
#define ESP_SERIAL_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Packet.h>
#include <FastLED.h>
#include "AnimatedLcd.h"

#define ESP_SERIAL_BAUDRATE 115200
#define PAYLOAD_OFFSET 4
#define PAYLOAD_TAIL 4
#define CMD_ID_INDEX 4
#define CMD_LEN_DELTA 2

#define HEAD1 0xAB
#define HEAD2 0xCD

enum LedRingAnimationEnum {
  OFF,
  ON,
  FADE,
  BLINK,
  CIRCLE,
};

enum Command {
  RESP = 0xAA,
  GET_VERSION = 0x01,
  GET_VERSION_RESP = 0x02,
  PRINT_ROWS = 0x34,
  PRINT_ROW = 0x35,
  PRINT_ANIMATED_ROW = 0x36,
  CONTROL_BARRIER = 0x46,
  CONTROL_LED_RING = 0x57,
  CARD_READER_REQUEST = 0x68,
  CARD_READER_RESPONSE = 0x69,
  CONTROL_BUZZER = 0x70,
};

enum CommandLen {
  RESP_LEN = 0x02,
  GET_VERSION_LEN = 0x01,
  GET_VERSION_RESP_LEN = 0x04,
  CONTROL_BARRIER_LEN = 0x02,
  CONTROL_LED_RING_LEN = 0x07,
  CARD_READER_REQUEST_LEN = 0x01,
  CARD_READER_RESPONSE_LEN = 0x05,
};

typedef void (*EspSerialCallback)(Packet& p);

class EspSerial {
  EspSerialCallback cb;
  void readPacket();
  void sendAck(uint8_t cmdId);
public:
  EspSerial(EspSerialCallback cb) : cb(cb) {}
  void setup();
  void sendPacket(Packet& p);
  void process();

  uint8_t parsePrintRowPacket(Packet& p, uint8_t& row, char* buf);
  uint8_t parsePrintPacket(Packet& p, char* buf1, char* buf2, char* buf3);
  uint8_t parsePrintAnimatedPacket(Packet& p, uint8_t& row, char* buf, TextAnimation& animation, uint16_t& duration);
  uint8_t parseLedRingPacket(Packet& p, CRGB& color, LedRingAnimationEnum& animation, uint16_t& duration);
  uint8_t parseBuzzerPacket(Packet& p, uint16_t* buzzTones, uint8_t& buzzTonesLen, uint16_t* buzzTimings, uint8_t& buzzTimingsLen);
  uint8_t parseBarrierControlPacket(Packet& p, bool& isClose);
};

#endif
