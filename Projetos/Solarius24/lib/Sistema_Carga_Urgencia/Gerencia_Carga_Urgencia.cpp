#include "Gerencia_Carga_Urgencia.h"
#include "Controle_Carga_Urgencia.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Baterias_Setup()
 {

    debugSetupArquivo();
    debugSetupTexto("GERÊNCIA CARGA URGÊNCIA", NEGRITO | AMARELO | LINHA);
    debugVariavel("ESTADO CARGA URGÊNCIA :: ", ESTADO::URGENCIA::texto(ESTADO::URGENCIA::atual()), NEGRITO, VERDE, LARANJA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Carga_Urgencia_Loop() {

    ESTADO::URGENCIA::MODO estadoAtual = ESTADO::URGENCIA::atual();
    ESTADO::URGENCIA::MODO EstadoUrgencia = estadoAtual;

    // 1. Máquina de Estados da Carga Urgência
    switch (ESTADO::URGENCIA::atual()) {

        case ESTADO::URGENCIA::MODO::NORMAL:
            EstadoUrgencia = Controle_Carga_Urgencia_Logica_Normal();
            break;  

        case ESTADO::URGENCIA::MODO::AUTOMATICO:
             EstadoUrgencia = Controle_Carga_Urgencia_Logica_Automatico();
            break; 

        case ESTADO::URGENCIA::MODO::CALAMIDADE:
            EstadoUrgencia = Controle_Carga_Urgencia_Logica_Calamidade();
            break;

               default:
                 debugErro("ERRO CRÍTICO: Estado desconhecido! Forçando Normal.");
               ESTADO::URGENCIA::definir(ESTADO::URGENCIA::MODO::NORMAL); //ver EstadoSistema.cpp
        break;
    }

    if (EstadoUrgencia != estadoAtual)
    {
        ESTADO::URGENCIA::definir(EstadoUrgencia);
    }
}