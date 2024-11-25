//
// Created by Filipe Soares on 18/11/24.
//
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "MQTT_Menssager.h"
#include <ArduinoJson.h>
#include "Card_Manager.h"
#include "Doorman.h"

int MQTT_Menssager::_access_level;
int MQTT_Menssager::_open_time;

extern Card_Manager cm;
extern Doorman doorman;

MQTT_Menssager::MQTT_Menssager(WiFiClient* wifi_client, int access_level, int open_time):
 _mqtt_client(*wifi_client) {
    MQTT_Menssager::set_access_level(access_level);
    MQTT_Menssager::set_open_time(open_time);
};

void MQTT_Menssager::set_access_level(int access_level) {
    if (access_level == 1) {
        MQTT_Menssager::_access_level = access_level;
    } else {
        MQTT_Menssager::_access_level = 0;
    }
}

void MQTT_Menssager::set_open_time(int open_time) {
    if (open_time <= 20000 && open_time >= 2000) {
        MQTT_Menssager::_open_time = open_time;
    } else {
        MQTT_Menssager::_open_time = 2000;
    }
}

int MQTT_Menssager::get_access_level() {
    return MQTT_Menssager::_access_level;
}

int MQTT_Menssager::get_open_time() {
    return MQTT_Menssager::_open_time;
}

void MQTT_Menssager::_callback(char* topic, byte* payload, unsigned int length){
    Serial.println("Nova messagem recebida!");
    Serial.println(topic);

    payload[length] = '\0'; // Ensure null terminated string

    if (strcmp(topic, TOPIC_PREFIX"door_access_level") == 0){
        Serial.println("In door_access_level");
        if (length != 1) {
            Serial.println("Incorrect size");
            return;
        }

        char new_access_level_char = static_cast<char>(payload[0]);
        Serial.println("The payload is:");
        Serial.println(new_access_level_char);
        if (new_access_level_char == '1' || new_access_level_char == '0') {
            Serial.println("Updating the access level");
            int new_access_level_int = atoi(&new_access_level_char);
            MQTT_Menssager::set_access_level(new_access_level_int);
        }

    }
    else if (strcmp(topic, TOPIC_PREFIX "change_card_level") == 0){
        //ex: {"card": "03 03 03 03", "level": 1}
        Serial.println("In change_card_level");
        if (length != 1) {
            Serial.println("Incorrect size");
            return;
        }

        Serial.println("The payload is:");

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
        }
        String card = doc["card"];
        int level = doc["level"];
        if (level == 1) {
            cm.upgrade(card);
        } else if (level == 0) {
            cm.downgrade(card);
        }
    }
    else if (strcmp(topic, TOPIC_PREFIX "change_card_level") == 0){
        int open_time = atoi((const char*)payload);
        if (open_time >= 2000 && open_time <= 20000) {
            Serial.println("Tempo de abertura de porta atualizado: " + String(open_time));
            _open_time = open_time;
        }
    }
    else if (strcmp(topic, TOPIC_PREFIX "unlock_access") == 0){
        doorman.access(1);
    }
}

void MQTT_Menssager::init() {
    Serial.println("Iniciando menssager");
    this->_mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
    this->_mqtt_client.setCallback(MQTT_Menssager::_callback);

    while (!this->_mqtt_client.connected()) {

        Serial.println("Conectando ao Broker MQTT...");

        String clientId = "ESP32Client_" + String(random(0xffff), HEX);
        Serial.println("clientId = " + clientId);

        /* Conecta o cliente ao Broker MQTT.
           Configurações de credenciais e Last Will podem ser configuradas aqui*/
        if (this->_mqtt_client.connect(clientId.c_str(), "", "")) {

            Serial.println("O cliente " + clientId + " foi conectado com sucesso");
        } else {

            // Estado do cliente MQTT. Quando a conexão falhar pode ser usado para obter informações sobre a falha
            int clientState = this->_mqtt_client.state();

            Serial.print("Falha ao se conectar. ");
            Serial.println("Estado do cliente: " + (String) clientState);

            delay(2000);
        }
    }

    Serial.println("Tentando enviar a mensagem");

    this->_mqtt_client.publish(TOPIC_PREFIX"door_access_level", "It's me, Luigi!");
    this->_mqtt_client.subscribe(TOPIC_PREFIX"#");
}

void MQTT_Menssager::loop() {
    this->_mqtt_client.loop();
}

void MQTT_Menssager::door_open(){
    this->_mqtt_client.publish(TOPIC_PREFIX"esp/state_changed", "1");
}

void MQTT_Menssager::door_closed(){
    this->_mqtt_client.publish(TOPIC_PREFIX"esp/state_changed", "0");
}

void MQTT_Menssager::new_card(String id){
    this->_mqtt_client.publish(TOPIC_PREFIX"/new_card", id.c_str());
}
