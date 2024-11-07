#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi_app.h"
#include "mqtt_app.h"
#include "mfrc522.h"

int LED = 22;

void cmd_set_led(String payload)
{
    if (payload == "0")
    {
        Serial.println("\nDesligando luz");
        digitalWrite(LED, LOW);
    }

    if (payload == "1")
    {
        Serial.println("\nLigando luz");
        digitalWrite(LED, HIGH);
    }
}

void callback_mqtt(char *topic, byte *payload, unsigned int length)
{
    String pld = "";
    Serial.print("Uma mensagem chegou no tópico: ");
    Serial.println(topic);
    Serial.print("Payload: ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        pld.concat((char)payload[i]);
    }

    if (strcmp(topic, TOPIC_PREFIX "set_led"))
    {
        cmd_set_led(pld);
    }

    if (strcmp(topic, TOPIC_PREFIX "register_new_card"))
    {
        cmd_register_new_card(pld);
    }

    Serial.println();
    Serial.println(" -----------------------");
}

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    
    wifi_init();
    mqtt_init();
}

void loop() {
    delay(10);
}
