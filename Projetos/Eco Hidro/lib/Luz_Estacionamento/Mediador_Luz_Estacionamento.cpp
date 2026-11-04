//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Luz_Estacionamento.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"

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
//                   COORDENA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Luz_Estacionamento_Efeito_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();

        if (voltagem == VOLTAGEM::LEITURA::CARGA_COMPLETA)
        {
            if (luzEstacionamento != LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO)
            {
                LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO);
            }
        }
        else
        {
            if (luzEstacionamento != LUZ_ESTACIONAMENTO::SITUACAO::TEMPORIZADO)
            {
                LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::TEMPORIZADO);
            }
        }
    }

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Luz_Estacionamento_Atualiza()
{

}
