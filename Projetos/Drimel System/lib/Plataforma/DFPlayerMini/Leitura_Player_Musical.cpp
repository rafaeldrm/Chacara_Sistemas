//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Player_Musical.h"
#include "DEBUG_MODE.h"

#include "Mapa_Pinos_Arduino.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

namespace
{
    static constexpr uint8_t PINO_BUSY_EM_REPRODUCAO =
        pino(ARDUINO_CONEXAO::DFPLAYER::BUSY_EM_REPRODUCAO);

    static constexpr uint8_t VOLUME_DRIMEL_MINIMO = 0;
    static constexpr uint8_t VOLUME_DRIMEL_MAXIMO = 100;

    static constexpr uint8_t VOLUME_DFPLAYER_MINIMO = 0;
    static constexpr uint8_t VOLUME_DFPLAYER_MAXIMO = 30;
}

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Player_Musical_Inicializa()
{

}

//====================================================================================
//                   Em_Reproducao
//===================================================================================

bool Leitura_Player_Musical_Em_Reproducao()
{
    return digitalRead(PINO_BUSY_EM_REPRODUCAO) == LOW;
}

//====================================================================================
//                   Converte_Volume_Para_DFPlayer
//====================================================================================

uint8_t Leitura_Player_Musical_Converte_Volume_Para_DFPlayer(uint8_t volumeDrimel)
{
    volumeDrimel = constrain(
        volumeDrimel,
        VOLUME_DRIMEL_MINIMO,
        VOLUME_DRIMEL_MAXIMO
    );
    return (volumeDrimel * VOLUME_DFPLAYER_MAXIMO + 50) / 100;
}
//====================================================================================
//                   Converte_Volume_Para_Drimel
//====================================================================================

uint8_t Leitura_Player_Musical_Converte_Volume_Para_Drimel(uint8_t volumeDFPlayer)
{
    volumeDFPlayer = constrain(
        volumeDFPlayer,
        VOLUME_DFPLAYER_MINIMO,
        VOLUME_DFPLAYER_MAXIMO
    );
    return (volumeDFPlayer * 100 + 15) / 30;
}