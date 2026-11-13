//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Luz_Acesso_Reservatorio.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Luz_Acesso_Reservatorio_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR LUZ ACESSO RESERVATORIO INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   COORDENA ACIONAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Acionamento_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzAcessoReservatorio = LUZ_ACESSO::atual();

    //================================

        if (voltagem == VOLTAGEM::LEITURA::MANTENDO_CARGA &&
            luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::DESLIGADO)
        {

            LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::ACIONADO);

        }         
    }

//====================================================================================
//                   COORDENA DESLIGAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Desligamento_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzAcessoReservatorio = LUZ_ACESSO::atual();

    //================================

        if (voltagem == VOLTAGEM::LEITURA::CARGA_CONSUMIDA &&
            luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::ACIONADO)
        {

            LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);

        }         
    }

//====================================================================================
//                   COORDENA DESLIGAMENTO LUZ (POTENCIA, VOLTAGEM, ORDEM DRIMEL SYSTEM)
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Desligamento()
{
    const auto potencia = POTENCIA::atual();
    const auto luzAcessoReservatorio = LUZ_ACESSO::atual();
    const auto ordemLuzAcesso = ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::atual();
    const auto bateria = VOLTAGEM::atual();

    //================================

    if (ordemLuzAcesso == ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::DESLIGA){
    
        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::NENHUM);

    }

    //================================

    if (bateria == VOLTAGEM::LEITURA::CRITICO &&
        luzAcessoReservatorio != LUZ_ACESSO::SITUACAO::DESLIGADO)
    {

        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);

    }

    //================================

        if (luzAcessoReservatorio != LUZ_ACESSO::SITUACAO::DESLIGADO
            && (potencia == POTENCIA::SELECIONADO::BOMBA_CASA
            || potencia == POTENCIA::SELECIONADO::BOMBA_12V
            || potencia == POTENCIA::SELECIONADO::PORTAO))
        {

            LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);
            
        }         
    }

//====================================================================================
//                   LUZ ACESSO RESERVATORIO - FORÇADO PELO DRIMEL SYSTEM
//====================================================================================

 static void Mediador_Luz_Acesso_Reservatorio_Acionamento_Forcado()
{
    const auto ordemLuzAcesso = ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::atual();

    //===============================

        if (ordemLuzAcesso == ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::FORCADO)
        {

            LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::FORCADO);
            ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::NENHUM);

        }         
    }      
    
//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Luz_Acesso_Reservatorio_Atualiza()
{
    Mediador_Luz_Acesso_Reservatorio_Acionamento_Flutuacao_Bateria();

    Mediador_Luz_Acesso_Reservatorio_Desligamento_Flutuacao_Bateria();
    
    Mediador_Luz_Acesso_Reservatorio_Desligamento();

    Mediador_Luz_Acesso_Reservatorio_Acionamento_Forcado();
}
