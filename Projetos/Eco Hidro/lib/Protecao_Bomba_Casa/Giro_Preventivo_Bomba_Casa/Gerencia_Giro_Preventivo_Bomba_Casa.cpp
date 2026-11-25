//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Giro_Preventivo_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Controle_Giro_Preventivo_Bomba_Casa.h" 

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Giro_Preventivo_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "GERENCIA CONSERVACAO BOMBA CASA ESTADO INICIALIZANDO", COR::MAGENTA, "=", 0);
        GIRO_PREVENTIVO_BOMBA_CASA::definir(GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::REPOUSO);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Giro_Preventivo_Bomba_Casa_Atualiza()
{
    switch (GIRO_PREVENTIVO_BOMBA_CASA::atual()) {

     case GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::REPOUSO:
    //faz nada, serve para quando desligar o módulo não crashar o sistema todo.
     break;

    //=========================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::INICIA_CONTAGEM_COTIDIANO:
          Controle_Giro_Preventivo_Bomba_Casa_Inicia_Contagem_Cotidiano();       
    break;

    //=========================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::VERIFICA_CONTAGEM_COTIDIANO:
        Controle_Giro_Preventivo_Bomba_Casa_Verifica_Contagem_Cotidiano();
    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::CANCELA_CONTAGEM_COTIDIANO:
        Controle_Giro_Preventivo_Bomba_Casa_Cancela_Contagem_Cotidiano();

    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::PREPARA_EXERCICIO_BOMBA:
          Controle_Giro_Preventivo_Bomba_Casa_Prepara_Exercicio_Bomba();
    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::INICIO_EXERCICIO_BOMBA:
        Controle_Giro_Preventivo_Bomba_Casa_Inicio_Exercicio_Bomba();
    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::FIM_EXERCICIO_BOMBA:
        Controle_Giro_Preventivo_Bomba_Casa_Fim_Exercicio_Bomba();
    break;

         //====================================================
     case GIRO_PREVENTIVO_BOMBA_CASA::ESTADO::FINALIZADO:
     // faz nada
    break;

   }
}
