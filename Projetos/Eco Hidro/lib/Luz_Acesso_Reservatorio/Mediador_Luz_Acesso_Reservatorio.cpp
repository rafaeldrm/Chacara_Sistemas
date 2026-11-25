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

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      ORDEM DRIMEL SYSTEM

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   ACIONA LUZ
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Ordem_Aciona()
{

    const auto ordemLuzReservatorio = ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::atual();

    if (ordemLuzReservatorio == ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::ACIONADO)
    {
        LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::ACIONADO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   TEMPORIZA LUZ
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Temporizacao()
{

    const auto ordemLuzReservatorio = ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::atual();

    if (ordemLuzReservatorio == ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::TEMPORIZACAO)
    {
        LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::INICIANDO_TEMPORIZACAO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   DESLIGA LUZ
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Desliga()
{
    const auto ordemLuzReservatorio = ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::atual();

    if (ordemLuzReservatorio == ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::DESLIGA)
    {
        LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::DESLIGANDO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::NENHUM);
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      LUZ ACESSO RESERVATÓRIO

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   ACIONAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Acionamento_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzAcessoReservatorio = LUZ_ACESSO::atual();

    //================================

        if (voltagem == VOLTAGEM::ESTADO::MANTENDO_CARGA &&          //condição principal
            luzAcessoReservatorio == LUZ_ACESSO::ESTADO::DESLIGADO) //sinal de trava pra evitar ser executado constantemente
        {
            LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::ACIONADO);
        }         
    }

//====================================================================================
//                   DESLIGAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Desligamento_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzAcessoReservatorio = LUZ_ACESSO::atual();

    //================================

        if (voltagem == VOLTAGEM::ESTADO::CARGA_CONSUMIDA &&       
            luzAcessoReservatorio == LUZ_ACESSO::ESTADO::ACIONADO)
        {
            LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::DESLIGANDO);
        }         
    }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      MOTIVOS EXTERNOS LUZ ACESSO RESERVATÓRIO

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                    DESLIGAMENTO LUZ (POTENCIA, VOLTAGEM, ORDEM DRIMEL SYSTEM)
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Desligamento()
{
    const auto potencia = POTENCIA::atual();
    const auto luzAcessoReservatorio = LUZ_ACESSO::atual();
    const auto voltagem = VOLTAGEM::atual();

    //========= VOLTAGEM CRÍTICO =================================================================

    if (voltagem == VOLTAGEM::ESTADO::CRITICO &&
        luzAcessoReservatorio == LUZ_ACESSO::ESTADO::ACIONADO)
    {
        LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::DESLIGANDO);
    }

    //======== DESLIGAMENTO DA LUZ DE ACESSO AO RESERVATÓRIO VOLTAGEM BAIXA ======================

    if (voltagem == VOLTAGEM::ESTADO::CRITICO &&
        (luzAcessoReservatorio == LUZ_ACESSO::ESTADO::ACIONADO
        || luzAcessoReservatorio == LUZ_ACESSO::ESTADO::CONSUMINDO
        || luzAcessoReservatorio == LUZ_ACESSO::ESTADO::FORCADO
        || luzAcessoReservatorio == LUZ_ACESSO::ESTADO::TEMPORIZADO))
    {
        LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::DESLIGANDO);
    }

    //=========== DESLIGAMENTO POR CONTA DE APARELHO PESADO QUE VAI FUNCIONAR ===================

        if (luzAcessoReservatorio != LUZ_ACESSO::ESTADO::DESLIGADO
            && (potencia == POTENCIA::ESTADO::BOMBA_CASA
            || potencia == POTENCIA::ESTADO::BOMBA_12V
            || potencia == POTENCIA::ESTADO::PORTAO))
        {
            LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::DESLIGANDO);
        }         
    }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Luz_Acesso_Reservatorio_Atualiza()
{
    Mediador_Luz_Acesso_Reservatorio_Ordem_Aciona();

    Mediador_Luz_Acesso_Reservatorio_Temporizacao();

    Mediador_Luz_Acesso_Reservatorio_Desliga();

    Mediador_Luz_Acesso_Reservatorio_Acionamento_Flutuacao_Bateria();

    Mediador_Luz_Acesso_Reservatorio_Desligamento_Flutuacao_Bateria();
    
    Mediador_Luz_Acesso_Reservatorio_Desligamento();
}
