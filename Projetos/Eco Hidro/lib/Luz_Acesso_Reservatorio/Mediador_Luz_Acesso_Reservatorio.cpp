//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Luz_Estacionamento.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Luz_Estacionamento_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR LUZ ESTACIONAMENTO INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   COORDENA ACIONAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Acionamento_Luz_Estacionamento_Efeito_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();

        if (voltagem == VOLTAGEM::LEITURA::MANTENDO_CARGA &&
            luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO)
        {
            LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::ACIONADO);
        }         
    }

//====================================================================================
//                   COORDENA DESLIGAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Desligamento_Luz_Estacionamento_Efeito_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();

        if (voltagem == VOLTAGEM::LEITURA::CARGA_CONSUMIDA &&
            luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::ACIONADO)
        {
            LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);
        }         
    }

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Luz_Estacionamento_Atualiza()
{
    Mediador_Acionamento_Luz_Estacionamento_Efeito_Flutuacao_Bateria();

    Mediador_Desligamento_Luz_Estacionamento_Efeito_Flutuacao_Bateria();
}
