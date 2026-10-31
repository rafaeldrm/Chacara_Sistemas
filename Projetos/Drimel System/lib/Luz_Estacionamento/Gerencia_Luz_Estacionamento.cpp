//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Luz_Estacionamento.h"
#include "DEBUG_MODE.h"

#include "Controle_Luz_Estacionamento.h"
#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================



//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Luz_Estacionamento_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "GERÊNCIA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Luz_Estacionamento_Atualiza()
{

    switch (LUZ_ESTACIONAMENTO::atual()) {

    //========================================================
    case LUZ_ESTACIONAMENTO::SITUACAO::DESLIGADO:

        // não faz nada
        
    break;
    
       //========================================================
    case LUZ_ESTACIONAMENTO::SITUACAO::DESLIGANDO:

        Controle_Luz_Estacionamento_Desligando();
        
    break;

    //========================================================
    case LUZ_ESTACIONAMENTO::SITUACAO::TEMPORIZADO:

        Controle_Luz_Estacionamento_Temporizado();

    break;

    //========================================================
    case LUZ_ESTACIONAMENTO::SITUACAO::FORCADO:

        Controle_Luz_Estacionamento_Forcado();

    break;

    //========================================================
    case LUZ_ESTACIONAMENTO::SITUACAO::CONSUMINDO:

        Controle_Luz_Estacionamento_Consumindo();

    break;
    
    }

}

