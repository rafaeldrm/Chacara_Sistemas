//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Analise_LoRa.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

namespace LORA
{
namespace AUTENTICACAO
{

    RegistroAnalise Analise_LoRa::Classificar(
        ResultadoSequencia resultado,
        Sequencia sequencia) const
    {
        RegistroAnalise registro;
        registro.sequencia = sequencia;

        switch (resultado)
        {
        case ResultadoSequencia::REPETICAO_INTERNA:
            registro.evento = EventoAnalise::REPETICAO_INTERNA;
            break;

        case ResultadoSequencia::INFERIOR:
            registro.evento = EventoAnalise::SEQUENCIA_INFERIOR;
            break;

        default:
            registro.evento = EventoAnalise::NENHUM;
            break;
        }

        return registro;
    }

}
}