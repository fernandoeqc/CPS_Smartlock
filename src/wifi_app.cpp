#include "wifi_app.h"
#include <Arduino.h>

#include "credentials.h"

void wifi_init()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); // Configura o WiFi

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print("Tentando se conectar na rede: ");
        Serial.println(WIFI_SSID);
    }
}