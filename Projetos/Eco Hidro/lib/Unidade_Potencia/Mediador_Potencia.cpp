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
        debugSetupDivisao(COR::AMARELO, "MEDIADOR POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   CASA_TORRE
//====================================================================================

static void Mediador_Potencia_Casa_Torre_Ativamento()
{
    const auto potencia = POTENCIA::atual();
    const auto bombaCasa = BOMBA_CASA::atual();

    if (bombaCasa == BOMBA_CASA::ESTADO::LIBERADO
    && (potencia == POTENCIA::ESTADO::BOMBA_12V //interferência caso a casa precise de água rompe contrato com o reservatório
    || potencia == POTENCIA::ESTADO::REPOUSO))
        {
            POTENCIA::definir(POTENCIA::ESTADO::BOMBA_CASA);
        }
}

//====================================================================================
//                   RESERVATÓRIO
//====================================================================================

static void Mediador_Potencia_Reservatorio_Ativamento()
{
    const auto potencia = POTENCIA::atual();
    const auto bombaReservatorio = BOMBA_12V::atual();

    if (potencia == POTENCIA::ESTADO::REPOUSO &&
        bombaReservatorio == BOMBA_12V::ESTADO::LIBERADO)
        {
            POTENCIA::definir(POTENCIA::ESTADO::BOMBA_12V);
        }
}

//====================================================================================
//                   PORTÃO
//====================================================================================

static void Mediador_Potencia_Portao_Ativamento()
{

    const auto portao =  PORTAO::atual();

    if (portao == PORTAO::ESTADO::LIBERADO) //simplesmente tem o poder de "atropelar" todos os outros.
        {
            POTENCIA::definir(POTENCIA::ESTADO::PORTAO);
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

    if (bombaCasa == BOMBA_CASA::ESTADO::REPOUSO &&
        potencia == POTENCIA::ESTADO::BOMBA_CASA)
        {
            POTENCIA::definir(POTENCIA::ESTADO::DESLIGANDO);
        }

//============ BOMBA RESERVATÓRIO ACIONA ======================

    if (bomba12V == BOMBA_12V::ESTADO::REPOUSO &&
    potencia == POTENCIA::ESTADO::BOMBA_12V)
        {
            POTENCIA::definir(POTENCIA::ESTADO::DESLIGANDO);
        }

//============= PORTÃO ACIONA =================================

    if (portao == PORTAO::ESTADO::REPOUSO &&
    potencia == POTENCIA::ESTADO::PORTAO)
        {
            POTENCIA::definir(POTENCIA::ESTADO::DESLIGANDO);
        }
}


//====================================================================================
//                   POTÊNCIA DESLIGADA
//====================================================================================

static void Mediador_Potencia_Desligado()
{
    const auto potencia = POTENCIA::atual();

    if (potencia == POTENCIA::ESTADO::DESLIGANDO)
        {
            POTENCIA::definir(POTENCIA::ESTADO::REPOUSO);
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
