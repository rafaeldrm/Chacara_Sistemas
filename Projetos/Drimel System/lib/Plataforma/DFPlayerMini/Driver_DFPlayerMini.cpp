//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include <DFRobotDFPlayerMini.h>

#include "Driver_DFPlayerMini.h"
#include "DEBUG_MODE.h"
#include "Mapa_Pinos_Arduino.h"

#include "Tempo.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

namespace
{
    DFRobotDFPlayerMini DF_PlayerMini;

    static constexpr uint8_t PINO_BUSY_EM_REPRODUCAO =
        pino(ARDUINO_CONEXAO::DFPLAYER::BUSY_EM_REPRODUCAO);
}

static constexpr uint32_t TEMPO_ESPERA_BUSY = TEMPO::MILLISSEGUNDOS(500);

//====================================================================================
//                   SETUP
//====================================================================================

bool Driver_DFPlayerMini_Inicializa()
{
    debugSetupDivisao(
        COR::AMARELO,
        "DRIVER DFPLAYERMINI INICIALIZANDO",
        COR::MAGENTA,
        "=",
        45
    );

    pinMode(PINO_BUSY_EM_REPRODUCAO, INPUT);

    DF_PlayerMini.setTimeOut(TEMPO_ESPERA_BUSY);

    if (!DF_PlayerMini.begin(Serial1, true, true))
    {
        debugErro("Nao inicializado:");
        debugSetupTexto(
            "1. Cheque as conexoes do DFPlayer Mini",
            LINHA
        );
        debugSetupTexto(
            "2. Insira um cartao SD",
            LINHA
        );

        return false;
    }

    debugSetupTexto(
        "Modulo DFPlayer Mini inicializado!",
        LINHA
    );

    return true;
}

//====================================================================================
//                   TOCA FAIXA
//====================================================================================

bool Driver_DFPlayerMini_TocaFaixa(uint8_t pasta, uint8_t faixa)
{
    DF_PlayerMini.playFolder(pasta, faixa);

    return true;
}

//====================================================================================
//                   PAUSA
//====================================================================================

bool Driver_DFPlayerMini_Pausa()
{
    DF_PlayerMini.pause();

    return true;
}

//====================================================================================
//                   CONTINUA
//====================================================================================

bool Driver_DFPlayerMini_Continua()
{
    DF_PlayerMini.start();

    return true;
}

//====================================================================================
//                   PARA MÚSICA
//====================================================================================

bool Driver_DFPlayerMini_Para()
{
    DF_PlayerMini.stop();

    return true;
}

//====================================================================================
//                   DEFINE VOLUME
//====================================================================================

void Driver_DFPlayerMini_DefineVolume(uint8_t volume)
{
    DF_PlayerMini.volume(volume);
}