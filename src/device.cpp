#include "device.h"

Device device;

uint8_t Device::versionBuf[CommandLen::GET_VERSION_RESP_LEN + CMD_LEN_DELTA] = { Command::GET_VERSION_RESP, CommandLen::GET_VERSION_RESP_LEN, 0, uint8_t(VERSION_MAJOR), uint8_t(VERSION_MINOR), uint8_t(VERSION_BUILD) };

Device::Device() :
  cardReader(CARD_READER_RX_PIN, CARD_READER_TX_PIN, [](const uint8_t* cardUID) {device.cardReaden(cardUID);}, 50, 150),
  espSerial([](Packet& p) {device.espReaden(p);}) {
}

// void Device::espReaden(Packet& p) {
//   Command cmd = Command(p[2]);

//   switch (cmd) {
//   case Command::PRINT_ROW:
//   {
//     uint8_t row;
//     char buf[128] = "";
//     espSerial.parsePrintRowPacket(p, row, buf);
//     printRow(row, buf);
//   }
//   break;
//   case Command::PRINT_ROWS:
//   {
//     char b1[128], b2[128], b3[128];
//     espSerial.parsePrintPacket(p, b1, b2, b3);
//     printRows(b1, b2, b3);
//   }
//   break;
//   case Command::PRINT_ANIMATED_ROW:
//   {
//     uint8_t row;
//     char buf[128];
//     TextAnimation a;
//     uint16_t d;
//     espSerial.parsePrintAnimatedPacket(p, row, buf, a, d);
//     printAnimatedRow(row, buf, a, d);
//   }
//   break;
//   case Command::GET_VERSION:
//   {
//     versionBuf[CMD_ID_INDEX - CMD_LEN_DELTA] = p[CMD_ID_INDEX];
//     Packet vp(versionBuf, CommandLen::GET_VERSION_RESP_LEN + CMD_LEN_DELTA);
//     espSerial.sendPacket(vp);
//   }
//   break;
//   case Command::CARD_READER_REQUEST:
//     listenCardReader(p[CMD_ID_INDEX]);
//     break;
//   case Command::CONTROL_BARRIER:
//   {
//     bool isClose;
//     espSerial.parseBarrierControlPacket(p, isClose);
//     if (isClose) barrier.close();
//     else barrier.open();
//   }
//   break;
//   case Command::CONTROL_BUZZER:
//   {
//     uint16_t freqs[128];
//     uint8_t freqsLen;
//     uint16_t timings[128];
//     uint8_t timingsLen;

//     espSerial.parseBuzzerPacket(p, freqs, freqsLen, timings, timingsLen);

//     Serial.print((String)"[CONTROL BUZZER] freqLen = " + (int)freqsLen + ", freqs = [ ");

//     for (int i = 0; i < freqsLen; ++i) {
//       Serial.print(freqs[i]);
//       Serial.print(" ");
//     }

//     Serial.print((String)"], timingsLen = " + (int)timingsLen + ", timings = [ ");

//     for (int i = 0; i < timingsLen; ++i) {
//       Serial.print(timings[i]);
//       Serial.print(" ");
//     }

//     Serial.println("]");

//     controlBuzzer(freqs, freqsLen, timings, timingsLen);
//   }
//   break;
//   case Command::CONTROL_LED_RING:
//   {
//     CRGB color;
//     LedRingAnimationEnum a;
//     uint16_t d;
//     espSerial.parseLedRingPacket(p, color, a, d);

//     Serial.println((String)"[CONTROL LED RING] color: [" + color.r + ", " + color.g + ", " + color.b + "], duration = " + d);
//     controlLedRing(color, a, d);
//   }
//   break;
//   case Command::RESP: break;
//   case Command::CARD_READER_RESPONSE: break;
//   case Command::GET_VERSION_RESP: break;
//   default:
//     break;
//   }
// }


void Device::cardReaden(const uint8_t* cardUID) {
  static unsigned long cardReadTime = 0;
  static uint8_t cardUIDCached[4] = { 0,0,0,0 };

  if (memcmp(cardUIDCached, cardUID, 4)) {
    memcpy(cardUIDCached, cardUID, 4);
    cardReadTime = millis();
    return;
  }

  if (millis() - cardReadTime > 1000) {
    memset(cardUIDCached, 0, 4);
    return;
  }

  Serial.print((String)"[cardReaden] ");
  for (int i = 0; i < 4; ++i) {
    Serial.print(" ");
    Serial.print(cardUID[i], HEX);
  }

  Serial.println();

  uint8_t buf[Command::CARD_READER_RESPONSE + CMD_LEN_DELTA] = {
    Command::CARD_READER_RESPONSE,
    CommandLen::CARD_READER_RESPONSE_LEN,
    listeningCardReaderCmdId,
    cardUID[0],
    cardUID[1],
    cardUID[2],
    cardUID[3],
  };

  Packet p(buf, CommandLen::CARD_READER_RESPONSE_LEN + CMD_LEN_DELTA);
  listeningCardReaderCmdId = 0xFF;
  espSerial.sendPacket(p);
}



void Device::printRow(uint8_t row, const char* text) {
  Serial.println((String)"[printRow] row = " + (int)row + ", text = \"" + text + "\"");
  animatedLcd.printRow(row, text, TextAnimation::NONE);
}


void Device::printRows(const char* text1, const char* text2, const char* text3) {
  animatedLcd.printRows(text1, text2, text3);
}


void Device::printAnimatedRow(uint8_t row, const char* text, TextAnimation animation, uint16_t duration) {
  Serial.println((String)"[printAnimatedRow] row = " + (int)row + ", text = " + text + ", animation = " + (unsigned int)animation + ", duration = " + duration);
  animatedLcd.printRow(row, text, animation, duration);
}


void Device::controlLedRing(CRGB color, LedRingAnimationEnum animation, uint16_t duration) {
  switch (animation) {
  case LedRingAnimationEnum::OFF:
    ledRing.enable(CRGB::HTMLColorCode::Black);
    break;
  case LedRingAnimationEnum::ON:
    ledRing.enable(color);
    break;
  case LedRingAnimationEnum::BLINK:
    ledRing.enable(color);
    ledRing.blink(duration);
    break;
  case LedRingAnimationEnum::CIRCLE:
    ledRing.load(color, duration);
    break;
  case LedRingAnimationEnum::FADE:
    ledRing.enable(color);
    ledRing.fade(duration);
    break;
  default:
    break;
  }
}


void Device::controlBuzzer(const uint16_t* buzzTones, uint8_t buzzTonesLen, const uint16_t* buzzTimings, uint8_t buzzTimingsLen) {
  buzzer.setTrack(buzzTimings, int(buzzTimingsLen), buzzTones, int(buzzTonesLen));
  buzzer.start();
}


void Device::listenCardReader(uint8_t id) {
  listeningCardReaderCmdId = id;
}


void Device::setup() {
  ledRing.setup(&luminiContext);
  animatedLcd.setup();
  cardReader.setup();
  buzzerContext.setTrack(&buzzer);
}

void Device::loop() {
  // static unsigned long tt = 0;
  // if (millis() - tt > 1000) {
  //   tt = millis();
  //   Serial.println((String)"[LOOP] listeningCardReaderCmdId = " + (int)listeningCardReaderCmdId);
  // }
  buzzerContext.process();
  if (buzzerContext.isNotBusy() && listeningCardReaderCmdId != 0xFF) cardReader.process();
  luminiContext.process();
  animatedLcd.process();
  espSerial.process();
}