//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerenciador_Mensagens.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

#include "LoRa_Transporte.h"
#include "Recebimento_Ordens_LoRa.h"
#include "Endereco_LoRa.h"
#include "Protocolo_Chacara.h"

#include "Estados_Sistema_Eco_Hidro.h"

#include "Leitura_Boias.h"
#include "Leitura_Bateria.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static uint8_t passo = 0;

static bool checklistAtivo = false;

constexpr uint32_t INTERVALO = TEMPO::SEGUNDOS(3);

static CRONOMETRO::Temporizador ultimoPasso;

//====================================================================================
//                   SETUP
//====================================================================================

void LoRa_Gerencia_Inicializa()
{

    ultimoPasso.Cronometro_Iniciar();

    LoRa_Enviar_Evento((uint8_t)LORA::ECO_HIDRO::CODIGO::SISTEMA_BOOT, 0);
}

//====================================================================================
//                   LOOP
//====================================================================================

void LoRa_Gerencia_Atualiza()
{
    LORA::Pacote pacote;

     if (LoRa_Receber_Pacote(pacote))
    {
        Recebimento_Ordens_LoRa(pacote);
    }

    // CHECKLIST
    if (checklistAtivo && (ultimoPasso.Cronometro_Ciclo_Continuo(INTERVALO)))
    {
        ultimoPasso.Cronometro_Iniciar();

        switch (passo)
        {
            case 1:
                Endereco_LoRa_Mensagem((uint8_t)LORA::ECO_HIDRO::CODIGO::NIVEL_BOIAS, (uint16_t)Leitura_Boias_Mapa_Completo());
                break;

            case 2:
                Endereco_LoRa_Mensagem((uint8_t)LORA::ECO_HIDRO::CODIGO::STATUS_BOMBA_CASA, (uint16_t)BOMBA_CASA::atual());
                break;

            case 3:
                Endereco_LoRa_Mensagem((uint8_t)LORA::ECO_HIDRO::CODIGO::STATUS_BOMBA_RESERVATORIO, (uint16_t)BOMBA_12V::atual());
                break;

            case 4:
                Endereco_LoRa_Mensagem((uint8_t)LORA::ECO_HIDRO::CODIGO::VOLTAGEM_SISTEMA, (uint16_t)Leitura_Bateria_Entrega());
                break;

            case 5:
                Endereco_LoRa_Mensagem((uint8_t)LORA::ECO_HIDRO::CODIGO::ACK_STATUS_GERAL_FIM, 0);
                checklistAtivo = false;
                passo = 0;
                debugOk("Checklist finalizado");
                return;
        }

        passo++;
    }
}

//====================================================================================
//                 INICIA CHECKLIST
//====================================================================================



void LoRa_Iniciar_Checklist()
{
    if (!checklistAtivo)
    {
        checklistAtivo = true;
        passo = 1;
        ultimoPasso.Cronometro_Iniciar();
    }
}

//====================================================================================
//                   ENVIA EVENTOS
//====================================================================================

void LoRa_Enviar_Evento(uint8_t codigo, uint16_t valor)
{
    bool resolvido = false;

    for (uint8_t i = 0; i < 3 && !resolvido; i++)
    {
        Endereco_LoRa_Mensagem(codigo, valor);

        CRONOMETRO::Temporizador timer;
        timer.Cronometro_Iniciar();

        while (timer.Cronometro_Ciclo_Continuo(INTERVALO))
        {
            LORA::Pacote pacote;

            if (LoRa_Receber_Pacote(pacote))
            {
                if (Recebimento_Ordens_LoRa(pacote))
                {
                    resolvido = true;
                    break;
                }
            }
        }
    }
}