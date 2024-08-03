#include "device.h"
#include <Arduino.h>

// AnimatedLcd lcd;
// LuminiLedRing ledRing;
// LuminiLedRingContext context;
// CardReader cardReader(CARD_READER_RX_PIN, CARD_READER_TX_PIN, [](const uint8_t* card) {
//   Serial.print("CARD UID: ");
//   Serial.print(card[0], HEX);
//   Serial.print(card[1], HEX);
//   Serial.print(card[2], HEX);
//   Serial.print(card[3], HEX);
//   Serial.println();
//   });

void setup() {
  Serial.begin(115200);
  device.setup();
  delay(200);
  Serial.println("START lilygo");

  // cardReader.setup();
  // ledRing.setup(&context);
  // ledRing.enable(CRGB::HTMLColorCode::Violet);
  // lcd.setup();
  // lcd.printRow(0, "43217123123asdqwe", TextAnimation::RUNNING_LINE, 10000);
}

void loop() {
  uint64_t timeLoop = millis();

  // device.loop();
  int sf = millis() - timeLoop;
  if (sf > 10) {
    Serial.print("SLOW FRAME: ");
    Serial.println(sf);
  }
}
