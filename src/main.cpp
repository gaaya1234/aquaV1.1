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
  Serial1.begin(115200);
  device.setup();
  delay(200);
  Serial.println("START MEGA");

  // cardReader.setup();
  // ledRing.setup(&context);
  // ledRing.enable(CRGB::HTMLColorCode::Violet);
  // lcd.setup();
  // lcd.printRow(0, "43217123123asdqwe", TextAnimation::RUNNING_LINE, 10000);
}

void loop() {
  uint64_t timeLoop = millis();

  device.loop();
  int sf = millis() - timeLoop;
  if (sf > 10) {
    Serial.print("SLOW FRAME: ");
    Serial.println(sf);
  }

  // static unsigned long t = 0;
  // static bool sw = true;
  // if (millis() - t > 40000) {
  //   t = millis();
  //   sw = !sw;

  //   if (sw)
  //     lcd.printRow(0, "1234");
  //   else
  //     lcd.printRow(0, "43217123123asdqwe", TextAnimation::RUNNING_LINE, 10000);
  // }

  // lcd.process();

  // int bb = Serial1.read();
  // if (bb == -1) return;
  // Serial.println(bb, HEX);

  // context.process();
  // static unsigned long t = 0;
  // static bool sw = false;
  // if (millis() - t > 5000) {
  //   Serial.println("LedRing");
  //   sw = !sw;
  //   t = millis();
  //   if (sw) {
  //     ledRing.enable(CRGB::HTMLColorCode::Aqua);
  //   } else {
  //     ledRing.enable(CRGB::HTMLColorCode::Violet);
  //   }
  // }

  // cardReader.process();



  // int read = Serial1.read();
  // if (read == -1) return;
  // Serial.print("READ ");
  // Serial.println(read, HEX);
  // uint8_t buf[128];
  // int aa = Serial1.readBytes(buf, Serial1.available());
  // Serial1.flush();
  // Serial.println(aa);

  // // for (int i = 0; i < aa; ++i) {
  // //   Serial.print(" ");
  // //   Serial.print(buf[i], HEX);
  // // }
  // Serial.println();
}
