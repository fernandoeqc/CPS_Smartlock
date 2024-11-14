//
// Created by Filipe Soares on 11/11/24.
//
#pragma once
#include <Arduino.h>

class Buzzer {
    public:
        explicit Buzzer(uint8_t pin);

        void init();
        void quick();
        void lengthy();
        void twice();

    private:
        uint8_t _pin;
};

void buzzer_init();

void buzzer_short();

void buzzer_long();

void buzzer_double();