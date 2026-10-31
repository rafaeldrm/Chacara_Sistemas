#include "Emergencia_Voltagem.h"
#include "Modulo_Reles.h"
#include "DEBUG_MODE.h"

//====================================================================================
// CONFIG
//====================================================================================

static constexpr uint32_t TEMPO_PROTECAO   = 5000UL;
static constexpr uint32_t TEMPO_LIGADO     = 3600000UL; // 1h
static constexpr uint32_t TEMPO_DESLIGADO  = 7200000UL; // 2h

//====================================================================================
// ESTADO
//====================================================================================

static EmergenciaVoltagem _dados;

static EstadoEmergenciaVoltagem _estado;

static uint32_t _tempoReferencia = 0;

//====================================================================================
// INIT
//====================================================================================

void Emergencia_Voltagem_Inicializa()
{
    _estado = EstadoEmergenciaVoltagem::NORMAL;

    _dados =
    {
        .ativo = false,
        .fonte_ligada = false
    };

    debugOk("Emergência Voltagem pronta");
}

//====================================================================================
// LOOP
//====================================================================================

void Emergencia_Voltagem_Atualiza(const StatusDiagnosticoINA226& diag)
{
    uint32_t agora = millis();

    //============================================================
    // SENSOR OK → VOLTA AO NORMAL
    //============================================================

    if (diag.estado == SENSOR::OK)
    {
        _estado = EstadoEmergenciaVoltagem::NORMAL;
        _dados.ativo = false;
        _dados.fonte_ligada = false;

        FONTE_USINA.Desligar();

        return;
    }

    //============================================================
    // ATIVOU EMERGÊNCIA
    //============================================================

    if (!_dados.ativo)
    {
        _dados.ativo = true;
        _estado = EstadoEmergenciaVoltagem::PROTEGENDO;
        _tempoReferencia = agora;

        debugAviso("EMERGÊNCIA VOLTAGEM ATIVADA");
    }

    //============================================================
    // MÁQUINA DE ESTADOS
    //============================================================

    switch (_estado)
    {
        //========================================================
        // CORTE INICIAL
        //========================================================

        case EstadoEmergenciaVoltagem::PROTEGENDO:

            _dados.fonte_ligada = false;
            FONTE_USINA.Desligar();

            if (agora - _tempoReferencia >= TEMPO_PROTECAO)
            {
                _estado = EstadoEmergenciaVoltagem::CICLO_LIGADO;
                _tempoReferencia = agora;

                debugAviso("EMERGÊNCIA: INICIANDO CICLO");
            }

            break;

        //========================================================
        // LIGA POR 1H
        //========================================================

        case EstadoEmergenciaVoltagem::CICLO_LIGADO:

            _dados.fonte_ligada = true;
            FONTE_USINA.Ligar();

            if (agora - _tempoReferencia >= TEMPO_LIGADO)
            {
                _estado = EstadoEmergenciaVoltagem::CICLO_DESLIGADO;
                _tempoReferencia = agora;

                debugAviso("EMERGÊNCIA: DESLIGANDO (CICLO)");
            }

            break;

        //========================================================
        // DESLIGA POR 2H
        //========================================================

        case EstadoEmergenciaVoltagem::CICLO_DESLIGADO:

            _dados.fonte_ligada = false;
            FONTE_USINA.Desligar();

            if (agora - _tempoReferencia >= TEMPO_DESLIGADO)
            {
                _estado = EstadoEmergenciaVoltagem::CICLO_LIGADO;
                _tempoReferencia = agora;

                debugAviso("EMERGÊNCIA: RELIGANDO (CICLO)");
            }

            break;

        default:
            break;
    }
}

//====================================================================================
// GET
//====================================================================================

const EmergenciaVoltagem& Emergencia_Voltagem_ObterEstado()
{
    return _dados;
}