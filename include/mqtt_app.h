#pragma once
#include <Arduino.h>
#include <PubSubClient.h>

extern PubSubClient clientMqtt;
void callback_mqtt(char *topic, byte *payload, unsigned int length) __attribute__((__weak__));
void mqtt_init();
void mqtt_loop();
