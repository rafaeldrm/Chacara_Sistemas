#include "Gerencia_Voltagem.h"
#include "Controle_Voltagem.h"
#include "DEBUG_MODE.h"


//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Voltagem_Setup()
{        
            
    debugSetupArquivo();
    debugSetupTexto("GERÊNCIA VOLTAGEM", NEGRITO | AMARELO | LINHA);
    debugVariavel("ESTADO VOLTAGEM :: ", ESTADO::VOLTAGEM::texto(ESTADO::VOLTAGEM::atual()), NEGRITO, VERDE, LARANJA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Voltagem_Loop()
{

       Controle_Voltagem_Atualizar_Tensao();

    ESTADO::VOLTAGEM::NIVEL estadoAtual = ESTADO::VOLTAGEM::atual();
    ESTADO::VOLTAGEM::NIVEL EstadoVoltagem = estadoAtual;

    switch (estadoAtual)
    {
        case ESTADO::VOLTAGEM::NIVEL::NORMAL:
            EstadoVoltagem = Controle_Voltagem_Logica_Normal();
            break;

        case ESTADO::VOLTAGEM::NIVEL::AUXILIAR:
            EstadoVoltagem = Controle_Voltagem_Logica_Auxiliar();
            break;

        case ESTADO::VOLTAGEM::NIVEL::EMERGENCIAL:
            EstadoVoltagem = Controle_Voltagem_Logica_Emergencial();
            break;

        case ESTADO::VOLTAGEM::NIVEL::CRITICO:
            EstadoVoltagem = Controle_Voltagem_Logica_Critico();
            break;

        case ESTADO::VOLTAGEM::NIVEL::CALAMIDADE_INICIO:
            EstadoVoltagem = Controle_Voltagem_Logica_Calamidade_Inicio();
            break;

        case ESTADO::VOLTAGEM::NIVEL::CALAMIDADE_COMPLETO:
            EstadoVoltagem = Controle_Voltagem_Logica_Calamidade_Completo();
            break;

        case ESTADO::VOLTAGEM::NIVEL::TENTATIVA_RECUPERACAO:
            EstadoVoltagem = Controle_Voltagem_Logica_Tentativa_Recuperar_Sistema();
            break;

        case ESTADO::VOLTAGEM::NIVEL::AUTOMATICO:
            EstadoVoltagem = Controle_Voltagem_Logica_Automatico();
            break;
        
        case ESTADO::VOLTAGEM::NIVEL::FIM_AUTOMATICO:
            EstadoVoltagem = Controle_Voltagem_Logica_Fim_Automatico();
            break;

        case ESTADO::VOLTAGEM::NIVEL::RECUPERANDO_CARGA:
             EstadoVoltagem = Controle_Voltagem_Logica_Recuperando_Carga();
            break;

        case ESTADO::VOLTAGEM::NIVEL::RECUPERADO:
            EstadoVoltagem = Controle_Voltagem_Logica_Recuperado();
            break;

               default:
        debugErro("ERRO CRÍTICO: Estado desconhecido! Forçando Normal.");
        ESTADO::VOLTAGEM::definir(ESTADO::VOLTAGEM::NIVEL::NORMAL); //ver EstadoSistema.cpp
        break;
    }

    if (EstadoVoltagem != estadoAtual)
    {
        ESTADO::VOLTAGEM::definir(EstadoVoltagem);
    }
}