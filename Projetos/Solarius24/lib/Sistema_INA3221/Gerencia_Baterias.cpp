//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Baterias.h"
#include "DEBUG_MODE.h"

#include "Controle_Baterias.h"

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Baterias_Inicializa()
{

    debugSetupArquivo();
    debugSetupTexto("GERÊNCIA BATERIAS", NEGRITO | AMARELO | LINHA);
    debugVariavel("ESTADO BATERIA :: ", ESTADO::BATERIA::texto(ESTADO::BATERIA::atual()), NEGRITO, VERDE, LARANJA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Baterias_Atualiza() {

    debugLoopTexto("GERENCIA BATERIAS : ATUALIZA", NENHUM);

    // 2. Estado atual
    ESTADO::BATERIA::SITUACAO estadoAtual = ESTADO::BATERIA::atual();
    ESTADO::BATERIA::SITUACAO EstadoBateria = estadoAtual;

    // 3. Máquina de estados
    switch (estadoAtual)
    {
        //====================================================================================
        //                   NORMAL SISTEMA (INA3221)
        //====================================================================================
        
        
    case ESTADO::BATERIA::SITUACAO::NORMAL:
        
        EstadoBateria = Controle_Baterias_Logica_Normal(); //ver em 1_Controle_Baterias_INA3221.cpp

    break;

        //====================================================================================
        //                   OBSERVA (INA3221)
        //====================================================================================
        

    case ESTADO::BATERIA::SITUACAO::OBSERVA:
        
       EstadoBateria = Controle_Baterias_Logica_Observacao(); //ver em 1_Controle_Baterias_INA3221.cpp

    break;
    

        //====================================================================================
        //                   CALAMIDADE
        //====================================================================================

    case ESTADO::BATERIA::SITUACAO::CALAMIDADE_INICIO:
       
        EstadoBateria = Controle_Baterias_Logica_Calamidade_Inicio(); //ver em 1_Controle_Baterias_INA3221.cpp
    
    break;


    case ESTADO::BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO:

        EstadoBateria = Controle_Baterias_Logica_Calamidade_Disjuntor_Desligado(); //ver em 1_Controle_Baterias_INA3221.cpp
    
    break;


    case ESTADO::BATERIA::SITUACAO::CALAMIDADE_COMPLETO:

        EstadoBateria = Controle_Baterias_Logica_Calamidade_Completo(); //ver em 1_Controle_Baterias.INA3221.cpp
    
    break;

    //====================================================================================
    //                   TENTATIVA DE RECUPERAÇÃO DO SISTEMA
    //====================================================================================

    case ESTADO::BATERIA::SITUACAO::TENTATIVA_RECUPERACAO:

        EstadoBateria = Controle_Baterias_Logica_Tentativa_Recuperar_Sistema(); //ver em 1_Controle_Baterias.INA3221.cpp
    
    break;

    default:

        // Se caiu aqui, estado inválido. Força retorno ao normal ou observa.
        debugErro("ERRO CRÍTICO: Estado desconhecido! Forçando Normal.");
        ESTADO::BATERIA::definir(ESTADO::BATERIA::SITUACAO::NORMAL); //ver EstadoSistema.cpp

    break;

    }
    
    if (EstadoBateria != estadoAtual)
    {
        ESTADO::BATERIA::definir(EstadoBateria);
    }
}




                                                                                                  
                                                                                                  