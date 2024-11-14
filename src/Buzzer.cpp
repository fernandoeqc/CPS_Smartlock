//
// Created by Filipe Soares on 11/11/24.
//
#include "Buzzer.h"
#include <Arduino.h>

Buzzer::Buzzer(const uint8_t pin) {
  _pin = pin;
}

void Buzzer::init() {
  pinMode(_pin, OUTPUT);
}

void Buzzer::quick() {
  digitalWrite(_pin, HIGH);
  delay(50);
  digitalWrite(_pin, LOW);
  delay(50);
}

void Buzzer::lengthy() {
  digitalWrite(_pin, HIGH);
  delay(400);
  digitalWrite(_pin, LOW);
  delay(50);
}

void Buzzer::twice() {
  for (int i=0; i<2; i++) {
    this->quick();
  }
}