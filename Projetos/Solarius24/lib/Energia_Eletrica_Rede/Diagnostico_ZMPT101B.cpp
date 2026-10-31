//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Diagnostico_ZMPT101B.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   CONSTANTES
//====================================================================================

static constexpr uint16_t RMS_MINIMO_VALIDO = 50;
static constexpr uint16_t RMS_MAXIMO_VALIDO = 3500;

static constexpr uint8_t LIMITE_ERROS   = 5;
static constexpr uint8_t LIMITE_TRAVADO = 20;

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static StatusDiagnosticoZMPT101B _diag;

//====================================================================================
//                   SETUP
//====================================================================================

void Diagnostico_ZMPT101B_Inicializa()
{
   


    _diag =
    {
        .estado            = SENSOR::OK,
        .tem_rede          = false,
        .ultimo_rms        = 0,
        .contador_erros    = 0,
        .contador_travado  = 0,
        .erro_lora_enviado = false,
        .mostrar_erro_oled = false,
        .sensor_online     = false,
        .erro_sensor       = false,
       
    };

  
}

//====================================================================================
//                   LOOP
//====================================================================================

void Diagnostico_ZMPT101B_Atualiza(const RedeStatus& leitura)
{
    //===============================================================================
    // 1. ERRO REAL (valor muito baixo)
    //===============================================================================

    if (leitura.rms < RMS_MINIMO_VALIDO)
    {
        _diag.contador_erros++;
        _diag.contador_travado = 0;

        if (_diag.contador_erros > LIMITE_ERROS)
        {
            _diag.sensor_online     = false;
            _diag.erro_sensor       = true;
            _diag.mostrar_erro_oled = true;
        }

        _diag.tem_rede = false;
    }
    else
    {
        //===============================================================================
        // SENSOR NORMAL
        //===============================================================================

        _diag.sensor_online  = true;
        _diag.erro_sensor    = false;
        _diag.contador_erros = 0;

        //===============================================================================
        // DETECÇÃO DE TRAVAMENTO (mais realista)
        //===============================================================================

        if (abs((int32_t)leitura.rms - (int32_t)_diag.ultimo_rms) < 5)
        {
            _diag.contador_travado++;

            if (_diag.contador_travado > LIMITE_TRAVADO)
            {
                debugAviso("ZMPT101B TRAVADO");

                _diag.erro_sensor       = true;
                _diag.mostrar_erro_oled = true;
            }
        }
        else
        {
            _diag.contador_travado = 0;
        }

        //===============================================================================
        // VALOR IMPOSSÍVEL
        //===============================================================================

        if (leitura.rms > RMS_MAXIMO_VALIDO)
        {
            debugErro("ZMPT101B VALOR INVALIDO");

            _diag.sensor_online     = false;
            _diag.erro_sensor       = true;
            _diag.mostrar_erro_oled = true;
        }
    }

    //===============================================================================
    // ENVIO ÚNICO LORA
    //===============================================================================

    if (_diag.erro_sensor && !_diag.erro_lora_enviado)
    {
        debugAviso("LORA ERRO ZMPT101B");
        _diag.erro_lora_enviado = true;
    }

    //===============================================================================
    // ESTADO GLOBAL
    //===============================================================================

    if (_diag.erro_sensor)
        _diag.estado = SENSOR::ERRO;
    else if (_diag.contador_travado > 0)
        _diag.estado = SENSOR::INSTAVEL;
    else
        _diag.estado = SENSOR::OK;

    _diag.ultimo_rms = leitura.rms;
    _diag.tem_rede   = leitura.tem_rede;
}

//====================================================================================
//                   RESET
//====================================================================================

void Diagnostico_ZMPT101B_Resetar_Erros()
{
   

    _diag.contador_erros    = 0;
    _diag.contador_travado  = 0;
    _diag.erro_sensor       = false;
    _diag.mostrar_erro_oled = false;
    _diag.erro_lora_enviado = false;
}

//====================================================================================
//                   GET
//====================================================================================

const StatusDiagnosticoZMPT101B& Diagnostico_ZMPT101B_ObterStatus()
{
    return _diag;
}