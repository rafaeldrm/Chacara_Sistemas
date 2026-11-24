//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mediador_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Mediador_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "MEDIADOR BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   PREPARACAO ACIONAMENTO BOMBA CASA (CASA TORRE)
//====================================================================================

static void Mediador_Bomba_Casa_Prepara_Acionamento()
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

static void Mediador_Bomba_Casa_Liga()
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

static void Mediador_Bomba_Casa_Desliga()
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

//                      BOMBA CASA - PROTEÇÃO TÉRMICA

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   DESLIGAMENTO TEMPERATURA BOMBA
//====================================================================================

static void Mediador_Bomba_Casa_Protecao_Termica_Desligamento()
{
      const auto termicoBomba = PROTECAO_TERMICA_BOMBA_CASA::atual();
      const auto bombaCasa = BOMBA_CASA::atual();

      if (bombaCasa == BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA
        && termicoBomba == PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::SUPERAQUECIMENTO)
        {
            BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
        }
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//                      GIRO PREVENTIVO BOMBA - EXERCÍCIO DIÁRIO BOMBA CASA

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//====================================================================================
//                   INICIA EXERCÍCIO BOMBA CASA
//====================================================================================

static void Mediador_Bomba_Casa_Giro_Preventivo_Inicio_Exercicio_Bomba()
{
    const auto giroPreventivo = GIRO_PREVENTIVO_BOMBA_CASA::atual();
    const auto bombaCasa = BOMBA_CASA::atual();
    const auto potencia = POTENCIA::atual();

    if (giroPreventivo == GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::PREPARA_EXERCICIO_BOMBA
        && bombaCasa == BOMBA_CASA::ESTAGIOS::REPOUSO
        && potencia == POTENCIA::SELECIONADO::REPOUSO)
    {
     BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIBERADO);
    }
}

//====================================================================================
//                   FIM EXERCÍCIO BOMBA CASA
//====================================================================================

static void Mediador_Bomba_Casa_Giro_Preventivo_Fim_Exercicio_Bomba()
{
    const auto giroPreventivo = GIRO_PREVENTIVO_BOMBA_CASA::atual();
    const auto bombaCasa = BOMBA_CASA::atual();
 //   const auto casaTorre = CASA_TORRE::atual();

    if (giroPreventivo == GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::FIM_EXERCICIO_BOMBA
        && bombaCasa == BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA)
    {
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
    }
}

//====================================================================================
//                   CANCELA GIRO PREVENTIVO BOMBA CASA
//====================================================================================

static void Mediador_Bomba_Casa_Giro_Preventivo_Cancela_Exercicio()
{
    const auto giroPreventivo = GIRO_PREVENTIVO_BOMBA_CASA::atual();

    if (giroPreventivo == GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::CANCELA_CONTAGEM_COTIDIANO) 
    {
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIBERADO);
    }

}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Bomba_Casa_Atualiza()
{
    Mediador_Bomba_Casa_Prepara_Acionamento();

    Mediador_Bomba_Casa_Liga();

    Mediador_Bomba_Casa_Desliga();

    Mediador_Bomba_Casa_Protecao_Termica_Desligamento();

    Mediador_Bomba_Casa_Giro_Preventivo_Inicio_Exercicio_Bomba();

    Mediador_Bomba_Casa_Giro_Preventivo_Fim_Exercicio_Bomba();

    Mediador_Bomba_Casa_Giro_Preventivo_Cancela_Exercicio();
}
