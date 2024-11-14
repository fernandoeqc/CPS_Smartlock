//
// Created by Filipe Soares on 11/11/24.
//
#include "magnet_lock.h"
#include <Arduino.h>

#define LOCK 32

void lock_init(){
    pinMode(LOCK, OUTPUT);
};

void lock_open(){
    digitalWrite(LOCK, HIGH);
};

void lock_close(){
    digitalWrite(LOCK, LOW);
};