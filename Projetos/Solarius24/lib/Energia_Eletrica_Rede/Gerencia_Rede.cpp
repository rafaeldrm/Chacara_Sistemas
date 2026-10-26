//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Rede.h"
#include "DEBUG_MODE.h"

#include "Controle_Rede.h"
#include "Estados_Sistema_Solarius24.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Rede_Inicializa() 
{

    debugSetupArquivo();
    debugSetupTexto("GERÊNCIA REDE", NEGRITO | AMARELO | LINHA);
    debugVariavel("ESTADO REDE :: ", ::texto(REDE::atual()), NEGRITO, VERDE, LARANJA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Rede_Atualiza() 
{

    // 1. Máquina de Estados da Rede Elétrica
    switch (REDE::atual())
    {

        case REDE::STATUS::DORMINDO:
        // Não faz nada, o monitoramento está inativo.
        break;

        //========================================================

        case REDE::STATUS::VERIFICANDO_5S:
            Controle_Rede_Verificando_5segundos();
        break;

        //========================================================

        case REDE::STATUS::AUSENTE:
            Controle_Rede_Ausente();
        break;

        //========================================================

        case REDE::STATUS::AGUARDANDO_5MIN:
            Controle_Rede_Aguardando_5minutos();
        break;

        //========================================================

        case REDE::STATUS::INSTAVEL_15MIN:
            Controle_Rede_Instavel_15minutos();
        break;

        //========================================================

        case REDE::STATUS::MONITORANDO_FONTE:
            Controle_Rede_Monitorando_Fonte();
        break;

        //========================================================

        case REDE::STATUS::CARGA_FINALIZADA:
            Controle_Rede_Carga_Finalizada();
        break;
        
        //========================================================

        case REDE::STATUS::CALAMIDADE:
            Controle_Rede_Calamidade();
        break;

        //========================================================
                
    default:
        debugErro("ERRO CRÍTICO: Estado desconhecido! Forçando Dormindo.");
         REDE::definir(REDE::STATUS::DORMINDO); //ver EstadoSistema.cpp
        break;
    }
}