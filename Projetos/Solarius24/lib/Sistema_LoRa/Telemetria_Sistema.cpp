//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Telemetria_Sistema.h"
#include "LoRa_Transporte.h"
#include "Protocolo_Chacara.h"

//====================================================================================
//                   ENVIA TELEMETRIA PARA O SISTEMA RESIDENCIAL
//====================================================================================

void LoRa_Enviar_Telemetria(uint8_t codigo, uint16_t valor)
{
    LORA::Pacote p = LORA::CriarPacote(
        LORA::SISTEMA::RESIDENCIAL,
        LORA::SISTEMA::SOLAR,
        codigo,
        valor
    );

    LoRa_Enviar_Pacote(p);
}