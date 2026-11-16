//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Potencia.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Potencia_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   CASA_TORRE
//====================================================================================

static void Mediador_Potencia_Casa_Torre_Ativamento()
{
    const auto potencia = POTENCIA::atual();
    const auto bombaCasa = BOMBA_CASA::atual();

    if (bombaCasa == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO
    && (potencia == POTENCIA::SELECIONADO::BOMBA_12V
    || potencia == POTENCIA::SELECIONADO::REPOUSO))
        {
            POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_CASA);
        }
}

//====================================================================================
//                   RESERVATÓRIO
//====================================================================================

static void Mediador_Potencia_Reservatorio_Ativamento()
{
    const auto potencia = POTENCIA::atual();
    const auto bombaReservatorio = BOMBA_12V::atual();

    if (potencia == POTENCIA::SELECIONADO::REPOUSO &&
        bombaReservatorio == BOMBA_12V::ESTAGIOS::AGUARDA_LIBERACAO)
        {
            POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_12V);
        }
}

//====================================================================================
//                   PORTÃO
//====================================================================================

static void Mediador_Potencia_Portao_Ativamento()
{
    const auto potencia = POTENCIA::atual();
    const auto portao =  PORTAO::atual();

    if (potencia == POTENCIA::SELECIONADO::REPOUSO &&
        portao == PORTAO::ESTAGIOS::AGUARDA_LIBERACAO)
        {
            POTENCIA::definir(POTENCIA::SELECIONADO::PORTAO);
        }
}

//====================================================================================
//                   DESLIGANDO
//====================================================================================

static void Mediador_Potencia_Desligamento()
{

    const auto potencia = POTENCIA::atual();
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto bomba12V = BOMBA_12V::atual();
    const auto portao = PORTAO::atual();

//=========== BOMBA CASA ACIONA ==============================

    if (bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO &&
        potencia == POTENCIA::SELECIONADO::BOMBA_CASA)
        {
            POTENCIA::definir(POTENCIA::SELECIONADO::DESLIGANDO);
        }

//============ BOMBA RESERVATÓRIO ACIONA ======================

    if (bomba12V == BOMBA_12V::ESTAGIOS::REPOUSO &&
    potencia == POTENCIA::SELECIONADO::BOMBA_12V)
        {
            POTENCIA::definir(POTENCIA::SELECIONADO::DESLIGANDO);
        }

//============= PORTÃO ACIONA =================================

    if (portao == PORTAO::ESTAGIOS::REPOUSO &&
    potencia == POTENCIA::SELECIONADO::PORTAO)
        {
            POTENCIA::definir(POTENCIA::SELECIONADO::DESLIGANDO);
        }
}


//====================================================================================
//                   POTÊNCIA DESLIGADA
//====================================================================================

static void Mediador_Potencia_Desligado()
{
    const auto potencia = POTENCIA::atual();
    const auto unidadePotencia = UNIDADE_POTENCIA::atual();

    if (unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::REPOUSO &&
    potencia == POTENCIA::SELECIONADO::DESLIGANDO)
        {
            POTENCIA::definir(POTENCIA::SELECIONADO::REPOUSO);
        }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Potencia_Atualiza()
{
    Mediador_Potencia_Casa_Torre_Ativamento();

    Mediador_Potencia_Reservatorio_Ativamento();

    Mediador_Potencia_Portao_Ativamento();

    Mediador_Potencia_Desligamento();

    Mediador_Potencia_Desligado();
}
