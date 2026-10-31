//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Interpretador_Mensagens.h"
#include "DEBUG_MODE.h"

#include "Telemetria_Sistema.h"

// SISTEMA Carga Urgência
#include "Controle_Carga_Urgencia.h"


//====================================================================================
//                   RECEBE MENSAGENS DO SISTEMA RESIDENCIAL
//====================================================================================

bool LoRa_Interpretar(const LORA::Pacote& pacote)
{
    if (!LORA::EhParaMim(pacote.sistema, LORA::SISTEMA::SOLAR))
        return false;

    switch (pacote.codigo)
    {
        case (uint8_t)LORA::UNIVERSAL::STATUS_GERAL:
            debugAviso("LORA: Solicitar checklist");
            extern void LoRa_Iniciar_Checklist();
            LoRa_Iniciar_Checklist();
            return true; // 💥 resolve

        case (uint8_t)LORA::SOLAR::CMD_LIGAR_FONTE:
            debugAviso("LORA: Ligar fonte");
            Controle_Carga_Urgencia_Logica_Automatico();
            return true; // 💥 resolve

        default:
            return false; // não resolveu nada importante
    }
}