#include "Decisor_Voltagem.h"

#include "Diagnostico_INA226.h"
#include "Diagnostico_INA3221.h"

#include "Leitura_Voltagem.h"
#include "Leitura_Baterias.h"

#include "Estados_Modulos.h"
#include "DEBUG_MODE.h"

//====================================================================================
// ESTADO
//====================================================================================

static DadosVoltagemSistema _dados;

//====================================================================================
// INIT
//====================================================================================

void Decisor_Voltagem_Inicializa()
{
    _dados =
    {
        .tensao_mV = 0,
        .fonte = FonteVoltagem::INDEFINIDO,
        .valido = false
    };

    debugOk("Decisor Voltagem pronto");
}

//====================================================================================
// AUXILIAR → MENOR CÉLULA VÁLIDA (INA3221)
//====================================================================================

static bool Obter_Menor_Celula_Valida(uint16_t* saida)
{
    const auto& pack = Leitura_Baterias_Obter_Pack();

    if (pack.erro || !pack.pronta)
        return false;

    int32_t menor = pack.celula[0];

    for (uint8_t i = 1; i < 4; i++)
    {
        if (pack.celula[i] < menor)
            menor = pack.celula[i];
    }

    // segurança contra lixo absurdo
    if (menor < 8000 || menor > 16000)
        return false;

    *saida = (uint16_t)menor;
    return true;
}

//====================================================================================
// LOOP
//====================================================================================

void Decisor_Voltagem_Atualiza()
{
    const auto& diag226 = Diagnostico_INA226_ObterStatus();
    const auto& diag3221 = Diagnostico_INA3221_ObterStatus();

    //============================================================
    // PRIORIDADE 1 → INA226 (sensor principal)
    //============================================================

    if (diag226.estado == SENSOR::OK || diag226.estado == SENSOR::INSTAVEL)
    {
        const auto& leitura = Leitura_Voltagem_ObterDados();

    //    if (leitura.pronta && leitura.novo_dado)
        if (leitura.estado == LEITURA::PRONTA)
        {
            _dados.tensao_mV = leitura.media;
            _dados.fonte = FonteVoltagem::INA226;
            _dados.valido = true;

            debugLoopTexto("VOLTAGEM: INA226", NEGRITO | VERDE | LINHA);

            return;
        }
    }

    //============================================================
    // PRIORIDADE 2 → INA3221 (fallback inteligente)
    //============================================================

    if (diag3221.estado == SENSOR::OK || diag3221.estado == SENSOR::INSTAVEL)
    {
        uint16_t tensaoFallback;

        if (Obter_Menor_Celula_Valida(&tensaoFallback))
        {
            // Converte mV (12000) para escala 0.1V (120) para manter compatibilidade
            _dados.tensao_mV = tensaoFallback / 100; 
            _dados.fonte = FonteVoltagem::INA3221;
            _dados.valido = true;

            debugAviso("VOLTAGEM FALLBACK: INA3221");

            return;
        }
    }

    //============================================================
    // FALHA TOTAL
    //============================================================

    _dados.tensao_mV = 0;
    _dados.fonte = FonteVoltagem::INDEFINIDO;
    _dados.valido = false;

    debugErro("VOLTAGEM: FALHA TOTAL");
}

//====================================================================================
// GET
//====================================================================================

const DadosVoltagemSistema& Decisor_Voltagem_Obter()
{
    return _dados;
}