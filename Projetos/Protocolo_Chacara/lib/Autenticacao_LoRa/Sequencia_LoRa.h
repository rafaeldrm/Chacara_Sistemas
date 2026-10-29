#pragma once

#include <Arduino.h>

namespace LORA
{
namespace AUTENTICACAO
{

    using Sequencia = uint32_t;

    enum class ResultadoSequencia : uint8_t
    {
        NOVA_MENSAGEM = 0,
        REPETICAO_INTERNA,   // cópia da mesma mensagem (as 3 transmissões)
        INFERIOR
    };

    class Sequencia_LoRa
    {
    public:
        void Resetar();

        bool EstaSincronizada() const;

        // base = último valor já usado na sessão.
        // Saída começa em base+1; entrada só aceita sequência > base.
        void Sincronizar(Sequencia base);

        Sequencia ProximaSaida();
        Sequencia UltimaSaida() const;

        ResultadoSequencia ClassificarRecebida(Sequencia sequencia);

        Sequencia UltimaRecebida() const;

    private:
        bool sincronizada = false;
        Sequencia ultima_saida = 0;
        Sequencia ultima_recebida = 0;
    };

}
}