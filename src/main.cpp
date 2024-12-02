#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Doorman.h"
#include "Card_Manager.h"
#include "MQTT_Menssager.h"

#define BUZZER_PIN 33
#define LOCK_PIN 32
#define RST_PIN 15
#define SS_PIN 5
#define NEW_CARD_PIN 12
#define MAGNET_PIN 14

String white_card_id = String("bd 83 8a 21");
String key_chain_id = String("7a 6e 2d a4");

Card white_card = Card(white_card_id, 1);
Card key_chain = Card(key_chain_id, 0);

WiFiClient wifi_client; // Cliente de Rede WiFi

MQTT_Menssager menssager(&wifi_client);

Doorman doorman(BUZZER_PIN, LOCK_PIN);

Card_Manager cm(SS_PIN, RST_PIN);

bool new_request = true;

bool wait_new_card() {
    int new_card_btn_pin_state = digitalRead(NEW_CARD_PIN);
    if ( new_card_btn_pin_state == LOW && new_request == true ) {
        new_request = false;
        return true;
    } else if ( new_card_btn_pin_state == HIGH && new_request == false ) {
        new_request = true;
        return false;
    }
    return false;
}


void loop_card(void *z) {
    while(true) {
        cm.listen();
        String card_id = cm.read();

        int card_level = 2;
        if (wait_new_card())
        {
            Card* old_card = cm.get(card_id);
            if (old_card->get_level() == 1) {
                String new_card_id = cm.listen_new(card_id);
                if (cm.is_card_registred(new_card_id) == false) {
                    menssager.new_card(new_card_id);
                    Card new_card = Card(new_card_id, 0);
                    cm.append(new_card);
                }
            }

            continue;
        }

        if (cm.is_card_registred(card_id)) {
            Card* card = cm.get(card_id);
            card_level = card->get_level();
        }

        int access_level = MQTT_Menssager::get_access_level();
        int open_time = MQTT_Menssager::get_open_time();

        Serial.println(card_level);
        if (access_level == 0) {
            doorman.access(card_level, open_time);
        } else {
            doorman.admin_only_access(card_level, open_time);
        }


        delay(200);
        Serial.println("next loop");
        new_request = true;
    }
}

void loop_mqtt(void *z) {
    while(true) {
        menssager.loop();
        delay(10);
    }
}
void setup() {
    Serial.begin(115200);
    pinMode(NEW_CARD_PIN, INPUT_PULLUP);
    pinMode(MAGNET_PIN, INPUT_PULLUP);
    // CONFIGURA WIFI
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Configura o WiFi

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("Tentando se conectar na rede: ");
        Serial.println(WIFI_SSID);
    }

    Serial.println("Conectado na Rede WiFi.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    // TERIMNO CONFIGURAÇÃO WIFI

    menssager.init();
    doorman.init();
    cm.init();
    cm.append(white_card);
    // cm.append(key_chain);
    xTaskCreate(loop_mqtt, "loop_mqtt", 2048, nullptr, 1, nullptr);
    xTaskCreate(loop_card, "loop_card", 2048, nullptr, 1, nullptr);

}

bool is_door_open = false;

void loop() {
    int magnet_state = digitalRead(MAGNET_PIN);

    if (magnet_state == HIGH) {
        is_door_open = true;
        doorman.is_door_open = true;
    }

    if (is_door_open == true and magnet_state == LOW) {
        is_door_open = false;
        doorman.is_door_open = false;
    }
    delay(10);
}
