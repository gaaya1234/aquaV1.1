# Lumini Led Ring 1 inch

## Usage

```cpp
#define LUMINI_DATA_PIN 18
#define LUMINI_CLK_PIN 19
#include <Arduino.h>
#include "LuminiLedRing.h"
#include "LuminiLedRingContext.h"

LuminiLedRingContext context;
LuminiLedRing luminiLedRing;

void setup() {
  Serial.begin(115200);
  Serial.println("READY!");
  luminiLedRing.setup(&context);

  luminiLedRing.load(CRGB::HTMLColorCode::Aquamarine, 7000, 5);
}

void loop() {
  int lSt = millis();
  int lEnd = millis();
  if (lEnd - lSt > 2) {
    Serial.println((String)"SLOW FRAME: " + (lEnd - lSt));
  }
  context.loop();
}
```

Note: You can also define your serial pins in platformio.ini:

```ini
build_flags =
    -D LUMINI_DATA_PIN=18
    -D LUMINI_CLK_PIN=19
```
