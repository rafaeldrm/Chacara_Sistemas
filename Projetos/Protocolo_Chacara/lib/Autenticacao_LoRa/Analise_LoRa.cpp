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
namespace SEGURANCA
{

    RegistroAnalise Analise_LoRa::Classificar(
        SEQUENCIA_RESULTADO resultado,
        Sequencia sequencia) const
    {
        RegistroAnalise registro;
        registro.sequencia = sequencia;

        switch (resultado)
        {
        case SEQUENCIA_RESULTADO::REPETICAO_INTERNA:
            registro.evento = ANALISE_EVENTO::REPETICAO_INTERNA;
            break;

        case SEQUENCIA_RESULTADO::INFERIOR:
            registro.evento = ANALISE_EVENTO::SEQUENCIA_INFERIOR;
            break;

        default:
            registro.evento = ANALISE_EVENTO::NENHUM;
            break;
        }

        return registro;
    }

}
}