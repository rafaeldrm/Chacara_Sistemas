//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Giro_Preventivo_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"
#include "Controle_Giro_Preventivo_Bomba_Casa.h" 

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Giro_Preventivo_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "GERENCIA CONSERVACAO BOMBA CASA BOMBEAMENTO INICIALIZANDO", COR::MAGENTA, "=", 0);
        GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::INICIA_CONTAGEM_COTIDIANO);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Giro_Preventivo_Bomba_Casa_Atualiza()
{
    switch (GIRO_PREVENTIVO_BOMBA_CASA::atual()) {

    //=========================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::INICIA_CONTAGEM_COTIDIANO:
          Controle_Giro_Preventivo_Bomba_Casa_Inicia_Contagem_Cotidiano();       
    break;

    //=========================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::VERIFICA_CONTAGEM_COTIDIANO:
        Controle_Giro_Preventivo_Bomba_Casa_Verifica_Contagem_Cotidiano();
    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::CANCELA_CONTAGEM_COTIDIANO:
        Controle_Giro_Preventivo_Bomba_Casa_Cancela_Contagem_Cotidiano();

    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::PREPARA_EXERCICIO_BOMBA:
          Controle_Giro_Preventivo_Bomba_Casa_Prepara_Exercicio_Bomba();
    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::INICIO_EXERCICIO_BOMBA:
        Controle_Giro_Preventivo_Bomba_Casa_Inicio_Exercicio_Bomba();
    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::FIM_EXERCICIO_BOMBA:
        Controle_Giro_Preventivo_Bomba_Casa_Fim_Exercicio_Bomba();
    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::BOMBEAMENTO::FINALIZADO:
     // faz nada
    break;

   }
}
