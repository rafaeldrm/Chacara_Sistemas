//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Unidade_Potencia.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"
#include "Eventos_FSM_LoRa/Estados_Eventos_Eco_Hidro.h"

#include "Rastreador_Trocas.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Unidade_Potencia_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR UNIDADE POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   AUXILIAR: APARELHO ESCOLHIDO ESTÁ AGUARDANDO LIBERAÇÃO?
//====================================================================================

static void Mediador_Unidade_Potencia_Aparelho_Ligando()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto bombaReservatorio = BOMBA_12V::atual();
    const auto portao = PORTAO::atual();
    const auto unidadePotencia = UNIDADE_POTENCIA::atual();
    

    //============ BOMBA CASA ==========================

    if (bombaCasa == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO
    && unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
    }

    //============ BOMBA 12V ==========================

      if (bombaReservatorio == BOMBA_12V::ESTAGIOS::AGUARDA_LIBERACAO
    && unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
        
    }

    //============ PORTÃO ==========================

      if (portao == PORTAO::ESTAGIOS::AGUARDA_LIBERACAO
    && unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
    }
}

//====================================================================================
//                   DESLIGANDO INVERSOR, RELE 12V
//====================================================================================

static void Mediador_Unidade_Potencia_Desligando()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto bombaReservatorio = BOMBA_12V::atual();
    const auto portao = PORTAO::atual();
    const auto unidadePotencia = UNIDADE_POTENCIA::atual();
    const auto potencia = POTENCIA::atual();

    // ============= BOMBA CASA ==========================
    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO
        && potencia == POTENCIA::SELECIONADO::REPOUSO)
    {
            UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    }

    // ============= BOMBA 12V ==========================
    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && bombaReservatorio == BOMBA_12V::ESTAGIOS::REPOUSO
        && potencia == POTENCIA::SELECIONADO::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    }

    // ============= PORTÃO ==========================

    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && portao == PORTAO::ESTAGIOS::REPOUSO
        && potencia == POTENCIA::SELECIONADO::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Unidade_Potencia_Atualiza()
{
    Mediador_Unidade_Potencia_Aparelho_Ligando();

    Mediador_Unidade_Potencia_Desligando();
}