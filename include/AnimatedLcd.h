#ifndef LCD_CONTEXT_H
#define LCD_CONTEXT_H

#include <Arduino.h>
#include <Display.h>
#include "config.h"

enum TextAnimation {
  NONE = 0x05,
  BLINK_TEXT = 0x06,
  TIMED = 0x07,
  RUNNING_LINE = 0x08,
};

class AnimatedLcd {
  const int rowLen[3] = { ROW1_SLOTS, ROW2_SLOTS, ROW3_SLOTS };
  String rows[3];
  String rowsPrev[3];
  TextAnimation a[3] = { NONE, NONE, NONE };

  unsigned long animationStart[3] = { 0,0,0 };
  unsigned int duration[3] = { 0,0,0 };

  Display lcd;

  void shiftPrevString(int i) {
    char prStr[64];
    memcpy(prStr, rows[i].c_str() + 1, rows[i].length() - 1);
    prStr[rows[i].length() - 1] = rows[i][0];
    prStr[rows[i].length()] = '\0';
    rowsPrev[i] = prStr;
  }

public:
  AnimatedLcd() : lcd(LCD_ENABLE_PIN, LCD_CLK_PIN, LCD_DATA_PIN) {
    for (int i = 0; i < 3; ++i) {
      rows[i].reserve(64);
      rowsPrev[i].reserve(64);
    }
  }

  void setup() {
    lcd.setup();
    lcd.cls();
  }

  void printRow(int i, String str, TextAnimation animation = TextAnimation::NONE, int d = 0) {
    a[i] = animation;
    animationStart[i] = millis();
    duration[i] = d;

    if (animation == TextAnimation::BLINK_TEXT) {
      duration[i] = d / 2;
      rowsPrev[i] = "";
      rows[i] = str;
    } else if (animation == TextAnimation::TIMED) {
      rowsPrev[i] = rows[i];
      rows[i] = str;
    } else if (animation == TextAnimation::RUNNING_LINE) {
      rows[i] = str;
      for (int j = 0; j < 6; ++j) rows[i] += " ";
      duration[i] = d / rows[i].length();
      shiftPrevString(i);
    } else if (animation == TextAnimation::NONE) {
      rowsPrev[i] = "";
      rows[i] = str;
    }
    lcd.updateRow(i, str, Align::Right).apply();
  }

  void process() {
    for (int i = 0; i < 3; ++i) {
      if (a[i] == NONE) continue;
      if (millis() - animationStart[i] > duration[i]) {
        animationStart[i] = millis();
        if (a[i] == TextAnimation::BLINK_TEXT) {
          if (rows[i] == "") {
            rows[i] = rowsPrev[i];
            rowsPrev[i] = "";
          } else {
            rowsPrev[i] = rows[i];
            rows[i] = "";
          }
          lcd.updateRow(i, rows[i]).apply();
        } else if (a[i] == TextAnimation::TIMED) {
          rows[i] = rowsPrev[i];
          a[i] = NONE;
          lcd.updateRow(i, rows[i]).apply();
        } else if (a[i] == TextAnimation::RUNNING_LINE) {
          rows[i] = rowsPrev[i];
          shiftPrevString(i);
          lcd.updateRow(i, rows[i]).apply();
        }
      }
    }

    static long int qwe = 0;
    if (millis() - qwe > 2000) {
      qwe = millis();
      lcd.apply();
    }
  }
};

#endif