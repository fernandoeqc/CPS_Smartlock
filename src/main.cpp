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


String white_card_id = String("bd 83 8a 21");
String key_chain_id = String("7a 6e 2d a4");

Card white_card = Card(white_card_id, 1);
Card key_chain = Card(key_chain_id, 0);

Doorman doorman(BUZZER_PIN, LOCK_PIN);

Card_Manager cm(SS_PIN, RST_PIN);

WiFiClient wifi_client; // Cliente de Rede WiFi
PubSubClient mqtt_client(wifi_client); // Cria uma instancia de um cliente MQTT

int access_level = 1;
int open_time = 2000;

/* CONFIGURAÇÕES DO MQTT*/
const char* mqtt_server = "broker.mqtt.cool"; // Endereço do Broker MQTT
const int mqtt_port = 1883; // Porta TCP do Broker MQTT
const char* mqtt_user = ""; // Usuário MQTT
const char* mqtt_password = ""; // Senha MQTT

void loop_card(void *z) {
    while(true) {
        cm.listen();
        String card_id = cm.read();

        int card_level = 2;

        if (cm.is_card_registred(card_id)) {
            Card* card = cm.get(card_id);
            card_level = card->get_level();
        }

        Serial.println(card_level);
        if (access_level == 0) {
            doorman.access(card_level, open_time);
        } else {
            doorman.admin_only_access(card_level, open_time);
        }


        delay(200);
        Serial.println("next loop");

    }
}

void loop_mqtt(void *z) {
    while(true) {
        delay(1000);
        Serial.println("Mqtt a implementar");
    }
}
void setup() {
    Serial.begin(115200);
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

    mqtt_client.setServer(MQTT_SERVER, MQTT_PORT);


    /* Verifica se o cliente está conectado ao Broker */
    while (!mqtt_client.connected()) {

        Serial.println("Conectando ao Broker MQTT...");

        String clientId = "ESP32Client_" + String(random(0xffff), HEX);
        Serial.println("clientId = " + clientId);

        /* Conecta o cliente ao Broker MQTT.
           Configurações de credenciais e Last Will podem ser configuradas aqui*/
        if (mqtt_client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {

            Serial.println("O cliente " + clientId + " foi conectado com sucesso");
        } else {

            // Estado do cliente MQTT. Quando a conexão falhar pode ser usado para obter informações sobre a falha
            int clientState = mqtt_client.state();

            Serial.print("Falha ao se conectar. ");
            Serial.println("Estado do cliente: " + (String) clientState);

            delay(2000);
        }
    }

    Serial.print("Tentando enviar a mensagem");

    mqtt_client.publish(TOPIC_PREFIX"door_access_level", "It's me, Mario!");
    // mqtt_client.subscribe("unifor/cps/led");

    doorman.init();
    cm.init();
    cm.append(white_card);
    cm.append(key_chain);
    xTaskCreate(loop_mqtt, "loop_mqtt", 2048, NULL, 1, NULL);
    xTaskCreate(loop_card, "loop_mqtt", 2048, NULL, 1, NULL);

}

void loop() {
    delay(1000);

}
