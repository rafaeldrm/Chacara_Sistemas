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
        debugSetupDivisao(COR::AMARELO, "GERÊNCIA LUZ ACESSO RESERVATÓRIO INICIALIZANDO", COR::MAGENTA, "=", 0);
        LUZ_ACESSO::definir(LUZ_ACESSO::ESTADO::INICIANDO_TEMPORIZACAO);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Luz_Acesso_Reservatorio_Atualiza()
{

    switch (LUZ_ACESSO::atual()) {

    //========================================================
    case LUZ_ACESSO::ESTADO::DESLIGADO:

        // não faz nada
        
    break;
    
      //========================================================
    case LUZ_ACESSO::ESTADO::DESLIGANDO:

        Controle_Luz_Acesso_Reservatorio_Desligando();
        
    break;

    //==========================================================
    case LUZ_ACESSO::ESTADO::INICIANDO_TEMPORIZACAO:

        Controle_Luz_Acesso_Reservatorio_Iniciando_Temporizacao();

    break;

    //========================================================
    case LUZ_ACESSO::ESTADO::TEMPORIZADO:

        Controle_Luz_Acesso_Reservatorio_Temporizado();

    break;

    //========================================================
    case LUZ_ACESSO::ESTADO::FORCADO:

        Controle_Luz_Acesso_Reservatorio_Forcado();

    break;

    //========================================================

    case LUZ_ACESSO::ESTADO::CONSUMINDO:

        Controle_Luz_Acesso_Reservatorio_Consumindo();

    break;

    //========================================================

    case LUZ_ACESSO::ESTADO::ACIONADO:

        Controle_Luz_Acesso_Reservatorio_Acionado();

    break;

    }

}