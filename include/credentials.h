#pragma once

/* CONFIGURAÇÕES DO MQTT*/
// inline const char *mqttServer = "broker.hivemq.com";   // Endereço do Broker MQTT
inline const char *mqttServer = "broker.emqx.io";   // Endereço do Broker MQTT
inline const int mqttPort = 1883;                      // Porta TCP do Broker MQTT
inline const char *mqttUser = "";                      // Usuário MQTT
inline const char *mqttPassword = "";                  // Senha MQTT