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

        if (voltagem == VOLTAGEM::LEITURA::MANTENDO_CARGA &&          //condição principal
            luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::DESLIGADO) //sinal de trava pra evitar ser executado constantemente
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
    const auto voltagem = VOLTAGEM::atual();

    //========= VOLTAGEM CRÍTICO =================================================================

    if (voltagem == VOLTAGEM::LEITURA::CRITICO &&
        luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::ACIONADO)
    {

        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);

    }

    //====== ORDEM DRIMEL SYSTEM DESLIGAMENTO LUZ ACESSO AO RESERVATÓRIO =========================

    if (ordemLuzAcesso == ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::DESLIGA)
    {
    
        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::NENHUM);

    }

    //======== DESLIGAMENTO DA LUZ DE ACESSO AO RESERVATÓRIO VOLTAGEM BAIXA ======================

    if (voltagem == VOLTAGEM::LEITURA::CRITICO &&
        (luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::ACIONADO
        || luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::CONSUMINDO
        || luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::FORCADO
        || luzAcessoReservatorio == LUZ_ACESSO::SITUACAO::TEMPORIZADO))
    {

        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);

    }

    //=========== DESLIGAMENTO POR CONTA DE APARELHO PESADO QUE VAI FUNCIONAR ===================

        if (luzAcessoReservatorio != LUZ_ACESSO::SITUACAO::DESLIGADO
            && (potencia == POTENCIA::SELECIONADO::BOMBA_CASA
            || potencia == POTENCIA::SELECIONADO::BOMBA_12V
            || potencia == POTENCIA::SELECIONADO::PORTAO))
        {

            LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::DESLIGANDO);
            
        }         
    }

//====================================================================================
//                   LUZ ACESSORESERVATÓRIO - ACIONAMENTO PELO DRIMEL SYSTEM
//====================================================================================

static void Mediador_Luz_Acesso_Reservatorio_Acionamento_Drimel_System()
{
    const auto ordemLuzAcesso = ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::atual();

    if (ordemLuzAcesso == ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::ACIONADO)
    {

        LUZ_ACESSO::definir(LUZ_ACESSO::SITUACAO::ACIONADO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ACESSO::ORDEM::NENHUM);

    }

}
    
    
//====================================================================================
//                   LUZ ACESSO RESERVATORIO - FORÇADO PELO DRIMEL SYSTEM
//====================================================================================

 static void Mediador_Luz_Acesso_Reservatorio_Acionamento_Forcado_Drimel_System()
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

    Mediador_Luz_Acesso_Reservatorio_Acionamento_Drimel_System();

    Mediador_Luz_Acesso_Reservatorio_Acionamento_Forcado_Drimel_System();
}
