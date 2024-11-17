//
// Created by Filipe Soares on 17/11/24.
//
#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include "Card.h"

#define NUM_OF_CARDS 8


class Card_Manager {
  public:
    Card_Manager(uint8_t PIN_SS, uint8_t PIN_RST);

    void init();
    void listen();
    String read();
    bool is_card_registred(String id);
    bool is_list_full();
    int empty_slots();
    void append(Card card);
    void remove(Card card);
    void upgrade(String id);
    void downgrade(String id);
    Card* get(String id);

  private:
    Card _card_list[NUM_OF_CARDS];
    MFRC522 _mfrc522;
    int _empty_slots;
};
