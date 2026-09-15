#include "Gerenciador_Mensagens.h"
#include "LoRa_Transporte.h"
#include "Interpretador_Mensagens.h"
#include "Telemetria_Sistema.h"
#include "DEBUG_MODE.h"
#include "Protocolo_Chacara.h"

#include "Leitura_Rede.h"

#include "Estado_Globais.h"

static uint32_t ultimoPasso = 0;
static uint8_t passo = 0;
static bool checklistAtivo = false;

constexpr uint16_t INTERVALO = 3000;
constexpr uint32_t INTERVALO_AUTO = 600000;

static uint32_t ultimaAuto = 0;

//================================================

void LoRa_Gerencia_Inicializa()
{

    ultimaAuto = millis();

    LoRa_Enviar_Evento((uint8_t)LORA::UNIVERSAL::SISTEMA_BOOT, 0);
}

//================================================

void LoRa_Gerencia_Atualiza()
{
    LORA::Pacote pacote;

     if (LoRa_Receber_Pacote(pacote))
    {
        LoRa_Interpretar(pacote);
    }

    // CHECKLIST
    if (checklistAtivo && (millis() - ultimoPasso >= INTERVALO))
    {
        ultimoPasso = millis();

        RedeStatus rede = RedeStatus();

        switch (passo)
        {
            case 1:
                LoRa_Enviar_Telemetria((uint8_t)LORA::SOLAR::REDE_ATUAL, rede.rms);
                break;

            case 2:
                LoRa_Enviar_Telemetria((uint8_t)LORA::SOLAR::REDE_MAXIMA, rede.maximo);
                break;

            case 3:
                LoRa_Enviar_Telemetria((uint8_t)LORA::SOLAR::REDE_MINIMA, rede.minimo);
                break;

            case 4:
                LoRa_Enviar_Evento((uint8_t)LORA::UNIVERSAL::ACK_STATUS_FIM, 0);
                checklistAtivo = false;
                passo = 0;
                debugOk("Checklist finalizado");
                return;
        }

        passo++;
    }

    // AUTOMÁTICO
    if (!checklistAtivo && (millis() - ultimaAuto >= INTERVALO_AUTO))
    {
        LoRa_Iniciar_Checklist();
        ultimaAuto = millis();
    }
}

//================================================

void LoRa_Iniciar_Checklist()
{
    if (!checklistAtivo)
    {
        checklistAtivo = true;
        passo = 1;
        ultimoPasso = millis() - INTERVALO;
    }
}

//================================================

void LoRa_Enviar_Evento(uint8_t codigo, uint16_t valor)
{
    bool resolvido = false;

    for (uint8_t i = 0; i < 3 && !resolvido; i++)
    {
        LoRa_Enviar_Telemetria(codigo, valor);

        unsigned long inicio = millis();

        while (millis() - inicio < 300)
        {
            LORA::Pacote pacote;

            if (LoRa_Receber_Pacote(pacote))
            {
                if (LoRa_Interpretar(pacote))
                {
                    resolvido = true;
                    break;
                }
            }
        }
    }
}