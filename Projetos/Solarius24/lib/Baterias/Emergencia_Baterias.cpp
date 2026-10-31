//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Emergencia_Baterias.h"
#include "DEBUG_MODE.h"

#include "Recuperacao_Sistema.h"

//====================================================================================
//                   CONFIG
//====================================================================================

static constexpr uint8_t LIMITE_TENTATIVAS = 3;

//====================================================================================
//                   ESTADO
//====================================================================================

static EmergenciaBaterias _dados;
static uint8_t _tentativas = 0;

//====================================================================================
//                   SETUP
//====================================================================================

void Emergencia_Baterias_Inicializa()
{
    debugSetupArquivo();
    debugSetupDivisao(TEXTO_AMARELO, "EMERGÊNCIA BATERIAS", TEXTO_MAGENTA, "-", 90);

    _dados =
    {
        .ativo = false,
        .modo_degradado = false
    };

    _tentativas = 0;

    debugOk("Emergência Baterias pronta, Variáveis Limpas");
}

//====================================================================================
//                   LOOP
//====================================================================================

void Emergencia_Baterias_Atualiza(const StatusDiagnosticoINA3221& diag)
{
    // SENSOR OK → NORMALIZA
    if (diag.estado == SENSOR::OK)
    {
        _dados.ativo = false;
        _dados.modo_degradado = false;
        _tentativas = 0;
        return;
    }

    _dados.ativo = true;

    //============================================================
    // TENTATIVA 1 → RESET LÓGICO
    //============================================================

    if (_tentativas == 0)
    {
        Recuperacao_Sistema_Tentar_Logica();
        _tentativas++;
        return;
    }

    //============================================================
    // TENTATIVA 2 → WATCHDOG
    //============================================================

    if (_tentativas == 1)
    {
        _tentativas++;
        Recuperacao_Sistema_Forcar_Watchdog();
        return;
    }

    //============================================================
    // TENTATIVA 3 → RESET FÍSICO
    //============================================================

    if (_tentativas == 2)
    {
        _tentativas++;
        Recuperacao_Sistema_Reset_Fisico_555();
        return;
    }

    //============================================================
    // FALHA TOTAL → MODO DEGRADADO
    //============================================================

    _dados.modo_degradado = true;

    debugErro("MODO DEGRADADO ATIVO (BATERIAS)");

    // aqui o sistema continua funcionando no "cego consciente"
}