//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Voltagem.h"
#include "Controle_Voltagem.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Solarius24.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Voltagem_Setup()
{        
            
    debugSetupArquivo();
    debugSetupTexto("GERÊNCIA VOLTAGEM", NEGRITO | AMARELO | LINHA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Voltagem_Loop()
{

       Controle_Voltagem_Atualizar_Tensao();

    switch (VOLTAGEM::atual())
    {
        case VOLTAGEM::NIVEL::NORMAL:
            Controle_Voltagem_Normal();
            break;

        case VOLTAGEM::NIVEL::AUXILIAR:
            Controle_Voltagem_Auxiliar();
            break;

        case VOLTAGEM::NIVEL::EMERGENCIAL:
            Controle_Voltagem_Emergencial();
            break;

        case VOLTAGEM::NIVEL::CRITICO:
            Controle_Voltagem_Critico();
            break;

        case VOLTAGEM::NIVEL::CALAMIDADE_INICIO:
            Controle_Voltagem_Calamidade_Inicio();
            break;

        case VOLTAGEM::NIVEL::CALAMIDADE_COMPLETO:
            Controle_Voltagem_Calamidade_Completo();
            break;

        case VOLTAGEM::NIVEL::TENTATIVA_RECUPERACAO:
            Controle_Voltagem_Tentativa_Recuperar_Sistema();
            break;

        case VOLTAGEM::NIVEL::AUTOMATICO:
            Controle_Voltagem_Automatico();
            break;
        
        case VOLTAGEM::NIVEL::FIM_AUTOMATICO:
            Controle_Voltagem_Fim_Automatico();
            break;

        case VOLTAGEM::NIVEL::RECUPERANDO_CARGA:
            Controle_Voltagem_Recuperando_Carga();
            break;

        case VOLTAGEM::NIVEL::RECUPERADO:
            Controle_Voltagem_Recuperado();
            break;

               default:
        debugErro("ERRO CRÍTICO: Estado desconhecido! Forçando Normal.");
        VOLTAGEM::definir(VOLTAGEM::NIVEL::NORMAL); //ver EstadoSistema.cpp
        break;
    }
}