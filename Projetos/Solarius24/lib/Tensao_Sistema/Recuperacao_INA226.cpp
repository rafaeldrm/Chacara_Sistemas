#include "Recuperacao_INA226.h"
#include "Driver_INA226.h"
#include "DEBUG_MODE.h"

//====================================================================================
// CONFIG
//====================================================================================

static constexpr uint8_t  MAX_TENTATIVAS = 3;
static constexpr uint32_t INTERVALO_MS   = 5000;

//====================================================================================
// ESTADO
//====================================================================================

static uint8_t  _tentativas = 0;
static uint32_t _ultimo_tempo = 0;

//====================================================================================
// LOOP
//====================================================================================

void Recuperacao_INA226_Executar(const StatusDiagnosticoINA226& status)
{
    uint32_t agora = millis();

    //============================================================
    // 1. SENSOR OK → RESETA TUDO
    //============================================================

    if (status.estado != SENSOR::ERRO)
    {
        _tentativas = 0;
        return;
    }

    //============================================================
    // 2. CONTROLE DE TEMPO
    //============================================================

    if (agora - _ultimo_tempo < INTERVALO_MS)
        return;

    _ultimo_tempo = agora;

    //============================================================
    // 3. LIMITE DE TENTATIVAS
    //============================================================

    if (_tentativas >= MAX_TENTATIVAS)
        return;

    //============================================================
    // 4. RECUPERAÇÃO
    //============================================================

    debugAviso("RECUPERANDO INA226");

    Driver_INA226_Reset();
    Driver_INA226_Liga();
    Driver_INA226_Calibra();

    _tentativas++;
}