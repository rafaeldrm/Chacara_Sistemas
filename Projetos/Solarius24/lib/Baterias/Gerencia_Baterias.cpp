//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Baterias.h"
#include "DEBUG_MODE.h"

#include "Controle_Baterias.h"
#include "Estados_Sistema_Solarius24.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Baterias_Inicializa()
{

    debugSetupArquivo();
    debugSetupTexto("GERÊNCIA BATERIAS", NEGRITO | AMARELO | LINHA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Baterias_Atualiza() {

    debugLoopTexto("GERENCIA BATERIAS : ATUALIZA", NENHUM);

    // 3. Máquina de estados
    switch (BATERIA::atual())
    {
        //====================================================================================
        //                   NORMAL SISTEMA (INA3221)
        //====================================================================================
        
        
    case BATERIA::SITUACAO::NORMAL:
        
        Controle_Baterias_Normal(); //ver em 1_Controle_Baterias_INA3221.cpp

    break;

        //====================================================================================
        //                   OBSERVA (INA3221)
        //====================================================================================
        

    case BATERIA::SITUACAO::OBSERVA:
        
       Controle_Baterias_Observacao(); //ver em 1_Controle_Baterias_INA3221.cpp

    break;
    

        //====================================================================================
        //                   CALAMIDADE
        //====================================================================================

    case BATERIA::SITUACAO::CALAMIDADE_INICIO:
       
        Controle_Baterias_Calamidade_Inicio(); //ver em 1_Controle_Baterias_INA3221.cpp
    
    break;


    case BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO:

        Controle_Baterias_Calamidade_Disjuntor_Desligado(); //ver em 1_Controle_Baterias_INA3221.cpp
    
    break;


    case BATERIA::SITUACAO::CALAMIDADE_COMPLETO:

        Controle_Baterias_Calamidade_Completo(); //ver em 1_Controle_Baterias.INA3221.cpp
    
    break;

    //====================================================================================
    //                   TENTATIVA DE RECUPERAÇÃO DO SISTEMA
    //====================================================================================

    case BATERIA::SITUACAO::TENTATIVA_RECUPERACAO:

        Controle_Baterias_Tentativa_Recuperar_Sistema(); //ver em 1_Controle_Baterias.INA3221.cpp
    
    break;

    default:

        // Se caiu aqui, estado inválido. Força retorno ao normal ou observa.
        debugErro("ERRO CRÍTICO: Estado desconhecido! Forçando Normal.");
        BATERIA::definir(BATERIA::SITUACAO::NORMAL); //ver EstadoSistema.cpp

    break;

    }
}




                                                                                                  
                                                                                                  