#include "Diagnostico_INA226.h"
#include "DEBUG_MODE.h"

//====================================================================================
// CONFIG
//====================================================================================

static constexpr uint8_t  LIMITE_TRAVADO = 20;
static constexpr uint8_t  LIMITE_INSTAVEL = 3;

static constexpr uint16_t TENSAO_MINIMA_ABSURDA = 50;   // 5.0V
static constexpr uint16_t TENSAO_MAXIMA_ABSURDA = 200;  // 20.0V

//====================================================================================
// ESTADO
//====================================================================================

static StatusDiagnosticoINA226 _diag;

//====================================================================================
// INIT
//====================================================================================

void Diagnostico_INA226_Inicializa()
{
    _diag =
    {
        .estado = SENSOR::OK,
        .estado_anterior = SENSOR::OK,

        .ultima_tensao = 0,

        .contador_erros = 0,
        .contador_travado = 0,

        .erro_lora_enviado = false,
        .mostrar_erro_oled = false,

        .sensor_online = false,
        .erro_sensor = false,

        .solicitar_recuperacao = false,

        // EVENTOS
        .evento_recuperado = false,
        .evento_erro_novo = false
    };

    debugOk("Diagnóstico INA226 pronto");
}

//====================================================================================
// LOOP
//====================================================================================

void Diagnostico_INA226_Atualiza(const DadosLeituraVoltagem& leitura)
{
    //============================================================
    // RESET FLAGS VOLÁTEIS
    //============================================================

    _diag.solicitar_recuperacao = false;
    _diag.evento_recuperado     = false;
    _diag.evento_erro_novo      = false;

    //============================================================
    // AGUARDANDO DADO VÁLIDO
    //============================================================

    if (leitura.estado != LEITURA::PRONTA)
        return;

    if (!leitura.novo_dado)
        return;

    uint16_t tensao = leitura.media;

    //============================================================
    // SALVA ESTADO ANTERIOR
    //============================================================

    _diag.estado_anterior = _diag.estado;

    //============================================================
    // RESET PARCIAL
    //============================================================

    _diag.erro_sensor = false;

    //============================================================
    // TRAVAMENTO
    //============================================================

    if (abs((int16_t)tensao - (int16_t)_diag.ultima_tensao) < 3)
    {
        _diag.contador_travado++;

        if (_diag.contador_travado > LIMITE_TRAVADO)
        {
            debugAviso("INA226 TRAVADO");

            _diag.erro_sensor       = true;
            _diag.mostrar_erro_oled = true;
        }
    }
    else
    {
        _diag.contador_travado = 0;
    }

    //============================================================
    // VALOR IMPOSSÍVEL
    //============================================================

    if (tensao < TENSAO_MINIMA_ABSURDA || tensao > TENSAO_MAXIMA_ABSURDA)
    {
        debugErro("INA226 VALOR IMPOSSIVEL");

        _diag.erro_sensor       = true;
        _diag.mostrar_erro_oled = true;
    }

    //============================================================
    // ESTADO FINAL
    //============================================================

    if (_diag.erro_sensor)
    {
        _diag.estado = SENSOR::ERRO;
        _diag.solicitar_recuperacao = true;

        _diag.contador_erros++;
    }
    else if (_diag.contador_travado > LIMITE_INSTAVEL)
    {
        _diag.estado = SENSOR::INSTAVEL;
    }
    else
    {
        _diag.estado = SENSOR::OK;
    }

    //============================================================
    // EVENTOS
    //============================================================

    // ERRO NOVO
    if (_diag.estado_anterior != SENSOR::ERRO && _diag.estado == SENSOR::ERRO)
    {
        _diag.evento_erro_novo = true;
        debugAviso("INA226 ENTROU EM ERRO");
    }

    // RECUPERADO
    if (_diag.estado_anterior == SENSOR::ERRO && _diag.estado == SENSOR::OK)
    {
        _diag.evento_recuperado = true;
        debugOk("INA226 RECUPERADO");
    }

    //============================================================
    // LORA (UMA VEZ POR ERRO)
    //============================================================

    if (_diag.estado == SENSOR::ERRO && !_diag.erro_lora_enviado)
    {
        debugAviso("LORA ERRO INA226");
        _diag.erro_lora_enviado = true;
    }

    // LIBERA NOVO ENVIO SE NORMALIZAR
    if (_diag.estado == SENSOR::OK)
    {
        _diag.erro_lora_enviado = false;
    }

    //============================================================
    // STATUS FINAL
    //============================================================

    _diag.sensor_online = (_diag.estado != SENSOR::ERRO);
    _diag.ultima_tensao = tensao;
}

//====================================================================================
// GET
//====================================================================================

const StatusDiagnosticoINA226& Diagnostico_INA226_ObterStatus()
{
    return _diag;
}