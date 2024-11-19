//
// Created by Filipe Soares on 18/11/24.
//
#include <Arduino.h>
#include <PubSubClient.h>
#include <Wifi.h>
#include "MQTT_Menssager.h"


MQTT_Menssager::MQTT_Menssager(WiFiClient wifi_client):
 _mqtt_client(wifi_client){};

void MQTT_Menssager::_callback(char* topic, byte* payload, unsigned int length){
    Serial.println("Nova messagem recebida!");
}

void MQTT_Menssager::init() {
    _mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);
    _mqtt_client.setCallback(MQTT_Menssager::_callback);

    while (!_mqtt_client.connected()) {

        Serial.println("Conectando ao Broker MQTT...");

        String clientId = "ESP32Client_" + String(random(0xffff), HEX);
        Serial.println("clientId = " + clientId);

        /* Conecta o cliente ao Broker MQTT.
           Configurações de credenciais e Last Will podem ser configuradas aqui*/
        if (_mqtt_client.connect(clientId.c_str(), "", "")) {

            Serial.println("O cliente " + clientId + " foi conectado com sucesso");
        } else {

            // Estado do cliente MQTT. Quando a conexão falhar pode ser usado para obter informações sobre a falha
            int clientState = _mqtt_client.state();

            Serial.print("Falha ao se conectar. ");
            Serial.println("Estado do cliente: " + (String) clientState);

            delay(2000);
        }
    }

    Serial.print("Tentando enviar a mensagem");

    _mqtt_client.publish(TOPIC_PREFIX"door_access_level", "It's me, Mario!");
    _mqtt_client.subscribe(TOPIC_PREFIX"#");
}

void MQTT_Menssager::door_open(){
    _mqtt_client.publish(TOPIC_PREFIX"esp/state_changed", "1");
}

void MQTT_Menssager::door_closed(){
    _mqtt_client.publish(TOPIC_PREFIX"esp/state_changed", "0");
}

void MQTT_Menssager::new_card(String id){
    _mqtt_client.publish(TOPIC_PREFIX"/new_card", id.c_str());
}
