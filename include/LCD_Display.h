//
// Created by Filipe Soares on 11/11/24.
//
#pragma once

#include <LiquidCrystal_I2C.h>
#include <Arduino.h>


class LCD_Display {
    public:
        explicit LCD_Display();

        void turn_on();
        void clear();
        void print_access(const String& access);

    private:
        LiquidCrystal_I2C _lcd;
};
