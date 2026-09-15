//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Recebimento_Ordens_LoRa.h"
#include "DEBUG_MODE.h"

#include "Endereco_LoRa.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Gerenciador_Mensagens.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================



//====================================================================================
//                   SETUP
//====================================================================================

void Recebimento_Ordens_Lora_Inicializa()
{
    // Inicialização de variáveis ou estados, se necessário
}

//====================================================================================
//                   RECEBE MENSAGENS DO SISTEMA RESIDENCIAL
//====================================================================================

bool Recebimento_Ordens_LoRa(const LORA::Pacote& pacote)
{
    if (!LORA::EhParaMim(pacote.sistema, LORA::SISTEMA::ECO_HIDRO))
        return false;

    switch ((LORA::ECO_HIDRO::CODIGO)pacote.codigo)
    {
        case LORA::ECO_HIDRO::CODIGO::CMD_STATUS_GERAL:
          debugAviso("LORA: Solicitar checklist");
          LoRa_Iniciar_Checklist();
          return true; 

       case LORA::ECO_HIDRO::CODIGO::CMD_CICLO_COMPLETO:
          ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::CICLO_COMPLETO);
          return true;

      case LORA::ECO_HIDRO::CODIGO::CMD_MEIO_CICLO:
          ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::MEIO_CICLO);
          return true;

      case LORA::ECO_HIDRO::CODIGO::CMD_DESLIGAR_BOMBA_CASA:
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::DESLIGA);
        return true;

      case LORA::ECO_HIDRO::CODIGO::CMD_NORMAL:
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
        return true;

          //outras mensagens LoRa a serem adicionados aqui

        default:
            return false; // não resolveu nada importante
    }
}


