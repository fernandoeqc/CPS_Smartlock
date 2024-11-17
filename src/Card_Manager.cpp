//
// Created by Filipe Soares on 17/11/24.
//
#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "Card.h"
#include "Card_Manager.h"

#define NUM_OF_CARDS 8


Card_Manager::Card_Manager(uint8_t PIN_SS, uint8_t PIN_RST):
        _mfrc522(PIN_SS,PIN_RST){
  for (int i = 0; i < NUM_OF_CARDS; i++) {
    _card_list[i] = Card();
  }
  _empty_slots = NUM_OF_CARDS;
}

void Card_Manager::init(){
    SPI.begin();
    _mfrc522.PCD_Init();
}

void Card_Manager::listen(){
  Serial.println("Card Listening");
  while(true){
    if (_mfrc522.PICC_IsNewCardPresent()) {
      Serial.println("Card found");
    }

    if (_mfrc522.PICC_ReadCardSerial()) {
      Serial.println("Card red");
      return;
    }

    delay(10);
  }
}

String Card_Manager::read(){
  String id = "";
  for (byte i = 0; i < _mfrc522.uid.size; i++) {
    id.concat(String(_mfrc522.uid.uidByte[i] < HEX ? " 0" : " "));
    id.concat(String(_mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println(id.substring(1));
  return String(id.substring(1));
}

bool Card_Manager::is_card_registred(String id){
  for (int i = 0; i < NUM_OF_CARDS; i++) {
    if (id == _card_list[i].get_id()) {
      return true;
    }
  }
  return false;
}

bool Card_Manager::is_list_full(){
  if (_empty_slots == 0) {
    return true;
  }
  return false;
}

int Card_Manager::empty_slots(){
  return _empty_slots;
}

void Card_Manager::append(Card card){
  if (this->is_card_registred(card.get_id())) {
    Serial.println("Card already registered");
    return;
  } else if (this->is_list_full()) {
    Serial.println("Card list already full");
    return;
  }
  for (int i = 0; i < NUM_OF_CARDS; i++) {
    if (_card_list[i].get_id() == "None") {
      _card_list[i] = card;
      _empty_slots--;
      Serial.println(String("Registered the card id: ") + String(_card_list[i].get_id()) );
      return;
    }
  }
}

void Card_Manager::remove(Card card){
  if (this->is_card_registred(card.get_id())) {
    for (int i = 0; i < NUM_OF_CARDS; i++) {
      if (_card_list[i].get_id() == card.get_id()) {
        _card_list[i] = Card();
        _empty_slots++;
        return;
      }
    }
  }
  Serial.println("Card not found");
  return;
}

void Card_Manager::upgrade(String id) {
  for (int i = 0; i < NUM_OF_CARDS; i++) {
    if (_card_list[i].get_id() == id) {
      int card_level = _card_list[i].get_level();

      switch (card_level) {
        case 0:
          _card_list[i].give_admin();
          break;

        case 1:
          break;

        default:
          _card_list[i].give_access();
          break;
      }
      return;
    }
  }
}

void Card_Manager::downgrade(String id) {
  for (int i = 0; i < NUM_OF_CARDS; i++) {
    if (_card_list[i].get_id() == id) {
      int card_level = _card_list[i].get_level();

      switch (card_level) {
        case 0:
          _card_list[i].remove_access();
        break;

        case 1:
          _card_list[i].give_access();
          break;

        default:
          break;
      }
      return;
    }
  }
}

Card* Card_Manager::get(String id) {
  for (int i = 0; i < NUM_OF_CARDS; i++) {
    if (_card_list[i].get_id() == id) {
      return &(_card_list[i]);
    }
  }
  return nullptr;
}
