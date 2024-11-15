//
// Created by Filipe Soares on 11/11/24.
//
#pragma once

#include <Arduino.h>


class Magnet_Lock {
    public:
        explicit Magnet_Lock(uint8_t pin);

        void init();
        void open();
        void close();

    private:
        uint8_t _pin;
};
