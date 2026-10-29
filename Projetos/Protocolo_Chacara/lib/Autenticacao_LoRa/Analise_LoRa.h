//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Sequencia_LoRa.h"

//-------------------------- DADOS ------------------------------------------//

namespace LORA
{
namespace AUTENTICACAO
{

    enum class EventoAnalise : uint8_t
    {
        NENHUM = 0,
        REPETICAO_INTERNA,
        SEQUENCIA_REPETIDA_FORA_DA_JANELA,
        SEQUENCIA_INFERIOR,
        MENSAGEM_ESTRANHA
    };

    struct RegistroAnalise
    {
        EventoAnalise evento = EventoAnalise::NENHUM;
        Sequencia sequencia = 0;
    };

    class Analise_LoRa
    {
    public:
        RegistroAnalise Classificar(ResultadoSequencia resultado, Sequencia sequencia) const;

    };

}
}

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

//-------------------------- ACESSO AOS DADOS -------------------------------//