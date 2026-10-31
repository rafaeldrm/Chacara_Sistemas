//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Audio_Eco_Hidro.h"
#include "DEBUG_MODE.h"

#include "DFPlayerMini\Driver_DFPlayerMini.h"
#include "DFPlayerMini\Leitura_Player_Musical.h"
#include "DFPlayerMini\Musicas_Mapa.h"

#include "Estado_Sistema_Drimel_System.h"

//====================================================================================
//                   LOOP
//====================================================================================

void Audio_Eco_Hidro_Atualiza()
{
    const auto evento = EVENTO::RESERVATORIO::atual();

    if (evento == EVENTO::RESERVATORIO::MENSAGEM::NENHUM)
        return;

    // Player ocupado: deixa o evento no slot e tenta de novo no próximo loop.
    if (Leitura_Player_Musical_Em_Reproducao())
        return;

    switch (evento)
    {
        //==========================================================================

        case EVENTO::RESERVATORIO::MENSAGEM::CAIXA_ENTRADA_CLORO:

            Driver_DFPlayerMini_TocaFaixa(faixa(MUSICA::PASTA::EVENTOS_ECO_HIDRO),
                faixa(MUSICA::ECO_HIDRO::EVENTOS::CAIXA_ENTRADA_VAZIO_CLORO));

        break;

        //==========================================================================

        case EVENTO::RESERVATORIO::MENSAGEM::CAIXA_ENTRADA_TRANSFERENCIA:

            Driver_DFPlayerMini_TocaFaixa(faixa(MUSICA::PASTA::EVENTOS_ECO_HIDRO),
                faixa(MUSICA::ECO_HIDRO::EVENTOS::CAIXA_ENTRADA_CHEIO_TRANSFERENCIA));

        break;

        //==========================================================================

        default:

            // Evento sem música ainda (ex.: ENCHIMENTO_DEMORANDO): só limpa.

        break;
    }

    EVENTO::RESERVATORIO::definir(EVENTO::RESERVATORIO::MENSAGEM::NENHUM);   // consumiu, libera o slot
}