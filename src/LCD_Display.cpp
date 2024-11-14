//
// Created by Filipe Soares on 11/11/24.
//
#include "LCD_Display.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "buzzer.h"
#include "magnet_lock.h"

#define BUZZER_PIN 33

LCD_Display::LCD_Display():
    _lcd(0x27, 16, 2) {}

Buzzer buzzer(BUZZER_PIN);

void LCD_Display::turn_on() {
    _lcd.init();
    _lcd.backlight();
    buzzer.init();
    lock_init();
}

void LCD_Display::_print_access(const String& access) {
    _lcd.clear();
    _lcd.home();
    _lcd.setCursor(5,0);
    _lcd.print("ACESSO");
    _lcd.setCursor(5,1);
    _lcd.print(access);
}

void LCD_Display::show_access_result(const int result) {
    _lcd.clear();
    if(result==0){
        for (int i=0; i<2; i++) {
            this->_print_access("ACEITO!");
            delay(800);
            _lcd.clear();
            buzzer.twice();
        }
        lock_open();
        this->_print_access("ACEITO!");
        delay(2000);
        lock_close();
        _lcd.clear();

    } else if(result==1){
        for (int i=0; i<2; i++) {
            this->_print_access("ADMIN!");
            delay(800);
            _lcd.clear();
            buzzer.quick();
        }
        lock_open();
        this->_print_access("ADMIN!");
        delay(2000);
        lock_close();
        _lcd.clear();
    }
    else {
        for (int i=0; i<2; i++) {
            this->_print_access("NEGADO!");
            delay(800);
            _lcd.clear();
            buzzer.lengthy();
        }
        this->_print_access("NEGADO!");
        delay(2000);
        _lcd.clear();
    }
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

