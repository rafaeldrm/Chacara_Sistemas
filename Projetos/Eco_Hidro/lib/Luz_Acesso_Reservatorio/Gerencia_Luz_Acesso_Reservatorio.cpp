//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Luz_Acesso_Reservatorio.h"
#include "DEBUG_MODE.h"

#include "Controle_Luz_Acesso_Reservatorio.h"
#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================



//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Luz_Acesso_Reservatorio_Inicializa()
{
    

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Luz_Acesso_Reservatorio_Atualiza()
{

    switch (LUZ_ACESSO::atual()) {

    //========================================================
    case LUZ_ACESSO::SITUACAO::DESLIGADO:

        // não faz nada
        
    break;
    
      //========================================================
    case LUZ_ACESSO::SITUACAO::DESLIGANDO:

        Controle_Luz_Acesso_Reservatorio_Desligando();
        
    break;

    //========================================================
    case LUZ_ACESSO::SITUACAO::TEMPORIZADO:

        Controle_Luz_Acesso_Reservatorio_Temporizado();

    break;

    //========================================================
    case LUZ_ACESSO::SITUACAO::FORCADO:

        Controle_Luz_Acesso_Reservatorio_Forcado();

    break;

        //========================================================
    case LUZ_ACESSO::SITUACAO::CONSUMINDO:

        Controle_Luz_Acesso_Reservatorio_Consumindo();

    break;

    
    }

}

