//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Notificacao_Reservatorio.h"
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
    const bool bombaReservatorioOperando =
        BOMBA_12V::atual() ==
        BOMBA_12V::ESTAGIOS::BOMBA_LIGADA;

    if (!TRAVA_BOLEANA::MudouEstado(
            bombaReservatorioOperando,
            bombaReservatorioAnterior))
    {
        return;
    }

    debugVariavel("Enviando Mensagem LoRa Situacao Bomba Reservatorio :", bombaReservatorioOperando, NEGRITO, LARANJA, AMARELO | LINHA);

    LoRa_Enviar_Evento(
        (uint8_t)LORA::ECO_HIDRO::CODIGO::NOTIFICACAO_STATUS_BOMBA_RESERVATORIO,
        bombaReservatorioOperando ? 1 : 0
    );
}