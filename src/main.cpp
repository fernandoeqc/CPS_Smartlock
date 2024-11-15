#include <Arduino.h>
#include <MFRC522.h>
#include "Doorman.h"

#define BUZZER_PIN 33
#define LOCK_PIN 32


Doorman doorman(BUZZER_PIN, LOCK_PIN);

void setup() {
    Serial.begin(115200);
    doorman.init();
}

void loop() {
    for (int i = 0; i < 3; i++) {
        doorman.access(i);
        delay(500);
    }
}
