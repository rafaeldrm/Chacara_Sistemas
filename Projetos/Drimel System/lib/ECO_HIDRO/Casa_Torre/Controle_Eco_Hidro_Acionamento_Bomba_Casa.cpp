//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Eco_Hidro_Acionamento_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "Protocolo_Chacara.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================



//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Eco_Hidro_Acionamento_Bomba_Casa_Inicializa()
{



}

//====================================================================================
//                   ACIONAMENTO DA BOMBA DA CASA COMPLETO
//====================================================================================

void Controle_Eco_Hidro_Acionamento_Bomba_Casa_Ciclo_Completo()
{
    //apenas teste para o LoRa, enviar ordem de ciclo completo para a Eco Hidro
        LoRa_Enviar_Ordem(
        (uint8_t)LORA::ECO_HIDRO::CODIGO::CMD_CICLO_COMPLETO);

}

//====================================================================================
//                   ACIONAMENTO DA BOMBA DA CASA MEIO CICLO
//====================================================================================

void Controle_Eco_Hidro_Acionamento_Bomba_Casa_Meio_Ciclo()
{
    //apenas teste para o LoRa, enviar ordem de ciclo completo para a Eco Hidro
        LoRa_Enviar_Ordem(
        (uint8_t)LORA::ECO_HIDRO::CODIGO::CMD_CICLO_COMPLETO);

}

