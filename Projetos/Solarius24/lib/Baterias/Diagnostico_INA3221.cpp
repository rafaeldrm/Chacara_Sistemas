//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Diagnostico_INA3221.h"
#include "DEBUG_MODE.h"

#include "Driver_INA3221.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint16_t LIMITE_ERROS   = 5;
static constexpr uint8_t  LIMITE_TRAVADO = 15;

// Faixa aceitável por célula (12V real)
static constexpr int32_t TENSAO_MINIMA_ABSURDA = 9000;   // 9V
static constexpr int32_t TENSAO_MAXIMA_ABSURDA = 15500;  // 15.5V

// Anti loop de reset
static constexpr uint32_t COOLDOWN_RESET = 5000;

static StatusDiagnosticoINA3221 _diag;

static uint32_t _ultimoReset = 0;

//====================================================================================
//                   SETUP
//====================================================================================

void Diagnostico_INA3221_Inicializa()
{
    debugSetupArquivo();
    debugSetupDivisao(TEXTO_AMARELO, "DIAGNÓSTICO INA3221", TEXTO_MAGENTA, "-", 120);

    
        _diag.estado = SENSOR::OK;

    for (uint8_t i = 0; i < 4; i++)
        _diag.ultima_celula[i] = 0;

         _diag.contador_erros = 0;
         _diag.contador_travado = 0;
         _diag.erro_lora_enviado = false;
         _diag.mostrar_erro_oled = false;
         _diag.sensor_online = false;
         _diag.erro_sensor = false;

    _ultimoReset = 0;

    debugOk("Diagnóstico INA3221 pronto, Variáveis Limpas");
}

//====================================================================================
//                   LOOP
//====================================================================================

void Diagnostico_INA3221_Atualiza(const PackBaterias& leitura)
{
    //============================================================
    // 1. ERRO DE LEITURA (CAMADA LEITURA)
    //============================================================

    if (leitura.erro)
    {
        _diag.contador_erros++;

        if (_diag.contador_erros > LIMITE_ERROS)
        {
            debugErro("INA3221 SEM DADO");

            _diag.sensor_online     = false;
            _diag.erro_sensor       = true;
            _diag.mostrar_erro_oled = true;
        }

        return;
    }

    //============================================================
    // 2. RESET PARCIAL (LEITURA OK)
    //============================================================

    _diag.sensor_online  = true;
    _diag.erro_sensor    = false;
    _diag.contador_erros = 0;

    //============================================================
    // 3. VALORES ABSURDOS (ANTI BUG)
    //============================================================

    for (uint8_t i = 0; i < 4; i++)
    {
        int32_t v = leitura.celula[i];

        if (v < TENSAO_MINIMA_ABSURDA || v > TENSAO_MAXIMA_ABSURDA)
        {
            debugErro("INA3221 VALOR INVALIDO");

            _diag.sensor_online     = false;
            _diag.erro_sensor       = true;
            _diag.mostrar_erro_oled = true;

            break;
        }
    }

    //============================================================
    // 4. DETECÇÃO DE TRAVAMENTO INTELIGENTE
    //============================================================

    uint8_t iguais = 0;

    for (uint8_t i = 0; i < 4; i++)
    {
        if (abs(leitura.celula[i] - _diag.ultima_celula[i]) < 10)
            iguais++;
    }

    if (iguais >= 3) // maioria travada
    {
        _diag.contador_travado++;

        if (_diag.contador_travado > LIMITE_TRAVADO)
        {
            debugAviso("INA3221 POSSIVEL TRAVAMENTO");

            _diag.erro_sensor       = true;
            _diag.mostrar_erro_oled = true;
        }
    }
    else
    {
        _diag.contador_travado = 0;
    }

    //============================================================
    // 5. AUTO RECUPERAÇÃO (COM COOLDOWN)
    //============================================================

    if (_diag.erro_sensor)
    {
        if (millis() - _ultimoReset > COOLDOWN_RESET)
        {
            debugAviso("RECUPERANDO INA3221");

            SENSOR_INA3221_B1.Reset_Software();
            SENSOR_INA3221_B1.Liga();

            SENSOR_INA3221_B2.Reset_Software();
            SENSOR_INA3221_B2.Liga();

            _ultimoReset = millis();
        }
    }

    //============================================================
    // 6. LORA (UMA VEZ)
    //============================================================

    if (_diag.erro_sensor && !_diag.erro_lora_enviado)
    {
        debugAviso("LORA ERRO INA3221");

        _diag.erro_lora_enviado = true;
    }

    //============================================================
    // 7. ESTADO GLOBAL
    //============================================================

    if (_diag.erro_sensor)
    {
        _diag.estado = SENSOR::ERRO;
    }
    else if (_diag.contador_travado > 0)
    {
        _diag.estado = SENSOR::INSTAVEL;
    }
    else
    {
        _diag.estado = SENSOR::OK;
    }

    _diag.sensor_online = !_diag.erro_sensor;

    //============================================================
    // 8. ATUALIZA HISTÓRICO
    //============================================================

    for (uint8_t i = 0; i < 4; i++)
        _diag.ultima_celula[i] = leitura.celula[i];
}

//====================================================================================
//                   RESET POR OLED
//====================================================================================

void Diagnostico_INA3221_ResetarErros()
{
    _diag.contador_erros    = 0;
    _diag.contador_travado  = 0;
    _diag.erro_sensor       = false;
    _diag.mostrar_erro_oled = false;
    _diag.erro_lora_enviado = false;

    debugLoopTexto("RESET DIAGNOSTICO INA3221", NENHUM);
}

//====================================================================================
//                   ACESSO AOS DADOS
//====================================================================================

const StatusDiagnosticoINA3221& Diagnostico_INA3221_ObterStatus()
{
    return _diag;
}