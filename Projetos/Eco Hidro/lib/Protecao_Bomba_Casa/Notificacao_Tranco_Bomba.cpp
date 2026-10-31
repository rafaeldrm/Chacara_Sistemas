//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Notificacao_Tranco_Bomba.h"
#include "DEBUG_MODE.h"

#include "Protocolo_Chacara.h"
#include "Envio_Eventos_LoRa.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Notificacao_Tranco_Bomba_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "NOTIFICACAO TRANCO BOMBA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Notificacao_Tranco_Bomba_Travada()
{
        debugAviso("Enviando Mensagens LoRa Bomba Travada");
    
    LoRa_Enviar_Evento(
        (uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_BOMBA_TRAVADA, 0);
}
