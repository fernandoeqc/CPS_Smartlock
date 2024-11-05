#include <MFRC522.h>
#include <SPI.h>

#define PINO_RST 15
#define PINO_SS 5
#define LED_VERD 4
#define LED_VERM 2
#define BUZZER 33

#define MAX_TIME_WAIT_NEW_CARD 10 * 1000 // 10 secs
#define MAX_CARDS 10

// Definicoes pino modulo RC522
MFRC522 mfrc522(PINO_SS, PINO_RST);

xTaskHandle t_rfid_handle;
struct UUID {
    String list [MAX_CARDS];
    int count = 0;
} uuids;

bool get_card_data(String *conteudo);


void t_rfid_new_card(void *z)
{
    if (uuids.count >= MAX_CARDS)
    {
        Serial.println("Número de cartões excedido!");
        return;
    }

    vTaskSuspend(t_rfid_handle);
    String conteudo = "";

    long end_task = millis() + MAX_TIME_WAIT_NEW_CARD;
    while (millis() < end_task)
    {
        delay(10);
        if (!get_card_data(&conteudo))
        {
            // uuids[uuids.count] = conteudo; /// COPIAR NOVO CARTÃO
            continue;;
        }

        Serial.println("Novo cartão encontrado");

    }

    vTaskResume(t_rfid_handle);
    vTaskDelete(NULL);
}

void t_rfid(void *z)
{
    // Aguarda a aproximacao do cartao
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    // Seleciona um dos cartoes
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    // Obtem a UID
    String conteudo = "";

    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        conteudo.concat(String(mfrc522.uid.uidByte[i] < HEX ? " 0" : " "));
        conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    Serial.print(conteudo);

    bool found_card = false;
    for (size_t i = 0; i < MAX_CARDS; i++)
    {
        if (conteudo.substring(1) == uuids.list[i])
        {
            found_card = true;
            break;
        }
    }

    if (found_card)
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
}

bool get_card_data(String *conteudo)
{
    // Aguarda a aproximacao do cartao
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return false;
    }

    // Seleciona um dos cartoes
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return false;
    }

    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        conteudo->concat(String(mfrc522.uid.uidByte[i] < HEX ? " 0" : " "));
        conteudo->concat(String(mfrc522.uid.uidByte[i], HEX));
    }

    Serial.print(*conteudo);
    return true;
}
void cmd_register_new_card()
{
    xTaskCreate(t_rfid_new_card, "RFID_new_card", 1024*2, (void*)NULL, 1, NULL);
}

void rfid_init()
{
    Serial.begin(9600); // Configura a porta serial
    SPI.begin();        // Configura a comunicação SPI
    mfrc522.PCD_Init(); // Configura o modulo MFRC522

    pinMode(LED_VERM, OUTPUT);
    pinMode(LED_VERD, OUTPUT);
    pinMode(BUZZER, OUTPUT);

    xTaskCreate(t_rfid, "RFID", 1024*2, (void*)NULL, 1, &t_rfid_handle);
}
