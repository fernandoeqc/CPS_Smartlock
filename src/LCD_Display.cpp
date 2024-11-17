//
// Created by Filipe Soares on 11/11/24.
//
#include "LCD_Display.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>


LCD_Display::LCD_Display():
    _lcd(0x27, 16, 2) {}

void LCD_Display::turn_on() {
    _lcd.init();
    _lcd.backlight();
}

void LCD_Display::clear() {
    _lcd.clear();
}

void LCD_Display::print_access(const String& access) {
    _lcd.clear();
    _lcd.home();
    _lcd.setCursor(5,0);
    _lcd.print("ACESSO");
    _lcd.setCursor(5,1);
    _lcd.print(access);
}

//byte smile[8] = {
//    B00000,
//    B10001,
//    B00000,
//    B00000,
//    B10001,
//    B01110,
//    B00000,
//    B00000
//  };

//lcd.createChar(0, smile);

