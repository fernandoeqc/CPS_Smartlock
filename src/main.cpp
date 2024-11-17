#include <Arduino.h>
#include "Doorman.h"
#include "Card_Manager.h"

#define BUZZER_PIN 33
#define LOCK_PIN 32
#define RST_PIN 15
#define SS_PIN 5


String white_card_id = String("bd 83 8a 21");
String key_chain_id = String("7a 6e 2d a4");

Card white_card = Card(white_card_id, 1);
Card key_chain = Card(key_chain_id, 0);

Doorman doorman(BUZZER_PIN, LOCK_PIN);

Card_Manager cm(SS_PIN, RST_PIN);

void setup() {
    Serial.begin(115200);
    doorman.init();
    cm.init();
    cm.append(white_card);
    cm.append(key_chain);
}

void loop() {
    cm.listen();
    String card_id = cm.read();

    int card_level = 2;

    if (cm.is_card_registred(card_id)) {
        Card* card = cm.get(card_id);
        card_level = card->get_level();
    }

    Serial.println(card_level);

    doorman.access(card_level);

    delay(200);
}
