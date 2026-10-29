//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Envio_Eventos_LoRa.h"
#include "DEBUG_MODE.h"

#include "LoRa_Transporte.h"
#include "Endereco_LoRa.h"
#include "Driver_LoRa.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

struct EnvioEventosLoRa
{

    LORA::ENVIO_EVENTOS envioEventosLoRa;

};

static EnvioEventosLoRa estadoLoRa =
{
    LORA::ENVIO_EVENTOS::NENHUM
};



// Envio "cego": cada mensagem sai em 3 cópias iguais, com pausa (e sorteio) entre elas
static constexpr uint8_t  COPIAS_POR_MENSAGEM = 3;
static constexpr uint32_t INTERVALO_COPIAS_MS = 400;
static constexpr uint32_t JITTER_COPIAS_MS    = 300;

// BOOT: pedido repetido até o Drimel responder
static constexpr uint32_t INTERVALO_BOOT_MS   = 4000;
static constexpr uint32_t JITTER_BOOT_MS      = 2000;



// Fila pequena: as mensagens já entram assinadas, na ordem em que foram pedidas
static constexpr uint8_t FILA_TAMANHO = 4;

struct ItemFila
{
    LORA::Pacote pacote;
    uint8_t restantes;
};

static ItemFila fila[FILA_TAMANHO];
static uint8_t filaInicio = 0;
static uint8_t filaTotal = 0;

static uint32_t proximoPassoMs = 0;

//====================================================================================
//                   FSM do LORA
//====================================================================================

namespace LORA
{
    void definir(ENVIO_EVENTOS novo)
    {
        if (estadoLoRa.envioEventosLoRa == novo) return;
        estadoLoRa.envioEventosLoRa = novo;
        debugVariavel("ENVIO_EVENTOS envioEventosLoRa : ", texto(novo), NENHUM, VERDE, AMARELO);
    }



    ENVIO_EVENTOS atualEnvio()
    {
         return estadoLoRa.envioEventosLoRa;
    }



    const __FlashStringHelper* texto(ENVIO_EVENTOS a)
    {
        switch(a)
        {
            case ENVIO_EVENTOS::NENHUM:                                   return F("NENHUM");
            case ENVIO_EVENTOS::BOOT_PEDINDO:                             return F("BOOT_PEDINDO");
            case ENVIO_EVENTOS::BOOT_AGUARDA_RESPOSTA:                    return F("BOOT_AGUARDA_RESPOSTA");
            case ENVIO_EVENTOS::OPERACIONAL:                              return F("OPERACIONAL");

            default:                return F("DESCONHECIDO");
        }
    }
}

//====================================================================================
//                   AUXILIARES
//====================================================================================

// Seguro contra a volta do millis() (a cada ~49 dias)
static bool Passou(uint32_t alvo)
{
    return (int32_t)(millis() - alvo) >= 0;
}



// Sorteio de 0 a "maximo" ms. Evita que vários sistemas religados juntos (queda de
// energia) falem exatamente ao mesmo tempo.
static uint32_t Sorteio(uint32_t maximo)
{
    return Driver_LoRa_Aleatorio32() % (maximo + 1);
}

//====================================================================================
//                   SETUP
//====================================================================================

void Envio_Eventos_LoRa_Inicializa()
{
    debugSetupDivisao(COR::AMARELO, "ENVIO EVENTOS LORA INICIALIZANDO", COR::MAGENTA, "=", 45);

    filaInicio = 0;
    filaTotal = 0;

    // O primeiro pedido de BOOT sai depois de um pequeno sorteio
    proximoPassoMs = millis() + Sorteio(JITTER_BOOT_MS);

    LORA::definir(LORA::ENVIO_EVENTOS::BOOT_AGUARDA_RESPOSTA);
}

//====================================================================================
//                   PEDE O ENVIO DE UM EVENTO
//====================================================================================

bool LoRa_Enviar_Evento(uint8_t codigo, uint16_t valor)
{
    if (filaTotal >= FILA_TAMANHO)
    {
        debugErro("Envio_Eventos: fila cheia, evento descartado");
        return false;
    }



    ItemFila& item = fila[(filaInicio + filaTotal) % FILA_TAMANHO];

    if (!Endereco_LoRa_Montar(codigo, valor, item.pacote))
        return false;   // ainda sem sessão (BOOT pendente)

    item.restantes = COPIAS_POR_MENSAGEM;
    ++filaTotal;

    return true;
}

//====================================================================================
//                   ENVIA A FILA (UMA CÓPIA POR VEZ)
//====================================================================================

static void EnviarFila()
{
    if (filaTotal == 0 || !Passou(proximoPassoMs))
        return;



    ItemFila& item = fila[filaInicio];

    LoRa_Enviar_Pacote(item.pacote);

    if (--item.restantes == 0)
    {
        filaInicio = (filaInicio + 1) % FILA_TAMANHO;
        --filaTotal;
    }



    proximoPassoMs = millis() + INTERVALO_COPIAS_MS + Sorteio(JITTER_COPIAS_MS);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Envio_Eventos_LoRa_Atualiza()
{
    switch (LORA::atualEnvio())
    {
        case LORA::ENVIO_EVENTOS::BOOT_PEDINDO:

            Endereco_LoRa_PedirBoot();

            proximoPassoMs = millis() + INTERVALO_BOOT_MS + Sorteio(JITTER_BOOT_MS);

            LORA::definir(LORA::ENVIO_EVENTOS::BOOT_AGUARDA_RESPOSTA);
            break;



        case LORA::ENVIO_EVENTOS::BOOT_AGUARDA_RESPOSTA:

            if (Endereco_LoRa_Operacional())
            {
                proximoPassoMs = millis();
                LORA::definir(LORA::ENVIO_EVENTOS::OPERACIONAL);
            }
            else if (Passou(proximoPassoMs))
            {
                LORA::definir(LORA::ENVIO_EVENTOS::BOOT_PEDINDO);
            }
            break;



        case LORA::ENVIO_EVENTOS::OPERACIONAL:

            EnviarFila();
            break;



        default:
            break;
    }
}