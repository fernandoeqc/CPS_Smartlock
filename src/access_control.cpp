#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "access_control.h"
#include "mfrc522_app.h"

#define LED_VERD 4
#define LED_VERM 2
#define BUZZER 33

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void ARDUINO_ISR_ATTR onTimer()
{
    // Increment the counter and set the time of ISR
    portENTER_CRITICAL_ISR(&timerMux);

    // Bloquear a trava
    portEXIT_CRITICAL_ISR(&timerMux);
    // It is safe to use digitalRead/Write here if you want to toggle an output
}

void t_access_control(void *z)
{
    while (true)
    {
        if (!card_detected())
        {
            continue;
        }

        if (!card_registered())
        {
            Serial.println(": Cartão não registrado");
            continue;
        }

        String level = get_card_level();

        if (level == "admin")
        {
            Serial.println(": Acesso Liberado");

            digitalWrite(BUZZER, HIGH);
            delay(250);
            digitalWrite(BUZZER, LOW);

            digitalWrite(LED_VERD, HIGH);
            delay(2000);
            digitalWrite(LED_VERD, LOW);
        }
        else if (level == "user")
        {
            Serial.println(": Acesso Liberado");

            digitalWrite(BUZZER, HIGH);
            delay(250);
            digitalWrite(BUZZER, LOW);

            digitalWrite(LED_VERD, HIGH);
            delay(2000);
            digitalWrite(LED_VERD, LOW);
        }
        else
        {
            Serial.println(": Acesso Negado");

            for (int i = 0; i < 3; i++)
            {
                digitalWrite(LED_VERM, HIGH);
                digitalWrite(BUZZER, HIGH);
                delay(500);

                digitalWrite(LED_VERM, LOW);
                digitalWrite(BUZZER, LOW);
                delay(500);
            }
        }

        card_clear();
    }
    delay(1000);
    
}

void access_control_init()
{
    pinMode(LED_VERM, OUTPUT);
    pinMode(LED_VERD, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    // Set timer frequency to 1Mhz
    timer = timerBegin(0, 80, true);

    // Attach onTimer function to our timer.
    timerAttachInterrupt(timer, &onTimer, true);

    xTaskCreate(t_access_control, "ACCESS CONTROL", 1024*1, (void*)NULL, 1, NULL);
}