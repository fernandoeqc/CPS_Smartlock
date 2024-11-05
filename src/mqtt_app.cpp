#include "mqtt_app.h"
#include <Arduino.h>

#include "WiFi.h"
#include "WiFiClient.h"
#include "credentials.h"


WiFiClient espClient; // Cliente de Rede WiFi
PubSubClient clientMqtt(espClient); // Cria uma instancia de um cliente MQTT

void callback_mqtt(char *topic, byte *payload, unsigned int length)
{
    Serial.println("weak func callback_mqtt()");
    // Weak function
}

void mqtt_init()
{
    Serial.println("Conectado na Rede WiFi.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    clientMqtt.setServer(mqttServer, mqttPort);
    clientMqtt.setCallback(callback_mqtt);

    /* Verifica se o cliente está conectado ao Broker */
    while (!clientMqtt.connected())
    {

        Serial.println("Conectando ao Broker MQTT...");

        String clientId = "ESP32Client_" + String(random(0xffff), HEX);
        Serial.println("clientId = " + clientId);

        /* Conecta o cliente ao Broker MQTT.
           Configurações de credenciais e Last Will podem ser configuradas aqui*/
        if (clientMqtt.connect(clientId.c_str(), mqttUser, mqttPassword))
        {

            Serial.println("O cliente " + clientId + " foi conectado com sucesso");
        }
        else
        {

            // Estado do cliente MQTT. Quando a conexão falhar pode ser usado para obter informações sobre a falha
            int clientState = clientMqtt.state();

            Serial.print("Falha ao se conectar. ");
            Serial.println("Estado do cliente: " + (String)clientState);

            delay(2000);
        }
    }

    Serial.print("Tentando enviar a mensagem");

    clientMqtt.publish("unifor/CPS/Smartlock", "Hello from ESP32");
    clientMqtt.subscribe("unifor/cps/led");
}

void mqtt_loop()
{
    clientMqtt.loop();
}