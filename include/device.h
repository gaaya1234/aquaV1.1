#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>
#include <CardReader.h>
#include <Display.h>
#include "config.h"
#include "Buzzer/BuzzerContext.h"
#include "AnimatedLcd.h"

class Transaction{
  public: 
    //well internal variables
    uint16_t standbyDurationTime = WAIT_TIME;
    uint16_t waterTankLevel = 0;
    uint64_t accumlatedWaterLitre = 0;

    //variables in each transaction
    uint8_t transWaterType; //0 means cold, 1 means hot
    uint16_t transHotWaterUnitPrice = 0;
    uint16_t transColdWaterUnitPrice = 0;
    uint8_t transCardUID[4]={0};
    uint32_t transCardBalance = 0;
    uint16_t transWaterMeter = 0;
    uint16_t transOldWaterMeter = 0;
    uint16_t transWaterLitre = 0;
    uint32_t transWaterAmount = 0;
    uint32_t transCounterNumber = 0;
    bool cardReadStatus = false;
    bool transStarted = false;

    //Transaction functions
    bool funcReadSerial(SoftwareSerial &DataSerial);
    uint8_t *funcSendCardReqCmd();     //checked
    uint8_t *funcSendTransInfoCmd();    //Checked
    void funcGetSensorData();

    //Card Reader functions ISO14443A communication
    byte *funcCardReadCmd(SoftwareSerial &CardSerial); //Card read cmd
    bool funcGetByteArray(SoftwareSerial &CardSerial);
    bool funcIsCardReaden(SoftwareSerial &CardSerial);
};


class Device {
  CardReader cardReader;
  AnimatedLcd animatedLcd;
  BuzzerTrack buzzer;
  BuzzerContext buzzerContext;
  uint8_t listeningCardReaderCmdId = 0xFF;

public:
  Device();
  void setup();
  void loop();

  void cardReaden(const uint8_t* cardUID);

  void printRow(uint8_t row, const char* text);
  void printRows(const char* text1, const char* text2, const char* text3);
  void printAnimatedRow(uint8_t row, const char* text, TextAnimation animation, uint16_t duration);
  void controlBuzzer(const uint16_t* buzzTones, uint8_t buzzTonesLen, const uint16_t* buzzTimings, uint8_t buzzTimingsLen);
  void listenCardReader(uint8_t id);
  void valveControl();
  int watermeter();
};

extern Device device;

#endif