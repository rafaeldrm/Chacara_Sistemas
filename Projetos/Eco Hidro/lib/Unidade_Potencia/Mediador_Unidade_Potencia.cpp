//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Unidade_Potencia.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

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

    if (bombaCasa == BOMBA_CASA::ESTADO::AGUARDA_LIBERACAO
    && unidadePotencia == UNIDADE_POTENCIA::ESTADO::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTADO::ACIONAMENTO_RELES);
    }

    //============ BOMBA 12V ==========================

      if (bombaReservatorio == BOMBA_12V::ESTADO::AGUARDA_LIBERACAO
    && unidadePotencia == UNIDADE_POTENCIA::ESTADO::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTADO::ACIONAMENTO_RELES);
        
    }

    //============ PORTÃO ==========================

      if (portao == PORTAO::ESTADO::AGUARDA_LIBERACAO
    && unidadePotencia == UNIDADE_POTENCIA::ESTADO::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTADO::ACIONAMENTO_RELES);
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
    if (unidadePotencia == UNIDADE_POTENCIA::ESTADO::LIGADO
        && bombaCasa == BOMBA_CASA::ESTADO::REPOUSO
        && potencia == POTENCIA::ESTADO::REPOUSO)
    {
            UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTADO::DESLIGANDO);
    }

    // ============= BOMBA 12V ==========================
    if (unidadePotencia == UNIDADE_POTENCIA::ESTADO::LIGADO
        && bombaReservatorio == BOMBA_12V::ESTADO::REPOUSO
        && potencia == POTENCIA::ESTADO::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTADO::DESLIGANDO);
    }

    // ============= PORTÃO ==========================

    if (unidadePotencia == UNIDADE_POTENCIA::ESTADO::LIGADO
        && portao == PORTAO::ESTADO::REPOUSO
        && potencia == POTENCIA::ESTADO::REPOUSO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTADO::DESLIGANDO);
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