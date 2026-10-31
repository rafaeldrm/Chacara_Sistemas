//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Climatizacao_MPPT.h"
#include "DEBUG_MODE.h"
#include "Estados_Modulos.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static ClimatizacaoMPPT _dados;

// Histerese (valores *10)
static constexpr uint16_t TEMPERATURA_LIGAR    = 350;
static constexpr uint16_t TEMPERATURA_DESLIGAR = 260;


//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Climatizacao_MPPT_Inicializa()
{
   

    _dados.cooler_ligado = true;   // fail-safe inicial
    _dados.controle_ativo = false;

 

}

//====================================================================================
//                   LOOP
//====================================================================================

void Controle_Climatizacao_MPPT_Atualiza(
    const DadosLeituraDHT11& leitura,
    const StatusDiagnosticoDHT11& status)
{
    uint16_t temperatura_usada = 0;

    //===============================================================================
    // FAIL-SAFE: SENSOR EM ERRO
    //===============================================================================

    if (status.estado == SENSOR::ERRO)
    {
        _dados.controle_ativo = false;
        _dados.cooler_ligado = true;
        return;
    }

    //===============================================================================
    // SEGURANÇA: LEITURA NÃO FINALIZADA
    //===============================================================================

    if (!leitura.leitura_finalizada)
    {
        _dados.controle_ativo = false;
        _dados.cooler_ligado = true;
        return;
    }

    _dados.controle_ativo = true;

    //===============================================================================
    // SELEÇÃO DA TEMPERATURA
    //===============================================================================

    if (status.estado == SENSOR::OK)
    {
        temperatura_usada = leitura.temperatura_media_atual;
    }
    else // INSTÁVEL
    {
        temperatura_usada = leitura.temperatura_antiga;
    }

    //===============================================================================
    // HISTERESSE
    //===============================================================================

    bool deve_ligar    = (temperatura_usada >= TEMPERATURA_LIGAR);
    bool deve_desligar = (temperatura_usada <= TEMPERATURA_DESLIGAR);

    if (deve_ligar)
    {
        _dados.cooler_ligado = true;
    }
    else if (deve_desligar)
    {
        _dados.cooler_ligado = false;
    }

    // Meio da faixa → mantém estado atual
}

//====================================================================================
//                   ACESSO AOS DADOS
//====================================================================================

const ClimatizacaoMPPT& Controle_Climatizacao_MPPT_ObterEstado()
{
    return _dados;
}