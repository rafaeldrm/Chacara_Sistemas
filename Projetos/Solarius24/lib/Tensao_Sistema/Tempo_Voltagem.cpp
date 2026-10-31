//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Tempo_Voltagem.h"
#include "DEBUG_MODE.h"

//====================================================================================
// CONFIGURAÇÕES
//====================================================================================

constexpr uint16_t LIMITE_NORMAL  = 125; // 12.5V
constexpr uint16_t LIMITE_CRITICO = 123; // 12.3V

constexpr uint32_t TEMPO_LENTO   = 15UL * 60UL * 1000UL; // 15 min
constexpr uint32_t TEMPO_MEDIO   = 5UL  * 60UL * 1000UL; // 5 min
constexpr uint32_t TEMPO_RAPIDO  = 1000;                 // 1 segundo

//====================================================================================
// ESTADO
//====================================================================================

static uint32_t ultimoTempo = 0;

//====================================================================================
// INIT
//====================================================================================

void Tempo_Voltagem_Inicializa()
{
    ultimoTempo = 0;
}

//====================================================================================
// DECISÃO DE TEMPO
//====================================================================================

bool Tempo_Voltagem_Deve_Atualizar(uint16_t tensao_atual)
{
    uint32_t agora = millis();
    uint32_t intervalo;

    if (tensao_atual >= LIMITE_NORMAL)
    {
        intervalo = TEMPO_LENTO;
    }
    else if (tensao_atual >= LIMITE_CRITICO)
    {
        intervalo = TEMPO_MEDIO;
    }
    else
    {
        intervalo = TEMPO_RAPIDO;
    }

    if (agora - ultimoTempo >= intervalo)
    {
        ultimoTempo = agora;


        return true;
    }

    return false;
}