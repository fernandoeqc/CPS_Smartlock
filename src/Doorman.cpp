//
// Created by Filipe Soares on 14/11/24.
//
#include <Arduino.h>
#include "Buzzer.h"
#include "LCD_Display.h"
#include "Magnet_Lock.h"
#include "Doorman.h"

#define INNER_DELAY 800


Doorman::Doorman(uint8_t buzzer_pin, uint8_t lock_pin):
     _buzzer(buzzer_pin),
     _lock(lock_pin),
     _display() {
      this->is_door_open = false;
}

void Doorman::init(){
  _buzzer.init();
  _lock.init();
  _display.turn_on();
}

void Doorman::open(){
  this->is_door_open = true;
  this->_lock.open();
}

void Doorman::close(){
  this->is_door_open = false;
  this->_lock.close();
}

void Doorman::listen_lock(int time) {
  int initial = 0;
  while(initial < time) {
    if (this->is_door_open == false) {
      return;
    }
    delay(50);
    initial += 50;
  }
}

void Doorman::access(int access, int time){
  if (access == 0){
    for (int i=0; i < 2; i++){
      this->_display.print_access("ACEITO!");
      delay(INNER_DELAY);
      this->_display.clear();
      this->_buzzer.twice();
    }
    this->_display.print_access("ACEITO!");
    this->open();
    this->listen_lock(time);
    this->close();
    Serial.println("Doorman closed");

  } else if (access == 1){
    for (int i=0; i < 2; i++){
      this->_display.print_access("ADMIN!");
      delay(INNER_DELAY);
      this->_display.clear();
      this->_buzzer.quick();
    }
    this->_display.print_access("ADMIN!");
    this->open();
    this->listen_lock(time);
    this->close();
    Serial.println("Doorman closed");

  } else {
    for (int i=0; i < 2; i++){
      this->_display.print_access("NEGADO!");
      delay(INNER_DELAY);
      this->_display.clear();
      this->_buzzer.lengthy();
    }
    this->_display.print_access("NEGADO!");
  }

  this->_display.clear();
  Serial.println("Doorman display clear");
}

void Doorman::admin_only_access(int access, int time){
if (access == 1){
    for (int i=0; i < 2; i++){
      this->_display.print_access("ADMIN!");
      delay(INNER_DELAY);
      this->_display.clear();
      this->_buzzer.quick();
    }
    this->_display.print_access("ADMIN!");
    this->open();
    this->listen_lock(time);
    this->close();
    Serial.println("Doorman closed");

  } else {
    for (int i=0; i < 2; i++){
      this->_display.print_access("NEGADO!");
      delay(INNER_DELAY);
      this->_display.clear();
      this->_buzzer.lengthy();
    }
    this->_display.print_access("NEGADO!");
  }

  this->_display.clear();
  Serial.println("Doorman display clear");
}