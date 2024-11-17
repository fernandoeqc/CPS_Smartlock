#pragma once
#include <Arduino.h>

class MFRC522controller
{
    private:
    public:
    bool card_detected();
    bool card_registered();
    String get_card_level();
    void card_clear();
    void cmd_register_new_card(String level);
    void rfid_init();
};
