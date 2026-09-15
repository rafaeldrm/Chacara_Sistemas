//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Emergencia_Climatizacao_MPPT.h"
#include "DEBUG_MODE.h"
#include "Estados_Modulos.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

// Tempos em ms
static constexpr uint32_t TEMPO_LIGADO     = 10UL * 60UL * 1000UL; // 10 min
static constexpr uint32_t TEMPO_DESLIGADO  = 30UL * 60UL * 1000UL; // 30 min

static EmergenciaClimatizacao _dados;

static EstadoEmergenciaClimatizacao _estado = EstadoEmergenciaClimatizacao::NORMAL;
static uint32_t _tempoReferencia = 0;

static bool _lora_enviado = false;

//====================================================================================
//                   SETUP
//====================================================================================

void Emergencia_Climatizacao_MPPT_Inicializa()
{
     
    _estado = EstadoEmergenciaClimatizacao::NORMAL;
    _tempoReferencia = 0;
    _lora_enviado = false;

    _dados =
    {
        .ativo = false,
        .cooler_ligado = false
    };

   
    
}

//====================================================================================
//                   LOOP
//====================================================================================

void Emergencia_Climatizacao_MPPT_Atualiza(const StatusDiagnosticoDHT11& status)
{
    uint32_t agora = millis();

    //===============================================================================
    // SENSOR OK OU INSTÁVEL → SAI DA EMERGÊNCIA
    //===============================================================================

    if (status.estado != SENSOR::ERRO)
    {
        _estado = EstadoEmergenciaClimatizacao::NORMAL;
        _tempoReferencia = 0;
        _lora_enviado = false;

        _dados.ativo = false;

        return;
    }

    //===============================================================================
    // ENTRADA NA EMERGÊNCIA
    //===============================================================================

    if (_estado == EstadoEmergenciaClimatizacao::NORMAL)
    {
        _estado = EstadoEmergenciaClimatizacao::LIGADO;
        _tempoReferencia = agora;

        _dados.cooler_ligado = true; // fail-safe imediato
    }

    //===============================================================================
    // EVENTO ÚNICO (LoRa, log, etc)
    //===============================================================================

    if (!_lora_enviado)
    {
        _lora_enviado = true;
        // Disparo externo aqui
    }

    //===============================================================================
    // MÁQUINA DE ESTADOS
    //===============================================================================

    switch (_estado)
    {
        case EstadoEmergenciaClimatizacao::LIGADO:

            _dados.cooler_ligado = true;

            if (agora - _tempoReferencia >= TEMPO_LIGADO)
            {
                _estado = EstadoEmergenciaClimatizacao::DESLIGADO;
                _tempoReferencia = agora;
            }

            break;

        case EstadoEmergenciaClimatizacao::DESLIGADO:

            _dados.cooler_ligado = false;

            if (agora - _tempoReferencia >= TEMPO_DESLIGADO)
            {
                _estado = EstadoEmergenciaClimatizacao::LIGADO;
                _tempoReferencia = agora;
            }

            break;

        default:
            break;
    }

    //===============================================================================
    // ESTADO GLOBAL
    //===============================================================================

    _dados.ativo = (_estado != EstadoEmergenciaClimatizacao::NORMAL);
}

//====================================================================================
//                   ACESSO AOS DADOS
//====================================================================================

const EmergenciaClimatizacao& Emergencia_Climatizacao_ObterEstado()
{
    return _dados;
}