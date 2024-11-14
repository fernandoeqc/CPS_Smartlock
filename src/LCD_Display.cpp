//
// Created by Filipe Soares on 11/11/24.
//

#include "LCD_Display.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "buzzer.h"
#include "magnet_lock.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);

void lcd_print_access(String access) {
    lcd.clear();
    lcd.home();
    lcd.setCursor(5,0);
    lcd.print("ACESSO");
    lcd.setCursor(5,1);
    lcd.print(access);
}

void lcd_turn_on(){
    lcd.init();
    lcd.backlight();
    buzzer_init();
    lock_init();
}

void lcd_show_acces_result(int result){
    lcd.clear();
    if(result==0){
        for (int i=0; i<2; i++) {
            lcd_print_access("ACEITO!");
            delay(800);
            lcd.clear();
            buzzer_double();
        }
        lock_open();
        lcd_print_access("ACEITO!");
        delay(2000);
        lock_close();
        lcd.clear();

    } else if(result==1){
        for (int i=0; i<2; i++) {
            lcd_print_access("ADMIN!");
            delay(800);
            lcd.clear();
            buzzer_short();
        }
        lcd_print_access("ADMIN!");
        delay(2000);
        lcd.clear();
    }
    else {
        for (int i=0; i<2; i++) {
            lcd_print_access("NEGADO!");
            delay(800);
            lcd.clear();
            buzzer_long();
        }
        lcd_print_access("NEGADO!");
        delay(2000);
        lcd.clear();
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

