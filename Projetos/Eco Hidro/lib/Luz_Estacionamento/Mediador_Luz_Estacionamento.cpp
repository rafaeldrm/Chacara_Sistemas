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

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      ORDEM DRIMEL SYSTEM

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   ACIONA LUZ
//====================================================================================

static void Mediador_Luz_Estacionamento_Ordem_Aciona()
{

    const auto ordemLuzEstacionamento = ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::atual();

    if (ordemLuzEstacionamento == ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::ACIONADO)
    {
        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::ACIONADO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   TEMPORIZA LUZ
//====================================================================================

static void Mediador_Luz_Estacionamento_Temporizacao()
{

    const auto ordemLuzEstacionamento = ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::atual();

    if (ordemLuzEstacionamento == ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::TEMPORIZACAO)
    {
        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::INICIANDO_TEMPORIZACAO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   DESLIGA LUZ
//====================================================================================

static void Mediador_Luz_Estacionamento_Desliga()
{
    const auto ordemLuzEstacionamento = ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::atual();

    if (ordemLuzEstacionamento == ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::DESLIGA)
    {
        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::DESLIGANDO);
        ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::definir(ORDENS_DRIMEL_SYSTEM::LUZ_ESTACIONAMENTO::ORDEM::NENHUM);
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      LUZ ESTACIONAMENTO

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   ACIONAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Luz_Estacionamento_Acionamento_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();

    //================================

        if (voltagem == VOLTAGEM::ESTADO::MANTENDO_CARGA &&          //condição principal
            luzEstacionamento == LUZ_ESTACIONAMENTO::ESTADO::DESLIGADO) //sinal de trava pra evitar ser executado constantemente
        {
            LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::ACIONADO);
        }         
    }

//====================================================================================
//                   DESLIGAMENTO LUZ PARA EFEITO FLUTUAÇÃO BATERIA (SISTEMA SOLAR 12V)
//====================================================================================

static void Mediador_Luz_Estacionamento_Desligamento_Flutuacao_Bateria()
{
    const auto voltagem = VOLTAGEM::atual();
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();

    //================================

        if (voltagem == VOLTAGEM::ESTADO::CARGA_CONSUMIDA &&       
            luzEstacionamento == LUZ_ESTACIONAMENTO::ESTADO::ACIONADO)
        {
            LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::DESLIGANDO);
        }         
    }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      MOTIVOS EXTERNOS LUZ ESTACIONAMENTO

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                    DESLIGAMENTO LUZ (POTENCIA, VOLTAGEM, ORDEM DRIMEL SYSTEM)
//====================================================================================

static void Mediador_Luz_Estacionamento_Desligamento()
{
    const auto potencia = POTENCIA::atual();
    const auto luzEstacionamento = LUZ_ESTACIONAMENTO::atual();
    const auto voltagem = VOLTAGEM::atual();

    //========= VOLTAGEM CRÍTICO =================================================================

    if (voltagem == VOLTAGEM::ESTADO::CRITICO &&
        luzEstacionamento == LUZ_ESTACIONAMENTO::ESTADO::ACIONADO)
    {
        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::DESLIGANDO);
    }

    //======== DESLIGAMENTO DA LUZ DO ESTACIONAMENTO VOLTAGEM BAIXA ======================

    if (voltagem == VOLTAGEM::ESTADO::CRITICO &&
        (luzEstacionamento == LUZ_ESTACIONAMENTO::ESTADO::ACIONADO
        || luzEstacionamento == LUZ_ESTACIONAMENTO::ESTADO::CONSUMINDO
        || luzEstacionamento == LUZ_ESTACIONAMENTO::ESTADO::FORCADO
        || luzEstacionamento == LUZ_ESTACIONAMENTO::ESTADO::TEMPORIZADO))
    {
        LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::DESLIGANDO);
    }

    //=========== DESLIGAMENTO POR CONTA DE APARELHO PESADO QUE VAI FUNCIONAR ===================

        if (luzEstacionamento != LUZ_ESTACIONAMENTO::ESTADO::DESLIGADO
            && (potencia == POTENCIA::ESTADO::BOMBA_CASA
            || potencia == POTENCIA::ESTADO::BOMBA_12V
            || potencia == POTENCIA::ESTADO::PORTAO))
        {
            LUZ_ESTACIONAMENTO::definir(LUZ_ESTACIONAMENTO::ESTADO::DESLIGANDO);
        }         
    }

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Luz_Estacionamento_Atualiza()
{

    Mediador_Luz_Estacionamento_Ordem_Aciona();

    Mediador_Luz_Estacionamento_Temporizacao();

    Mediador_Luz_Estacionamento_Desliga();

    Mediador_Luz_Estacionamento_Acionamento_Flutuacao_Bateria();

    Mediador_Luz_Estacionamento_Desligamento_Flutuacao_Bateria();
    
    Mediador_Luz_Estacionamento_Desligamento();
}

