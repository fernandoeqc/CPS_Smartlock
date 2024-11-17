//
// Created by Filipe Soares on 17/11/24.
//
#pragma once

#include <Arduino.h>


class Card {
  public:
    Card(String id="None", int level=2);

    void give_admin();
    void give_access();
    void remove_access();

    String get_id();
    int get_level();

  private:
    String _id;
    int _level;
};