//
// Created by Filipe Soares on 14/11/24.
//
#pragma once

#include <Arduino.h>
#include "Buzzer.h"
#include "LCD_Display.h"
#include "Magnet_Lock.h"


class Doorman {
    public:
        Doorman(uint8_t buzzer_pin, uint8_t lock_pin);

        void init();
        void access(int access, int time=2000);
        void admin_only_access(int access, int time=2000);
        void close();
        void open();

    private:
        Buzzer _buzzer;
        Magnet_Lock _lock;
        LCD_Display _display;
};

