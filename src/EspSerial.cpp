#include "EspSerial.h"

void EspSerial::setup() {
  ESP_HARDWARE_SERIAL.begin(ESP_SERIAL_BAUDRATE);
  ESP_HARDWARE_SERIAL.setTimeout(5);
}


void EspSerial::process() {
  readPacket();
}


void EspSerial::readPacket() {
  static const int MAX_PACKET_LEN = 64;
  static uint8_t buf[MAX_PACKET_LEN];
  static uint8_t bufIndex = 0;

  int read = -1;
  if ((read = ESP_HARDWARE_SERIAL.read()) == -1) { return; }
  buf[bufIndex] = read;
  // Serial.println('[' + String(bufIndex) + "] " + String(buf[bufIndex], HEX) + " " + String(buf[bufIndex]));
  ++bufIndex;

  if (bufIndex <= 4) {
    switch (bufIndex) {
    case 1:
      if (buf[bufIndex - 1] == HEAD1) return;
      bufIndex = 0;
      return;
    case 2:
      if (buf[bufIndex - 1] == HEAD2) return;
      bufIndex = 0;
      return;
    case 3:
      return;
    case 4:
      if (buf[bufIndex - 1] > MAX_PACKET_LEN - 5) {
        bufIndex = 0;
        return;
      }
      return;
    default: break;
    }
    return;
  }
  if (bufIndex < buf[3] + PAYLOAD_TAIL + PAYLOAD_OFFSET) return;

  Packet p;
  p.insertPacket(buf, bufIndex);
  Serial.print("From esp: ");
  for (int i = 0; i < bufIndex; ++i) {
    char bb[8];
    sprintf(bb, " %02X", buf[i]);
    Serial.print(bb);
  }
  Serial.print(" ----> ");
  Serial.println(String("isValid: ") + String(p.isValid()));

  bufIndex = 0;
  if (!p.isValid()) return;
  if (buf[2] != Command::CARD_READER_REQUEST && buf[2] != Command::GET_VERSION)
    sendAck(buf[CMD_ID_INDEX]);

  if (cb != nullptr) cb(p);
  return;
}


void EspSerial::sendAck(uint8_t cmdId) {
  uint8_t buf[16] = { Command::RESP, CommandLen::RESP_LEN, cmdId };
  Packet p(buf, CommandLen::RESP_LEN + CMD_LEN_DELTA);

  sendPacket(p);
}


void EspSerial::sendPacket(Packet& p) {
  Serial.print("[sendPacket] To ESP: ");
  p.print();
  ESP_HARDWARE_SERIAL.write(p.getData(), p.size());
}


uint8_t EspSerial::parsePrintRowPacket(Packet& p, uint8_t& row, char* buf) {
  row = p[5];
  memcpy(buf, p.getData() + 6, p[3] - 2);
  return p[CMD_ID_INDEX];
}


uint8_t EspSerial::parsePrintPacket(Packet& p, char* buf1, char* buf2, char* buf3) {
  int l1 = p[5];
  int l2 = p[6 + l1];
  int l3 = p[7 + l1 + l2];

  memcpy(buf1, p.getData() + 6, l1);
  memcpy(buf2, p.getData() + 7 + l1, l2);
  memcpy(buf3, p.getData() + 8 + l1 + l2, l3);
  buf1[l1] = buf2[l2] = buf3[l3] = '\0';

  Serial.println((String)"[parsePrintPacket] l1 = " + l1 + ", l2 = " + l2 + ", l3 = " + l3);
  Serial.println((String)"[parsePrintPacket] buf1 = " + buf1 + ", buf2 = " + buf2 + ", buf3 = " + buf3);
  return p[CMD_ID_INDEX];
}


uint8_t EspSerial::parsePrintAnimatedPacket(Packet& p, uint8_t& row, char* buf, TextAnimation& animation, uint16_t& duration) {
  row = p[5];
  memcpy(buf, p.getData() + 6, p[3] - 5);
  animation = TextAnimation(p[p[3] + 1]);
  duration = makeWord(p[p[3] + 2], p[p[3] + 3]);
  return p[CMD_ID_INDEX];
}

uint8_t EspSerial::parseLedRingPacket(Packet& p, CRGB& color, LedRingAnimationEnum& animation, uint16_t& duration) {
  color.r = p[5];
  color.g = p[6];
  color.b = p[7];
  animation = LedRingAnimationEnum(p[8]);
  duration = makeWord(p[9], p[10]);
  return p[CMD_ID_INDEX];
}


uint8_t EspSerial::parseBuzzerPacket(Packet& p, uint16_t* buzzTones, uint8_t& buzzTonesLen, uint16_t* buzzTimings, uint8_t& buzzTimingsLen) {
  buzzTonesLen = p[5] / 2;
  memcpy(buzzTones, p.getData() + 6, p[5]);
  buzzTimingsLen = p[6 + p[5]] / 2;
  memcpy(buzzTimings, p.getData() + 7 + p[5], p[6 + p[5]]);
  return p[CMD_ID_INDEX];
}

uint8_t EspSerial::parseBarrierControlPacket(Packet& p, bool& isClose) {
  isClose = p[5] == 0x00;
  return p[CMD_ID_INDEX];
}

