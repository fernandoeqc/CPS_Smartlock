#include <Arduino.h>
#include "LCD_Display.h"
#include <MFRC522.h>

LCD_Display display;
void setup() {
    Serial.begin(115200);
    display.turn_on();
}

void loop() {
    display.show_access_result(0);
    delay(500);
    display.show_access_result(1);
    delay(500);
    display.show_access_result(2);
    delay(500);
}
