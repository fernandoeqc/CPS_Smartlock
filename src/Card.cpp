//
// Created by Filipe Soares on 17/11/24.
//
#include <Arduino.h>
#include "Card.h"

Card::Card(String id, int level){
  this->_id = id;
  this->_level = level;
}

void Card::give_admin(){
  this->_level = 1;
}

void Card::give_access(){
  this->_level = 0;
}

void Card::remove_access(){
  this->_level = 2;
}

String Card::get_id(){
  return String(this->_id);
}

int Card::get_level(){
  return this->_level;
}