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
        void show_access_result(int result);

    private:
        LiquidCrystal_I2C _lcd;

        void _print_access(const String& access);
};

// Header file example
// #pragma once
//
// #include <Arduino.h>
//
// class LEDControl {
// public:
//     // Constructor
//     LEDControl(int pin);
//
//     // Methods
//     void begin();
//     void on();
//     void off();
//     void toggle();
//
// private:
//     int _pin; // Pin where the LED is connected
//     bool _state; // Current state of the LED (on/off)
// };
