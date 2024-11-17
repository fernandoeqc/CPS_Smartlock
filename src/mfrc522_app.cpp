#include <MFRC522.h>
#include <SPI.h>

#define PINO_RST 15
#define PINO_SS 5

#define MAX_TIME_WAIT_NEW_CARD 10 * 1000 // 10 secs
#define MAX_CARDS 10

// Definicoes pino modulo RC522
MFRC522 mfrc522(PINO_SS, PINO_RST);

xTaskHandle t_rfid_handle;

struct UUID {
    String uuid;
    String level;
};

struct UUID_list {
    UUID card [MAX_CARDS];
    int count = 0;
} uuid_list;

String level_new_card = "";


bool card_found = false;
String card_data = "";
int current_card_index = -1;

bool load_card_data(String *conteudo);

/* Tasks */
void t_rfid_new_card(void *z)
{
    if (uuid_list.count >= MAX_CARDS)
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
        if (load_card_data(&conteudo))
        {
            uuid_list.card[uuid_list.count].uuid = conteudo; /// COPIAR NOVO CARTÃO
            Serial.println("Novo cartão encontrado");
            break;
        }
    }

    level_new_card = "";

    vTaskResume(t_rfid_handle);
    vTaskDelete(NULL);
}

void t_rfid(void *z)
{
    while (true)
    {
        delay(100);
        if (load_card_data(&card_data))
        {
            card_found = true;
            Serial.println("Cartão detectado");
            while (card_found)
            {
                delay(5000); // Delay para aguardar checagem de cartão lido
            }
            
        }
    }
}

bool load_card_data(String *conteudo)
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




/* Public functions */
bool card_detected()
{
    if (!card_found)
    {
        return false;
    }

    card_found = false;
    return true;
}

bool card_registered()
{
    for (size_t i = 0; i < MAX_CARDS; i++)
    {
        if (card_data == uuid_list.card[i].uuid)
        {
            current_card_index = i;
            return true;
        }
    }
    return false;
}

String get_card_level()
{
    return uuid_list.card[current_card_index].level;
}

void card_clear()
{
    card_data = "";
    card_found = false;
    current_card_index = -1;
}

void cmd_register_new_card(String level)
{
    level_new_card = level;
    xTaskCreate(t_rfid_new_card, "RFID_new_card", 1024*2, (void*)&level_new_card, 1, NULL);
}

void rfid_init()
{
    SPI.begin();        // Configura a comunicação SPI
    mfrc522.PCD_Init(); // Configura o modulo MFRC522

    xTaskCreate(t_rfid, "RFID", 1024*2, (void*)NULL, 1, &t_rfid_handle);
}
