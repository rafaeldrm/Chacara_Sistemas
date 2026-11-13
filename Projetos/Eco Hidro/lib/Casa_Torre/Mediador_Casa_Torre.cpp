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

//====================================================================================
//                   ORDEM DRIMEL SYSTEM - CICLO COMPLETO (CASA TORRE)
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
//                   ORDEM DRIMEL SYSTEM - MEIO CICLO (CASA TORRE)
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
//                   COORDENA PREPARACAO ACIONAMENTO BOMBA CASA (CASA TORRE)
//====================================================================================

static void Mediador_Casa_Torre_Prepara_Acionamento_Bomba()
{
    const auto casaTorre = CASA_TORRE::atual();
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto sistemaModo = SISTEMA_ECO_HIDRO::atual();
    const auto potencia = POTENCIA::atual();


    if (sistemaModo == SISTEMA_ECO_HIDRO::MODO::BLOQUEIO
       || potencia == POTENCIA::SELECIONADO::PORTAO)
    {
        return; //Sistema Bloqueado, ou Portão sendo utilizado: não aciona a bomba d' água.

    }
    
    //================================

    if (casaTorre == CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA
        || casaTorre == CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA)
    {
        if (bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO) //Estado apenas para um travamento pra evitar ficar entrando diversas vezes desnecessária.
        {
            BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIBERADO);
        }
    }
}

//====================================================================================
//                   COORDENA ACIONAMENTO DA BOMBA DA CASA
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
//                   COORDENA DESLIGAMENTO DA BOMBA DA CASA
//====================================================================================

static void Mediador_Casa_Torre_Desliga_Bomba()
{
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto casaTorre = CASA_TORRE::atual();
    const auto sistemaModo = SISTEMA_ECO_HIDRO::atual();
    const auto potencia = POTENCIA::atual();
    const auto ordemCasaTorre = ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual();
    
    //=========== BOMBA CASA CONDIÇÃO DE TRAVA PARA DEMAIS IF/ELSES SEGUINTES. SOMENTE BOMBA LIGADA LIBERA ============================

    if (!(bombaCasa == BOMBA_CASA::ESTAGIOS::AGUARDA_LIBERACAO 
      || bombaCasa == BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA))
    {
        return;  //se a bomba estiver em qualquer estágio que não seja BOMBA LIGADA, não faz nada.
    }
  
    // =========== SISTEMA BLOQUEIO OU PORTÃO SELECIONADO OU ORDEM DESLIGA ============================

    if (sistemaModo == SISTEMA_ECO_HIDRO::MODO::BLOQUEIO
        || potencia == POTENCIA::SELECIONADO::PORTAO
        || ordemCasaTorre == ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::DESLIGA)
    {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::FINALIZADO);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
    }

    // ====== CASA TORRE AGUARDANDO TRANSFERENCIA OU FINALIZADO ========
    
    if (bombaCasa == BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA
        && (casaTorre == CASA_TORRE::ETAPAS::AGUARDANDO_TRANSFERENCIA_GRAVIDADE
            || casaTorre == CASA_TORRE::ETAPAS::FINALIZADO))
    {
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
    }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Casa_Torre_Atualiza()
{
        Mediador_Casa_Torre_Ordem_Ciclo_Completo();

        Mediador_Casa_Torre_Ordem_Meio_Ciclo();

        Mediador_Casa_Torre_Prepara_Acionamento_Bomba();

        Mediador_Casa_Torre_Liga_Bomba();

        Mediador_Casa_Torre_Desliga_Bomba();
}