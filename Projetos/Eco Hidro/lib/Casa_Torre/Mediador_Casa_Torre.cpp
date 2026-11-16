//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Casa_Torre.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Estados_Eventos_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Casa_Torre_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR CASA TORRE INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      ORDEM DRIMEL SYSTEM

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   CICLO COMPLETO
//====================================================================================

static void Mediador_Casa_Torre_Ordem_Ciclo_Completo()
{

    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();

    if (ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::CICLO_COMPLETO)
    {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   MEIO CICLO
//====================================================================================

static void Mediador_Casa_Torre_Ordem_Meio_Ciclo()
{
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();

    if (ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::MEIO_CICLO)
    {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   NORMAL
//====================================================================================

static void Mediador_Casa_Torre_Ordem_Normal()
{
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();

    if (ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NORMAL)
    {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::NORMAL);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::REPOUSO);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }

}

//====================================================================================
//                   BLOQUEIO (CASA_TORRE - BOMBA CASA)
//====================================================================================

static void Mediador_Casa_Torre_Ordem_Bloqueio()
{
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();

    if (ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::BLOQUEIA)
    {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::BLOQUEIO);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::BLOQUEIO);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
    }

}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      CASA TORRE - BOMBA CASA

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   PREPARACAO ACIONAMENTO BOMBA CASA
//====================================================================================

static void Mediador_Casa_Torre_Prepara_Acionamento_Bomba()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto potencia = POTENCIA::atual();

//=========== TRAVA PRINCIPAL NÃO DEIXA PASSAR PARA AS PRÓXIMAS CONDIÇÕES ABAIXO ======================

    if (casaTorre == CASA_TORRE::ETAPAS::BLOQUEIO
       || potencia == POTENCIA::SELECIONADO::PORTAO)
    {
        return; //Sistema Bloqueado, ou Portão sendo utilizado: não aciona a bomba d' água.

    }
    
    //=========== ACIONA BOMBEAMENTO CICLO COMPLETO ou MEIO CICLO (Segundo ciclo) ========================

    if (casaTorre == CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA
        || casaTorre == CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA)
    {
        if (bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO
            || bombaCasa == BOMBA_CASA::ESTAGIOS::BLOQUEADO) //Estado apenas para um travamento pra evitar ficar entrando diversas vezes desnecessária.
        {
            BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIBERADO);
        }
    }
}

//====================================================================================
//                   ACIONAMENTO DA BOMBA DA CASA
//====================================================================================

static void Mediador_Casa_Torre_Liga_Bomba()
{
    const auto potencia = POTENCIA::atual();
    const auto unidadePotencia = UNIDADE_POTENCIA::atual();
    const auto bombaCasa = BOMBA_CASA::atual();

    if (potencia == POTENCIA::SELECIONADO::BOMBA_CASA
        && unidadePotencia == UNIDADE_POTENCIA::ESTAGIOS::LIGADO
        && bombaCasa == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO)
    {

         BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIGANDO_BOMBA);

    }
}

//====================================================================================
//                   DESLIGAMENTO DA BOMBA DA CASA
//====================================================================================

static void Mediador_Casa_Torre_Desliga_Bomba()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto casaTorre = CASA_TORRE::atual();
    const auto potencia = POTENCIA::atual();
    
    //=========== BOMBA CASA CONDIÇÃO DE TRAVA PARA DEMAIS IF/ELSES SEGUINTES. SOMENTE BOMBA LIGADA LIBERA ============================

    if (!(bombaCasa == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO 
      || bombaCasa == BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA))
    {
        return;  //se a bomba estiver em qualquer estágio que não seja BOMBA LIGADA, não faz nada.
    }
  
    // =========== SISTEMA BLOQUEIO OU PORTÃO SELECIONADO OU ORDEM DESLIGA ============================

    if (potencia == POTENCIA::SELECIONADO::PORTAO)
    {
       // CASA_TORRE::definir(CASA_TORRE::ETAPAS::BLOQUEIO); //conferir se posso ou não ativar essa opção também.
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::BLOQUEIO);
        return;
    }

    // ====== CASA TORRE AGUARDANDO TRANSFERENCIA OU FINALIZADO ========
    
    if (bombaCasa == BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA
        && (casaTorre == CASA_TORRE::ETAPAS::AGUARDANDO_TRANSFERENCIA_GRAVIDADE
            || casaTorre == CASA_TORRE::ETAPAS::LIBERADO_PARA_O_RESERVATORIO))
    {
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      CASA TORRE - RESERVATÓRIO

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   RESETANDO POSICIONAMENTO DA CASA TORRE
//====================================================================================

static void Mediador_Casa_Torre_Finalizado_Reservatorio()
{
    const auto reservatorio = RESERVATORIO::atual();
    const auto casaTorre = CASA_TORRE::atual();
    
    if (casaTorre == CASA_TORRE::ETAPAS::LIBERADO_PARA_O_RESERVATORIO
        && (reservatorio == RESERVATORIO::TRANSFERENCIA::BLOQUEIO_SEGURANCA
        || reservatorio == RESERVATORIO::TRANSFERENCIA::FINALIZADO))
    {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::NORMAL);
    }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Casa_Torre_Atualiza()
{
        Mediador_Casa_Torre_Ordem_Ciclo_Completo();

        Mediador_Casa_Torre_Ordem_Meio_Ciclo();

        Mediador_Casa_Torre_Ordem_Normal();

        Mediador_Casa_Torre_Ordem_Bloqueio();

        Mediador_Casa_Torre_Prepara_Acionamento_Bomba();

        Mediador_Casa_Torre_Liga_Bomba();

        Mediador_Casa_Torre_Desliga_Bomba();

        Mediador_Casa_Torre_Finalizado_Reservatorio();
}