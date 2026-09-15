//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Envio_Eventos_LoRa.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

#include "Endereco_LoRa.h"
#include "Gerenciador_Mensagens.h"      // LoRa_Receber_Pacote
#include "Recebimento_Ordens_LoRa.h"
#include "Protocolo_Chacara.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

enum class ESTADO_ENVIO : uint8_t
{
    IDLE = 0,
    AGUARDANDO_ENVIO,
    AGUARDANDO_RESPOSTA
};

static ESTADO_ENVIO estado = ESTADO_ENVIO::IDLE;

static uint8_t  codigoPendente   = 0;
static uint16_t valorPendente    = 0;
static uint8_t  tentativas       = 0;

static bool     ultimoSucesso    = false;

static constexpr uint8_t  MAX_TENTATIVAS = 3;
static constexpr uint32_t INTERVALO_MS   = TEMPO::SEGUNDOS(3);

static CRONOMETRO::Temporizador timerResposta;

//====================================================================================
//                   SETUP
//====================================================================================

void Envio_Eventos_LoRa_Inicializa()
{
    estado         = ESTADO_ENVIO::IDLE;
    tentativas     = 0;
    ultimoSucesso  = false;
    codigoPendente = 0;
    valorPendente  = 0;

    debugSetupTexto("Inicializando Envio de Eventos LoRa", NEGRITO | SUBLINHADO | AZUL | LINHA);
}

//====================================================================================
//                   SOLICITAR ENVIO (não bloqueia)
//====================================================================================

bool Envio_Eventos_LoRa_Solicitar(uint8_t codigo, uint16_t valor)
{
    if (estado != ESTADO_ENVIO::IDLE)
    {
        debugAviso("Envio_Eventos_LoRa: ocupado, solicitação ignorada");
        return false;
    }

    codigoPendente = codigo;
    valorPendente  = valor;
    tentativas     = 0;
    ultimoSucesso  = false;

    estado = ESTADO_ENVIO::AGUARDANDO_ENVIO;

    debugVariavel("Envio_Eventos_LoRa: solicitação aceita, codigo", codigo, NEGRITO, VERDE, AMARELO);
    return true;
}

//====================================================================================
//                   LOOP (chamar no LORA_LOOP)
//====================================================================================

void Envio_Eventos_LoRa_Atualiza()
{
    switch (estado)
    {
        //----------------------------------------------------------
        case ESTADO_ENVIO::IDLE:
            // Nada a fazer
            break;

        //----------------------------------------------------------
        case ESTADO_ENVIO::AGUARDANDO_ENVIO:
        {
            Endereco_LoRa_Mensagem(codigoPendente, valorPendente);
            tentativas++;

            timerResposta.Cronometro_Iniciar();
            estado = ESTADO_ENVIO::AGUARDANDO_RESPOSTA;

            debugVariavel("Envio_Eventos_LoRa: tentativa", tentativas, NEGRITO, CIANO, AMARELO);
            break;
        }

        //----------------------------------------------------------
        case ESTADO_ENVIO::AGUARDANDO_RESPOSTA:
        {
            // Verifica se chegou alguma ordem válida do Drimel
            LORA::Pacote pacote;
            if (LoRa_Receber_Pacote(pacote))
            {
                if (Recebimento_Ordens_LoRa(pacote))
                {
                    // Consideramos sucesso (recebeu ordem válida)
                    ultimoSucesso = true;
                    estado = ESTADO_ENVIO::IDLE;
                    debugOk("Envio_Eventos_LoRa: sucesso (ordem recebida)");
                    return;
                }
            }

            // Timeout da janela de 3 segundos?
            if (timerResposta.Cronometro_Ciclo_Continuo(INTERVALO_MS))
            {
                if (tentativas >= MAX_TENTATIVAS)
                {
                    // Esgotou as tentativas
                    ultimoSucesso = false;
                    estado = ESTADO_ENVIO::IDLE;
                    debugErro("Envio_Eventos_LoRa: falhou após 3 tentativas");
                }
                else
                {
                    // Ainda tem tentativa → volta para enviar de novo
                    estado = ESTADO_ENVIO::AGUARDANDO_ENVIO;
                }
            }
            break;
        }
    }
}

//====================================================================================
//                   CONSULTAS
//====================================================================================

bool Envio_Eventos_LoRa_Ocupado()
{
    return estado != ESTADO_ENVIO::IDLE;
}

bool Envio_Eventos_LoRa_Ultimo_Sucesso()
{
    return ultimoSucesso;
}