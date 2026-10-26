//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Notificacao_Casa_Torre.h"
#include "DEBUG_MODE.h"

#include "Mudou_Estado.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Envio_Eventos_LoRa.h"
#include "Protocolo_Chacara.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static bool bombaReservatorioAnterior = false;

//====================================================================================
//                   SETUP
//====================================================================================

void Notificacao_Reservatorio_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "NOTIFICACAO RESERVATORIO INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Notificacao_Reservatorio_Bomba_Acao()
{
    const bool bombaOperando =
        BOMBA_CASA::atual() ==
        BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA;

    if (!TRAVA_BOLEANA::MudouEstado(
            bombaOperando,
            bombaReservatorioAnterior))
    {
        return;
    }

    debugVariavel("Enviando Mennsagem LoRa Situacao Bomba :", bombaOperando, NEGRITO, LARANJA, AMARELO | LINHA);

    LoRa_Enviar_Evento(
        (uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_STATUS_BOMBA_CASA,
        bombaOperando ? 1 : 0
    );
}