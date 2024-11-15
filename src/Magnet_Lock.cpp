//
// Created by Filipe Soares on 11/11/24.
//
#include "Magnet_Lock.h"
#include <Arduino.h>

Magnet_Lock::Magnet_Lock(uint8_t pin) {
    _pin = pin;
}

void Magnet_Lock::init() {
    pinMode(_pin, OUTPUT);
}

void Magnet_Lock::open() {
    digitalWrite(_pin, HIGH);
}

void Magnet_Lock::close() {
    digitalWrite(_pin, LOW);
}