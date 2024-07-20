#ifndef CONFIG_H
#define CONFIG_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_BUILD 1
#define VERSION String(VERSION_MAJOR) + "." + VERSION_MINOR + "." + VERSION_BUILD

#define LCD_ENABLE_PIN     3
#define LCD_CLK_PIN        5
#define LCD_DATA_PIN       7
#define BUZZER_PIN         12
#define CARD_READER_TX_PIN 52
#define CARD_READER_RX_PIN 53
#define BUILTIN_LED        13

// TIME
#define ChargingThresholdTime   60000
#define VehicleConnectionTime   60000
#define MINUTE                  60000UL
#define SECOND                  1000UL


#ifndef K_DEBUG
#define NETWORK_WAIT                 180000UL // 3 min.
#define BARRIER_PARKING_WAIT_TIME    120000UL // 2 min.
#else
#define BARRIER_PARKING_WAIT_TIME    3000UL  
#define NETWORK_WAIT                 180000UL
#endif

#endif
