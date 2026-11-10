//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Luz_Acesso_Reservatorio.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Luz_Acesso_Reservatorio_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR LUZ ACESSO RESERVATORIO INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   COORDENA ACIONAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Acionamento_Luz_Acesso_Reservatorio_Efeito_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzAcessoReservatorio = LUZ_ACESSO::atual();

        if (voltagem == VOLTAGEM::LEITURA::MANTENDO_CARGA &&
            luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::DESLIGADO)
        {
            LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::ACIONADO);
        }         
    }

//====================================================================================
//                   COORDENA DESLIGAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Desligamento_Luz_Acesso_Reservatorio_Efeito_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzAcessoReservatorio = LUZ_ACESSO::atual();

        if (voltagem == VOLTAGEM::LEITURA::CARGA_CONSUMIDA &&
            luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::ACIONADO)
        {
            LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);
        }         
    }

//====================================================================================
//                   COORDENA DESLIGAMENTO LUZ PARA ACIONAMENTO RELE POTENCIA (BOMBA CASA, BOMBA 12V, PORTÃO)
//====================================================================================

static void Mediador_Desligamento_Luz_Acesso_Reservatorio_Acionamento_Rele_Potencia()
{
    const auto potencia = POTENCIA::atual();
    const auto luzAcessoReservatorio = LUZ_ACESSO::atual();

        if (luzAcessoReservatorio != LUZ_ACESSO::SITUACAO::DESLIGADO
            && (potencia == POTENCIA::SELECIONADO::BOMBA_CASA
            || potencia == POTENCIA::SELECIONADO::BOMBA_12V
            || potencia == POTENCIA::SELECIONADO::PORTAO))
        {
            LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);
        }         
    }
    
//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Luz_Acesso_Reservatorio_Atualiza()
{
    Mediador_Acionamento_Luz_Acesso_Reservatorio_Efeito_Flutuacao_Bateria();

    Mediador_Desligamento_Luz_Acesso_Reservatorio_Efeito_Flutuacao_Bateria();
    
    Mediador_Desligamento_Luz_Acesso_Reservatorio_Acionamento_Rele_Potencia();
}
