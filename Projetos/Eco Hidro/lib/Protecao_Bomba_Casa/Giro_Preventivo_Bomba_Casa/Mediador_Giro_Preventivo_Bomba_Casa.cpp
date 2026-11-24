//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Giro_Preventivo_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h" 

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Giro_Preventivo_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR GIRO PREVENTIVO BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   INICIA CONTAGEM COTIDIANO BOMBA CASA
//====================================================================================

static void Mediador_Giro_Preventivo_Bomba_Casa_Prepara_Exercicio()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto bomba12v = BOMBA_12V::atual();
    const auto giroPreventivo = GIRO_PREVENTIVO_BOMBA_CASA::atual();

    if(bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO
    && bomba12v == BOMBA_12V::ESTAGIOS::REPOUSO
    && casaTorre == CASA_TORRE::ETAPAS::NORMAL
    && giroPreventivo == GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::FINALIZADO)
    {
        GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::INICIA_CONTAGEM_COTIDIANO);
    } 
}

//====================================================================================
//                   CANCELA CONTAGEM COTIDIANO - BOMBA CASA
//====================================================================================

static void Mediador_Giro_Preventivo_Bomba_Casa_Cancela_Contagem_Cotidiano()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto giroPreventivo = GIRO_PREVENTIVO_BOMBA_CASA::atual();
    
    if (casaTorre == CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA
        && (giroPreventivo != GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::FINALIZADO
        && giroPreventivo != GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::CANCELA_CONTAGEM_COTIDIANO))
    {
        GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::CANCELA_CONTAGEM_COTIDIANO);
    }

}

//====================================================================================
//                   INICIO EXERCÍCIO - BOMBA CASA
//====================================================================================

static void Mediador_Giro_Preventivo_Bomba_Casa_Inicio_Exercicio()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto giroPreventivo = GIRO_PREVENTIVO_BOMBA_CASA::atual();

    if (bombaCasa == BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA
        && giroPreventivo == GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::PREPARA_EXERCICIO_BOMBA) 
    {
        GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::INICIO_EXERCICIO_BOMBA);
    }

}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Giro_Preventivo_Bomba_Casa_Atualiza()
{
        Mediador_Giro_Preventivo_Bomba_Casa_Prepara_Exercicio();

        Mediador_Giro_Preventivo_Bomba_Casa_Cancela_Contagem_Cotidiano();

        Mediador_Giro_Preventivo_Bomba_Casa_Inicio_Exercicio();
}
