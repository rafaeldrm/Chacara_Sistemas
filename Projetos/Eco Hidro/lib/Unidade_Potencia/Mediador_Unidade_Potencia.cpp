//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Unidade_Potencia.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Unidade_Potencia_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR UNIDADE POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 0);
}


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      ORDEM DRIMEL SYSTEM

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   ACIONAMENTO FORÇADO - POSSÍVEL TRAVAMENTO
//====================================================================================

static void Mediador_Unidade_Potencia_Acionamento()
{
    const auto ordemUnidadePotencia = ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::atual();

    if (ordemUnidadePotencia == ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::ORDEM::ACIONA)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
        ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::definir(ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   DESLIGAMENTO FORÇADO - POSSÍVEL TRAVAMENTO
//====================================================================================

static void Mediador_Unidade_Potencia_Desligamento()
{
    const auto ordemUnidadePotencia = ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::atual();

    if (ordemUnidadePotencia == ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::ORDEM::DESLIGA)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
        ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::definir(ORDENS_DRIMEL_SYSTEM::UNIDADE_POTENCIA::ORDEM::NENHUM);
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      UNIDADE POTENCIA

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   AUXILIAR: APARELHO ESCOLHIDO ESTÁ AGUARDANDO LIBERAÇÃO?
//====================================================================================

static void Mediador_Unidade_Potencia_Aparelho_Aguardando_Liberacao()
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
        && potencia == POTENCIA::SELECIONADO::BOMBA_CASA)
    {
            UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    }

    // ============= BOMBA 12V ==========================
    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && bombaReservatorio == BOMBA_12V::ESTAGIOS::REPOUSO
        && potencia == POTENCIA::SELECIONADO::BOMBA_12V)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    }

    // ============= PORTÃO ==========================

    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && portao == PORTAO::ESTAGIOS::REPOUSO
        && potencia == POTENCIA::SELECIONADO::PORTAO)
    {
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    }
    
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Unidade_Potencia_Atualiza()
{
    Mediador_Unidade_Potencia_Acionamento();

    Mediador_Unidade_Potencia_Desligamento();

    Mediador_Unidade_Potencia_Aparelho_Aguardando_Liberacao();

    Mediador_Unidade_Potencia_Desligando();
}