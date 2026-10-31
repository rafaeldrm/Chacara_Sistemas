//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Voltagem.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Voltagem_Inicializa()
{
    debugSetupDivisao(COR::AMARELO, "MEDIADOR VOLTAGEM INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//==========================================================================================================
//                   VOLTAGEM DE CONTROLE DA CARGA (CONSUMINDO ENERGIA) - ACIONAMENTO DAS LUZES PARA CONSUMO
//==========================================================================================================

static void Mediador_Voltagem_Manter_Carga() //14.4V
{

    const auto bateria = VOLTAGEM::atual();

    if (bateria == VOLTAGEM::LEITURA::MANTENDO_CARGA) {

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::CONSUMINDO);
        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::CONSUMINDO);
    }
}

//==============================================================================================
//                   VOLTAGEM DE CONTROLE DA CARGA (CONSUMO REALIZADO) - DESLIGAMENTO DAS LUZES 
//==============================================================================================

static void Mediador_Voltagem_Consumo_Realizado() //14.2V
{
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();
    const auto luzAcesso = LUZ_ACESSO::atual();
    const auto bateria = VOLTAGEM::atual();

    if 
        (bateria == VOLTAGEM::LEITURA::CARGA_CONSUMIDA
        && (luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::CONSUMINDO
        || luzAcesso == LUZ_ACESSO::SITUACAO::CONSUMINDO)) {

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);
        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Voltagem_Atualiza()
{
    Mediador_Voltagem_Manter_Carga();

    Mediador_Voltagem_Consumo_Realizado();
}