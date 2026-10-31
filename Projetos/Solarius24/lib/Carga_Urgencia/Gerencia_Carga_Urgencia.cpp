//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Carga_Urgencia.h"
#include "Controle_Carga_Urgencia.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Solarius24.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Baterias_Inicializa()
 {

    debugSetupArquivo();
    debugSetupTexto("GERÊNCIA CARGA URGÊNCIA", NEGRITO | AMARELO | LINHA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Carga_Urgencia_Atualiza()
{

    // 1. Máquina de Estados da Carga Urgência
    switch (URGENCIA::atual()) {

        case URGENCIA::MODO::NORMAL:
            Controle_Carga_Urgencia_Normal();
            break;  

        case URGENCIA::MODO::AUTOMATICO:
            Controle_Carga_Urgencia_Automatico();
            break; 

        case URGENCIA::MODO::CALAMIDADE:
            Controle_Carga_Urgencia_Calamidade();
            break;

               default:
                 debugErro("ERRO CRÍTICO: Estado desconhecido! Forçando Normal.");
            URGENCIA::definir(URGENCIA::MODO::NORMAL); //ver EstadoSistema.cpp
        break;
    }
}