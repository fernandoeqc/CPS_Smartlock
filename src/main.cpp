#include <Arduino.h>
#include "Doorman.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "wifi_app.h"
#include "mqtt_app.h"
#include "mfrc522_app.h"
#include "access_control.h"

#define BUZZER_PIN 33
#define LOCK_PIN 32

extern MFRC522controller mfrc522ctrl;
Doorman doorman(BUZZER_PIN, LOCK_PIN);


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
        mfrc522ctrl.cmd_register_new_card(pld);
    }

    if (strcmp(topic, TOPIC_PREFIX "change_card"))
    {

    }
    //Acionar trava
    if (strcmp(topic, TOPIC_PREFIX "unlock_access"))
    {
        
    }



    //config-tempo-acionamento



    Serial.println();
    Serial.println(" -----------------------");
}

void setup() {
    pinMode(LED, OUTPUT);

    Serial.begin(115200);
    doorman.init();
    
    wifi_init();
    mqtt_init();
    access_control_init();
}

void loop() {
    doorman.access(1);

    delay(10);
}
