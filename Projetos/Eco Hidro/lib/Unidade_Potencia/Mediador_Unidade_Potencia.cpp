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
        debugSetupDivisao(COR::AMARELO, "MEDIADOR UNIDADE POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   AUXILIAR: APARELHO ESCOLHIDO ESTÁ AGUARDANDO LIBERAÇÃO?
//====================================================================================

static void Mediador_Unidade_Potencia_Aparelho_Aguardando_Liberacao()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto bombaReservatorio = BOMBA_12V::atual();
    // const auto portao = PORTAO::atual(); //não tem FSM ainda.
    const auto potencia = POTENCIA::atual();
    

    //============ BOMBA CASA ==========================

    if (bombaCasa == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO
        && potencia == POTENCIA::SELECIONADO::REPOUSO)
    {
        POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_CASA);
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
        VOLTAGEM::definir(VOLTAGEM::LEITURA::CARGA_PRIORIDADE);
    }

    //============ BOMBA 12V ==========================

    if (bombaReservatorio == BOMBA_12V::ESTAGIOS::AGUARDA_LIBERACAO
        && potencia == POTENCIA::SELECIONADO::REPOUSO
        && bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO) //bomba casa trava especial (prioridade)
    {
        POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_12V);
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
    }

    //============ PORTÃO ==========================
/*
    if (PORTAO::atual() == PORTAO::ESTAGIOS::AGUARDA_LIBERACAO
        && potencia == POTENCIA::SELECIONADO::REPOUSO)
    {
        POTENCIA::definir(POTENCIA::SELECIONADO::PORTAO);
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES);
    }
        */
}

//====================================================================================
//                   DESLIGANDO INVERSOR, RELE 12V
//====================================================================================

static void Mediador_Unidade_Potencia_Desligando()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto bombaReservatorio = BOMBA_12V::atual();
  //  const auto portao = PORTAO::atual();
    const auto unidadePotencia = UNIDADE_POTENCIA::atual();
    const auto potencia = POTENCIA::atual();

    // ============= BOMBA CASA ==========================
    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO
        && potencia == POTENCIA::SELECIONADO::BOMBA_CASA)
    {
        POTENCIA::definir(POTENCIA::SELECIONADO::REPOUSO);
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    }

    // ============= BOMBA 12V ==========================
    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && bombaReservatorio == BOMBA_12V::ESTAGIOS::REPOUSO
        && potencia == POTENCIA::SELECIONADO::BOMBA_12V)
    {
        POTENCIA::definir(POTENCIA::SELECIONADO::REPOUSO);
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    }

    // ============= PORTÃO ==========================
/*    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && portao == PORTAO::ESTAGIOS::REPOUSO
        && potencia == POTENCIA::SELECIONADO::PORTAO)
    {
        POTENCIA::definir(POTENCIA::SELECIONADO::REPOUSO);
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO);
    }
    */
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Unidade_Potencia_Atualiza()
{
    Mediador_Unidade_Potencia_Aparelho_Aguardando_Liberacao();

    Mediador_Unidade_Potencia_Desligando();
}