#include <Arduino.h>
#include "LCD_Display.h"
#include <MFRC522.h>


void setup() {
    Serial.begin(115200);
    lcd_turn_on();
}

void loop() {
    lcd_show_acces_result(0);
    delay(500);
    lcd_show_acces_result(1);
    delay(500);
    lcd_show_acces_result(2);
    delay(500);
}
