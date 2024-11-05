#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi_app.h"
#include "mqtt_app.h"

int LED = 22;

void cmd_set_led(byte *payload)
{
    if (payload[0] == '0')
    {
        Serial.println("\nDesligando luz");
        digitalWrite(LED, LOW);
    }

    if (payload[0] == '1')
    {
        Serial.println("\nLigando luz");
        digitalWrite(LED, HIGH);
    }
}

void cmd_register_new_card(byte *payload)
{

}

void callback_mqtt(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Uma mensagem chegou no tópico: ");
    Serial.println(topic);
    Serial.print("Payload: ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }

    if (strcmp(topic, TOPIC_PREFIX "set_led"))
    {
        cmd_set_led(payload);
    }

    if (strcmp(topic, TOPIC_PREFIX "register_new_card"))
    {
        cmd_register_new_card(payload);
    }

    Serial.println();
    Serial.println(" -----------------------");
}

void t_mqtt(void* z)
{
    while (true)
    {
        mqtt_loop();
        delay(10);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    
    wifi_init();
    mqtt_init();

    xTaskCreate(t_mqtt, "MQTT", 1024*2, (void*)NULL, 1, NULL);
}

void loop() {
    delay(10);
}
