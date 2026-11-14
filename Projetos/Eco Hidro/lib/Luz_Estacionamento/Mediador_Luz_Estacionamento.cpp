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
        debugSetupDivisao(COR::AMARELO, "MEDIADOR LUZ ESTACIONAMENTO INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   COORDENA ACIONAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Luz_Estacionamento_Acionamento_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();

    //================================

        if (voltagem == VOLTAGEM::LEITURA::MANTENDO_CARGA &&          //condição principal
            luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO) //sinal de trava pra evitar ser executado constantemente
        {

            LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::ACIONADO);

        }         
    }

//====================================================================================
//                   COORDENA DESLIGAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Luz_Estacionamento_Desligamento_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();

    //================================

        if (voltagem == VOLTAGEM::LEITURA::CARGA_CONSUMIDA &&       
            luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::ACIONADO)
        {

            LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);

        }         
    }

//====================================================================================
//                   COORDENA DESLIGAMENTO LUZ (POTENCIA, VOLTAGEM, ORDEM DRIMEL SYSTEM)
//====================================================================================

static void Mediador_Luz_Estacionamento_Desligamento()
{
    const auto potencia = POTENCIA::atual();
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();
    const auto ordemLuzEstacionamento = ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::atual();
    const auto voltagem = VOLTAGEM::atual();

    //========= VOLTAGEM CRÍTICO =================================================================

    if (voltagem == VOLTAGEM::LEITURA::CRITICO &&
        luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::ACIONADO)
    {

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);

    }

    //====== ORDEM DRIMEL SYSTEM DESLIGAMENTO LUZ ESTACIONAMENTO =========================

    if (ordemLuzEstacionamento == ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::DESLIGA)
    {
    
        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::NENHUM);

    }

    //======== DESLIGAMENTO DA LUZ ESTACIONAMENTO VOLTAGEM BAIXA ======================

    if (voltagem == VOLTAGEM::LEITURA::CRITICO &&
        (luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::ACIONADO
        || luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::CONSUMINDO
        || luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::FORCADO
        || luzEstacionamento == LUZ_ESTACIONAMENTO::SITUACAO::TEMPORIZADO))
    {

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);

    }

    //=========== DESLIGAMENTO POR CONTA DE APARELHO PESADO QUE VAI FUNCIONAR ===================

        if (luzEstacionamento != LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO
            && (potencia == POTENCIA::SELECIONADO::BOMBA_CASA
            || potencia == POTENCIA::SELECIONADO::BOMBA_12V
            || potencia == POTENCIA::SELECIONADO::PORTAO))
        {

            LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO);
            
        }         
    }

//====================================================================================
//                   LUZ ESTACIONAMENTO - ACIONAMENTO PELO DRIMEL SYSTEM
//====================================================================================

static void Mediador_Luz_Estacionamento_Acionamento_Drimel_System()
{
    const auto ordemLuzEstacionamento = ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::atual();

    if (ordemLuzEstacionamento == ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::ACIONADO)
    {

        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::ACIONADO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::NENHUM);

    }

}
    
//====================================================================================
//                   LUZ ACESSO RESERVATORIO - FORÇADO PELO DRIMEL SYSTEM
//====================================================================================

 static void Mediador_Luz_Estacionamento_Acionamento_Forcado_Drimel_System()
{
    const auto ordemLuzAcesso = ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::atual();

    //===============================

        if (ordemLuzAcesso == ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::FORCADO)
        {

            LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::SITUACAO::FORCADO);
            ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::NENHUM);

        }         
    }      
    
//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Luz_Estacionamento_Atualiza()
{
    Mediador_Luz_Estacionamento_Acionamento_Flutuacao_Bateria();

    Mediador_Luz_Estacionamento_Desligamento_Flutuacao_Bateria();
    
    Mediador_Luz_Estacionamento_Desligamento();

    Mediador_Luz_Estacionamento_Acionamento_Drimel_System();

    Mediador_Luz_Estacionamento_Acionamento_Forcado_Drimel_System();
}
