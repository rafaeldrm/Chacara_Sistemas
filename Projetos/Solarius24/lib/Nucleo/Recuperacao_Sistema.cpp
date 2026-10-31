#include "Recuperacao_Sistema.h"
#include "DEBUG_MODE.h"
#include "Sistema_Seguranca.h"
#include "Mapa_Pinos_Arduino.h"
#include <Wire.h>
#include <SPI.h>


//====================================================================================
// ESTADO
//====================================================================================

static TipoRecuperacao _ultimoReset = TipoRecuperacao::NENHUMA;

static constexpr uint8_t PINO_RESET_555 =(pino(ARDUINO_CONEXAO::DIAGNOSTICO::PULSO_555_RESET));

//====================================================================================
// INIT
//====================================================================================

void Recuperacao_Sistema_Inicializa()
{
    pinMode(PINO_RESET_555, OUTPUT);
    digitalWrite(PINO_RESET_555, LOW);

    // lê EEPROM aqui depois
    debugOk("Recuperação Sistema pronta");
}

//====================================================================================
// TENTATIVA LEVE
//====================================================================================

bool Recuperacao_Sistema_Tentar_Logica()
{
    debugAviso("RECUPERAÇÃO LOGICA");

    // reset I2C
    Wire.end();
    delay(10);
    Wire.begin();

    // reset SPI (se usar)
    // SPI.end();
    // SPI.begin();

    return true;
}

//====================================================================================
// WATCHDOG FORÇADO
//====================================================================================

void Recuperacao_Sistema_Forcar_Watchdog()
{
    debugErro("FORÇANDO WATCHDOG");

    _ultimoReset = TipoRecuperacao::WATCHDOG;

    // trava proposital
    while (true)
    {
        // não alimenta watchdog
    }
}

//====================================================================================
// RESET FÍSICO (555)
//====================================================================================

void Recuperacao_Sistema_Reset_Fisico_555()
{
    debugErro("RESET FISICO 555");

    _ultimoReset = TipoRecuperacao::RESET_FISICO;

    // aqui você ativa o pino do 555
    digitalWrite(PINO_RESET_555, HIGH);
    //daqui fica ligado e o ardino se desliga.
}

//====================================================================================
// GET
//====================================================================================

TipoRecuperacao Recuperacao_Sistema_Obter_Ultimo_Reset()
{
    return _ultimoReset;
}