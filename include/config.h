#ifndef CONFIG_H
#define CONFIG_H

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_BUILD 1
#define VERSION String(VERSION_MAJOR) + "." + VERSION_MINOR + "." + VERSION_BUILD

#define LCD_ENABLE_PIN          3
#define LCD_CLK_PIN             5
#define LCD_DATA_PIN            7
#define BUZZER_PIN              12
#define CARD_READER_RX_PIN      21
#define CARD_READER_TX_PIN      19
#define BUILTIN_LED             13
#define WATER_COUNTING_PIN      4
#define BUTTON_PIN              2


// TIME
#define MINUTE                  60000UL
#define SECOND                  1000UL


#define NETWORK_WAIT            3*MINUTE
#define WAIT_TIME               1*MINUTE
#endif
