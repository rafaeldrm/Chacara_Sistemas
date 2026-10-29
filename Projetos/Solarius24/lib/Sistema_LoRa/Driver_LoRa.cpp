//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include <SPI.h>
#include <LoRa.h>
#include "Driver_LoRa.h"
#include "Mapa_Pinos_Arduino.h"

// ============================
// ESTADO INTERNO (privado)
// ============================

static struct {
    bool online;
    uint8_t ss;
    uint8_t rst;
    uint8_t dio0;
} lora;

//====================================================================================
//                   SETUP
//====================================================================================

bool Driver_LoRa_Inicializa()
{
    lora.ss   = pino(ARDUINO_CONEXAO::LORA::SS);
    lora.rst  = pino(ARDUINO_CONEXAO::LORA::RST);
    lora.dio0 = pino(ARDUINO_CONEXAO::LORA::DIO0);

    LoRa.setPins(lora.ss, lora.rst, lora.dio0);

    pinMode(lora.rst, OUTPUT);
    digitalWrite(lora.rst, HIGH);

    if (!LoRa.begin(433E6))
    {
        lora.online = false;
        return false;
    }

    // Configuração rádio
    LoRa.setSignalBandwidth(125E3);
    LoRa.setSpreadingFactor(9);
    LoRa.setCodingRate4(8);
    LoRa.setSyncWord(0xF3);
    LoRa.setTxPower(18);
    LoRa.enableCrc();

    lora.online = true;
    return true;
}

//====================================================================================
//                   STATUS
//====================================================================================

bool Driver_LoRa_Online()
{
    return lora.online;
}

//====================================================================================
//                   ENVIO 
//====================================================================================

bool Driver_LoRa_Enviar(const char* msg)
{
    if (!lora.online)
        return false;

    LoRa.beginPacket();
    LoRa.print(msg);

    int status = LoRa.endPacket();

    if (status != 1)
    {
        lora.online = false;
        return false;
    }

    return true;
}

//====================================================================================
//                   RESET CONTROLADO
//====================================================================================

bool Driver_LoRa_Reiniciar()
{
    lora.online = false;

    LoRa.sleep();

    SPI.end();
    SPI.begin();

    digitalWrite(lora.rst, LOW);
    delay(5);
    digitalWrite(lora.rst, HIGH);
    delay(10);

    return Driver_LoRa_Inicializa();
}