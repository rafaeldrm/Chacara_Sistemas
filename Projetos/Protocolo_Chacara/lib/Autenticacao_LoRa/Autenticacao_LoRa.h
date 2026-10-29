#pragma once

#include <Arduino.h>

#include "Protocolo_Chacara.h"
#include "Chaves_LoRa.h"
#include "Mac_LoRa.h"
#include "Sequencia_LoRa.h"
#include "Estado_Comunicacao_LoRa.h"
#include "Analise_LoRa.h"

namespace LORA
{
namespace AUTENTICACAO
{

    enum class ResultadoAutenticacao : uint8_t
    {
        NOVA_MENSAGEM = 0,      // aceita: pode processar
        REPETICAO_INTERNA,      // cópia da mesma mensagem: ignorar
        NAO_SINCRONIZADO,
        MAC_INVALIDO,
        SEQUENCIA_INFERIOR,
        BOOT_ACEITO,
        BOOT_INVALIDO           // código ou estado inesperado no handshake
    };

    class Autenticacao_LoRa
    {
    public:
        void Inicializar(const Chave& chave_do_sistema);

        // ---- BOOT: lado do SISTEMA (Eco Hidro, Solarius24, Gate Drimel...)
        void MontarPedidoBoot(Pacote& pedido, SISTEMA destino, SISTEMA remetente, uint32_t nonce);
        ResultadoAutenticacao ReceberRespostaBoot(const Pacote& resposta);

        // ---- BOOT: lado do DRIMEL SYSTEM
        void IniciarBoot();
        ResultadoAutenticacao ReceberPedidoBoot(const Pacote& pedido, Pacote& resposta);

        // ---- Operação normal (os dois lados)
        bool EstaOperacional() const;
        EstadoComunicacao ObterEstado() const;

        bool Assinar(Pacote& p);
        ResultadoAutenticacao Validar(const Pacote& p, RegistroAnalise* registro = nullptr);

    private:
        Chave chave = {};
        Mac_LoRa mac;
        Sequencia_LoRa sequencia;
        Estado_Comunicacao_LoRa estado;
        Analise_LoRa analise;

        uint32_t nonce_boot = 0;            // lado sistema: nonce do pedido em andamento

        bool possui_boot_aceito = false;    // lado Drimel: último pedido aceito
        uint32_t ultimo_nonce_boot = 0;
        uint32_t ultima_base_boot = 0;

        void MontarBufferBoot(const Pacote& p, uint32_t nonce, uint8_t buffer[TAMANHO_ASSINADO + 4]) const;
    };

}
}