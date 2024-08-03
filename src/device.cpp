#include "device.h"

Device device;


Device::Device() :
  cardReader(CARD_READER_RX_PIN, CARD_READER_TX_PIN, [](const uint8_t* cardUID) {device.cardReaden(cardUID);}, 50, 150) {
}
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

}
void Device::printRow(uint8_t row, const char* text) {
  Serial.println((String)"[printRow] row = " + (int)row + ", text = \"" + text + "\"");
  animatedLcd.printRow(row, text, TextAnimation::NONE);
}


void Device::printRows(const char* text1, const char* text2, const char* text3) {
  animatedLcd.printRow(0, text1, TextAnimation::NONE);
  animatedLcd.printRow(1, text2, TextAnimation::NONE);
  animatedLcd.printRow(2, text3, TextAnimation::NONE);
}


void Device::printAnimatedRow(uint8_t row, const char* text, TextAnimation animation, uint16_t duration) {
  Serial.println((String)"[printAnimatedRow] row = " + (int)row + ", text = " + text + ", animation = " + (unsigned int)animation + ", duration = " + duration);
  animatedLcd.printRow(row, text, animation, duration);
}


void Device::controlBuzzer(const uint16_t* buzzTones, uint8_t buzzTonesLen, const uint16_t* buzzTimings, uint8_t buzzTimingsLen) {
  buzzer.setTrack(buzzTimings, int(buzzTimingsLen), buzzTones, int(buzzTonesLen));
  buzzer.start();
}


void Device::listenCardReader(uint8_t id) {
  listeningCardReaderCmdId = id;
}


void Device::setup() {
  animatedLcd.setup();
  // cardReader.setup();
  buzzerContext.setTrack(&buzzer);
}

void Device::loop() {
  buzzerContext.process();
  if (buzzerContext.isNotBusy() && listeningCardReaderCmdId != 0xFF) cardReader.process();
  animatedLcd.process();
}