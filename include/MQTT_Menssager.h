//
// Created by Filipe Soares on 18/11/24.
//
#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <Wifi.h>


class MQTT_Menssager{
  public:
    MQTT_Menssager(WiFiClient wifi_client);

    void init();
    void door_open();
    void door_closed();
    void new_card(String id);

  private:
    PubSubClient _mqtt_client;

    static void _callback(char* topic, byte* payload, unsigned int length);
};