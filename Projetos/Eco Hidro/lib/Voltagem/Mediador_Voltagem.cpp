//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Voltagem.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Voltagem_Inicializa()
{
    debugSetupDivisao(COR::AMARELO, "MEDIADOR VOLTAGEM INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//==========================================================================================================
//                   VOLTAGEM DE CONTROLE DA CARGA (CONSUMINDO ENERGIA) - ACIONAMENTO DAS LUZES PARA CONSUMO
//==========================================================================================================

static void Mediador_Voltagem_Manter_Carga() //14.4V
{

    const auto potencia = POTENCIA::atual();
    const auto bateria = VOLTAGEM::atual();
    const auto casaTorre = CASA_TORRE::atual();

    if (bateria == VOLTAGEM::ESTADO::CHEIA && 
    potencia == POTENCIA::ESTADO::REPOUSO
    && casaTorre != CASA_TORRE::ESTADO::LIBERADO_PARA_O_RESERVATORIO)
    {
        VOLTAGEM::definir(VOLTAGEM::ESTADO::MANTENDO_CARGA);
    }
}

//====================================================================================
//                   CARGA PRIORIDADE (ACIONA PORTAO, BOMBA DA CASA, BOMBA RESERATÓRIO)
//====================================================================================

static void Mediador_Voltagem_Carga_Prioridade()
{
    const auto potencia = POTENCIA::atual();

    if (potencia == POTENCIA::ESTADO::BOMBA_CASA 
     || potencia == POTENCIA::ESTADO::BOMBA_12V  
     || potencia == POTENCIA::ESTADO::PORTAO)
    {
        VOLTAGEM::definir(VOLTAGEM::ESTADO::CARGA_PRIORIDADE);
    }
}

//====================================================================================
//                   VOLTAGEM SAINDO DE CARGA PRIORIDADE
//====================================================================================

static void Mediador_Voltagem_Retorna_Normal()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto potencia = POTENCIA::atual();
    const auto bombaCasa = BOMBA_CASA::atual();

    if (bombaCasa == BOMBA_CASA::ESTADO::REPOUSO
        && (voltagem == VOLTAGEM::ESTADO::CARGA_PRIORIDADE))
    {
        if (potencia == POTENCIA::ESTADO::REPOUSO)
        {
            VOLTAGEM::definir(VOLTAGEM::ESTADO::NORMAL);
        }
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Voltagem_Atualiza()
{
     Mediador_Voltagem_Manter_Carga(); 

    Mediador_Voltagem_Carga_Prioridade();

    Mediador_Voltagem_Retorna_Normal();
}