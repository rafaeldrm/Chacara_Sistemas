//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include <SPI.h>
#include <LoRa.h>
#include "Driver_LoRa.h"
#include "Mapa_Pinos_Arduino.h"
#include "DEBUG_MODE.h"

// ====================================================================================
// ESTADO INTERNO (privado)
// ====================================================================================

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
        debugSetupDivisao(COR::AMARELO, "DRIVER LORA INICIALIZANDO", COR::MAGENTA, "=", 45);
        
    lora.ss   = pino(ARDUINO_CONEXAO::LORA::SS);
    lora.rst  = pino(ARDUINO_CONEXAO::LORA::RST);
    lora.dio0 = pino(ARDUINO_CONEXAO::LORA::DIO0);

    debugOk("Pinos carregados LORA:SS, RST e DIO0");

    LoRa.setPins(lora.ss, lora.rst, lora.dio0);

    pinMode(lora.rst, OUTPUT);
    digitalWrite(lora.rst, HIGH);



        if (!LoRa.begin(433E6))
        {
            lora.online = false;

            debugErro("LoRa !!! FALHA AO INICIAR O SX1278");

            return false;
        
        }

    // Configuração rádio
    LoRa.setSignalBandwidth(125E3);
    LoRa.setSpreadingFactor(9);
    LoRa.setCodingRate4(8);
    LoRa.setSyncWord(0xF3);
    LoRa.setTxPower(18);
    LoRa.enableCrc();

    debugOk("LoRa >>> RADIO CONFIGURADO");

    lora.online = true;

    debugOk("LoRa >>> DRIVER ONLINE");

    return true;
}

//====================================================================================
//                   STATUS
//====================================================================================

//====================================================================================
//                   STATUS
//====================================================================================

bool Driver_LoRa_Online()
{
    return lora.online;
}

//====================================================================================
//                   RESET CONTROLADO
//====================================================================================

//====================================================================================
//                   RESET CONTROLADO
//====================================================================================

bool Driver_LoRa_Reiniciar()
{
    debugAviso("--- INICIANDO REINÍCIO CONTROLADO DO LoRa ---");

    lora.online = false;

    LoRa.end();   // põe o rádio em SLEEP e desliga o SPI

    debugOk("LoRa desligado");

    // O Driver_LoRa_Inicializa() religa o SPI, e o LoRa.begin() pulsa o pino de RESET
    return Driver_LoRa_Inicializa();
}

//====================================================================================
//                   NÚMERO ALEATÓRIO (NONCE DO BOOT)
//====================================================================================

uint32_t Driver_LoRa_Aleatorio32()
{
    uint32_t valor = micros();

    if (!lora.online)
        return valor;   // sem rádio, pelo menos o micros() não repete tão fácil



    LoRa.receive();   // o RSSI de banda larga só tem ruído com o rádio ouvindo

    for (uint8_t i = 0; i < 32; ++i)
    {
        valor = ((valor << 3) | (valor >> 29)) ^ LoRa.random();

        delayMicroseconds(150);
    }

    LoRa.idle();   // devolve o rádio ao standby

    return valor;
}