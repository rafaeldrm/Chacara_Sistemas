//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Mac_LoRa.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

namespace
{
    inline uint64_t Rotl64(uint64_t x, uint8_t b)
    {
        return (x << b) | (x >> (64 - b));
    }

    // Lê 8 bytes em little-endian (igual em qualquer placa, microcontrolador ou arquitetura)
    uint64_t Ler64(const uint8_t* p)
    {
        uint64_t v = 0;
        for (uint8_t i = 0; i < 8; ++i)
            v |= (uint64_t)p[i] << (8 * i);
        return v;
    }

    void SipRound(uint64_t& v0, uint64_t& v1, uint64_t& v2, uint64_t& v3)
    {
        v0 += v1; v1 = Rotl64(v1, 13); v1 ^= v0; v0 = Rotl64(v0, 32);
        v2 += v3; v3 = Rotl64(v3, 16); v3 ^= v2;
        v0 += v3; v3 = Rotl64(v3, 21); v3 ^= v0;
        v2 += v1; v1 = Rotl64(v1, 17); v1 ^= v2; v2 = Rotl64(v2, 32);
    }

    // SipHash-2-4 com chave de 16 bytes, devolve a etiqueta de 64 bits
    uint64_t SipHash24(const uint8_t chave[16], const uint8_t* dados, uint16_t tamanho)
    {
        const uint64_t k0 = Ler64(chave);
        const uint64_t k1 = Ler64(chave + 8);

        uint64_t v0 = k0 ^ 0x736f6d6570736575ULL;
        uint64_t v1 = k1 ^ 0x646f72616e646f6dULL;
        uint64_t v2 = k0 ^ 0x6c7967656e657261ULL;
        uint64_t v3 = k1 ^ 0x7465646279746573ULL;

        const uint16_t blocos = tamanho / 8;

        for (uint16_t i = 0; i < blocos; ++i)
        {
            const uint64_t m = Ler64(dados + 8 * i);
            v3 ^= m;
            SipRound(v0, v1, v2, v3);
            SipRound(v0, v1, v2, v3);
            v0 ^= m;
        }

        // Último bloco: bytes restantes + tamanho no byte mais alto
        uint64_t b = (uint64_t)(tamanho & 0xFF) << 56;
        const uint8_t restantes = tamanho & 7;

        for (uint8_t j = 0; j < restantes; ++j)
            b |= (uint64_t)dados[8 * blocos + j] << (8 * j);

        v3 ^= b;
        SipRound(v0, v1, v2, v3);
        SipRound(v0, v1, v2, v3);
        v0 ^= b;

        v2 ^= 0xff;
        SipRound(v0, v1, v2, v3);
        SipRound(v0, v1, v2, v3);
        SipRound(v0, v1, v2, v3);
        SipRound(v0, v1, v2, v3);

        return v0 ^ v1 ^ v2 ^ v3;
    }
}

//====================================================================================
//                   SETUP
//====================================================================================

void Mac_LoRa_Inicializa()
{
    debugSetupDivisao(COR::AMARELO, "MAC LORA INICIALIZANDO", COR::MAGENTA, "=", 45);

    // Vetor oficial do SipHash-2-4: chave 00..0f, mensagem 00..0e
    uint8_t chave[16];
    uint8_t mensagem[15];

    for (uint8_t i = 0; i < 16; ++i) chave[i] = i;
    for (uint8_t i = 0; i < 15; ++i) mensagem[i] = i;

    if (SipHash24(chave, mensagem, 15) == 0xa129ca6149be45e5ULL)
    {
        debugOk("MAC: SipHash-2-4 OK (vetor de teste)");
    }
    else
    {
        debugErro("MAC: SipHash-2-4 FALHOU no vetor de teste");
    }
}

//====================================================================================
//                   MAC
//====================================================================================

namespace LORA
{
namespace AUTENTICACAO
{
//====================================================================================
//                   Gerar Mac
//====================================================================================

   bool Mac_LoRa::Gerar(
    const uint8_t* dados,
    uint16_t tamanho,
    const Chave& chave,
    uint8_t mac[TAMANHO_MAC_LORA]) const
{
    if (dados == nullptr || mac == nullptr)
        return false;

    const uint64_t etiqueta = SipHash24(chave.bytes, dados, tamanho);

    // Os primeiros TAMANHO_MAC_LORA bytes da etiqueta (little-endian)
    for (uint8_t i = 0; i < TAMANHO_MAC_LORA; ++i)
        mac[i] = (uint8_t)(etiqueta >> (8 * i));

    return true;
}

//====================================================================================
//                    Validar MAC
//====================================================================================

bool Mac_LoRa::Validar(
        const uint8_t* dados,
        uint16_t tamanho,
        const Chave& chave,
        const uint8_t mac_recebido[TAMANHO_MAC_LORA]) const
    {
        if (dados == nullptr || mac_recebido == nullptr)
            return false;

        uint8_t mac_calculado[TAMANHO_MAC_LORA];

        if (!Gerar(dados, tamanho, chave, mac_calculado))
            return false;

        uint8_t diferente = 0;

        for (uint8_t i = 0; i < TAMANHO_MAC_LORA; ++i)
            diferente |= mac_calculado[i] ^ mac_recebido[i];

        return diferente == 0;
    }
}
}