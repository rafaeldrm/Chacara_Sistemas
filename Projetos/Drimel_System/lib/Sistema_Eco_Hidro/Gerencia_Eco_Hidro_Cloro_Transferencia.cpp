//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Eco_Hidro_Cloro_Transferencia.h"
#include "DEBUG_MODE.h"

#include "Estado_Sistema_Drimel_System.h"
#include "Controle_Eco_Hidro_Cloro_Transferencia.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================



//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Eco_Hidro_Cloro_Transferencia_Inicializa()
{

    ECO_HIDRO::CAIXA_ENTRADA::definir(ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::DESCONHECIDO);

}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Eco_Hidro_Cloro_Transferencia_Atualiza()
{

  Controle_Eco_Hidro_Cloro_Transferencia_Normal();

    switch (ECO_HIDRO::CAIXA_ENTRADA::atual())
    {

        //==========================================================================

        case ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::DESCONHECIDO:
            //não acontece nada aqui
        break;

        //==========================================================================

        case ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::ADICIONAR_CLORO:

        Controle_Eco_Hidro_Cloro_Transferencia_Adicionar_Cloro();

        break;

          //==========================================================================

        case ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::AVISADO_CLORO:

        Controle_Eco_Hidro_Cloro_Transferencia_Avisado_Cloro();

        break;

          //==========================================================================

        case ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::INICIAR_TRANSFERENCIA:

        Controle_Eco_Hidro_Cloro_Transferencia_Iniciar_Transferencia();

        break;

          //==========================================================================

        case ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::AVISADO_TRANSFERENCIA:

        Controle_Eco_Hidro_Cloro_Transferencia_Avisado_Transferencia();

        break;

          //==========================================================================

        case ECO_HIDRO::CAIXA_ENTRADA::SITUACAO::CONCLUIDO:

        Controle_Eco_Hidro_Cloro_Transferencia_Concluido();

        break;

    }


}

