//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Unidade_Potencia.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Controle_Unidade_Potencia.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Unidade_Potencia_Inicializa()
{
    debugSetupDivisao(COR::AMARELO, "GERENCIA UNIDADE POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Unidade_Potencia_Atualiza()
{

    switch (UNIDADE_POTENCIA::atual())
    {
        //=============================================================

    case UNIDADE_POTENCIA::ESTAGIOS::REPOUSO:
        // Nada a fazer, o sistema está em repouso
        break;

        //=============================================================

    case UNIDADE_POTENCIA::ESTAGIOS::ACIONAMENTO_RELES:

        Controle_Unidade_Potencia_Acionamento_Reles();

        break;

        //=============================================================

    case UNIDADE_POTENCIA::ESTAGIOS::AGUARDANDO_ESTABILIZAR:

        Controle_Unidade_Potencia_Aguardando_Estabilizar();

        break;

        //=============================================================

    case UNIDADE_POTENCIA::ESTAGIOS::LIGADO:
        // Código para o estágio de ligado
        break;

        //=============================================================

    case UNIDADE_POTENCIA::ESTAGIOS::DESLIGANDO:

        Controle_Unidade_Potencia_Desligando();
        break;

        //=============================================================

    default:
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::REPOUSO);
        // Código para lidar com um estado desconhecido
        break;
    }
}