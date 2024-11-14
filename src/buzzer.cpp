//
// Created by Filipe Soares on 11/11/24.
//
#include "buzzer.h"
#include <Arduino.h>

#define BUZZER_PIN 33

void buzzer_init() {
  pinMode(BUZZER_PIN, OUTPUT);
}

void buzzer_short() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(50);
  digitalWrite(BUZZER_PIN, LOW);
  delay(50);
};

void buzzer_long() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(400);
  digitalWrite(BUZZER_PIN, LOW);
  delay(50);
};

void buzzer_double() {
  for (int i=0; i<2; i++) {
    buzzer_short();
  }
}