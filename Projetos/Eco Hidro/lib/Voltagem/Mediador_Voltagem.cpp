//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Voltagem.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
//#include "Estados_Eventos_Eco_Hidro.h"

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

    if (bateria == VOLTAGEM::LEITURA::CHEIA && 
    (potencia == POTENCIA::SELECIONADO::REPOUSO || 
     potencia == POTENCIA::SELECIONADO::DESLIGANDO))
    {
        VOLTAGEM::definir(VOLTAGEM::LEITURA::MANTENDO_CARGA);
    }
}

//====================================================================================
//                   CARGA PRIORIDADE (ACIONA PORTAO, BOMBA DA CASA, BOMBA RESERATÓRIO)
//====================================================================================

static void Mediador_Voltagem_Carga_Prioridade()
{
    const auto potencia = POTENCIA::atual();

    if (potencia == POTENCIA::SELECIONADO::BOMBA_CASA 
     || potencia == POTENCIA::SELECIONADO::BOMBA_12V  
     || potencia == POTENCIA::SELECIONADO::PORTAO)
    {
        VOLTAGEM::definir(VOLTAGEM::LEITURA::CARGA_PRIORIDADE);
    }
}

//====================================================================================
//                   VOLTAGEM SAINDO DE CARGA PRIORIDADE
//====================================================================================

static void Mediador_Voltagem_Retorna_Prioridade_Normal()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto potencia = POTENCIA::atual();

    if (voltagem == VOLTAGEM::LEITURA::CARGA_PRIORIDADE)
    {
        if (potencia == POTENCIA::SELECIONADO::REPOUSO)
        {
            VOLTAGEM::definir(VOLTAGEM::LEITURA::NORMAL);
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

    Mediador_Voltagem_Retorna_Prioridade_Normal();
}