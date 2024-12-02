//
// Created by Filipe Soares on 18/11/24.
//
#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>


class MQTT_Menssager{
  public:
    explicit MQTT_Menssager(WiFiClient* wifi_client, int access_level=0 , int open_time=2000);

    void init();
    void door_open();
    void door_closed();
    void new_card(String id);
    void loop();

    static int get_access_level();
    static int get_open_time();
    static void set_open_time(int open_time);
    static void set_access_level(int access_level);

  private:
    PubSubClient _mqtt_client;
    static int _access_level;
    static int _open_time;

    static void _callback(char* topic, byte* payload, unsigned int length);
};