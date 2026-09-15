//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Diagnostico_DHT11.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   CONSTANTES INTERNAS
//====================================================================================

static constexpr uint8_t LIMITE_ERRO_INSTAVEL   = 3;
static constexpr uint8_t LIMITE_ERRO_TOTAL      = 10;
static constexpr uint8_t LIMITE_TRAVAMENTO      = 20;
static constexpr uint8_t TOLERANCIA_VARIACAO    = 2; // 0.2°C (escala *10)

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static StatusDiagnosticoDHT11 _status;

//====================================================================================
//                   SETUP
//====================================================================================

void Diagnostico_DHT11_Inicializa()
{
  

    _status =
    {
        .estado = SENSOR::OK,
        .contador_erros = 0,
        .contador_travado = 0,
        .ultima_temperatura_valida = 0,
        .ultima_umidade_valida = 0,
        .solicitar_reset = false,
        .erro_lora_enviado = false,
        .mostrar_erro_oled = false
    };

 
    
}

//====================================================================================
//                   LOOP
//====================================================================================

void Diagnostico_DHT11_Atualiza(const DadosLeituraDHT11 &leitura)
{
    // Reset de flags de ação (executadas externamente)
    _status.solicitar_reset = false;

    //===============================================================================
    // ERRO DE LEITURA
    //===============================================================================

    if (!leitura.leitura_finalizada)
    {
        _status.contador_erros++;

        // Não avalia mais nada se leitura falhou
    }
    else
    {
        //===============================================================================
        // LEITURA VÁLIDA
        //===============================================================================

        _status.contador_erros = 0;

        uint16_t temp = leitura.temperatura_media_atual;
        uint16_t umi  = leitura.umidade_media_atual;

        //===============================================================================
        // DETECÇÃO DE TRAVAMENTO (com tolerância)
        //===============================================================================

        uint16_t delta_temp = (temp > _status.ultima_temperatura_valida)
            ? (temp - _status.ultima_temperatura_valida)
            : (_status.ultima_temperatura_valida - temp);

        uint16_t delta_umi = (umi > _status.ultima_umidade_valida)
            ? (umi - _status.ultima_umidade_valida)
            : (_status.ultima_umidade_valida - umi);

        if (delta_temp < TOLERANCIA_VARIACAO && delta_umi < TOLERANCIA_VARIACAO)
        {
            _status.contador_travado++;
        }
        else
        {
            _status.contador_travado = 0;
        }

        // Atualiza histórico válido
        _status.ultima_temperatura_valida = temp;
        _status.ultima_umidade_valida     = umi;
    }

    //===============================================================================
    // DEFINIÇÃO DE ESTADO (PRIORIDADE)
    //===============================================================================

    if (_status.contador_erros > LIMITE_ERRO_TOTAL)
    {
        _status.estado = SENSOR::ERRO;
        _status.mostrar_erro_oled = true;
        _status.solicitar_reset = true;
    }
    else if (_status.contador_erros > LIMITE_ERRO_INSTAVEL ||
             _status.contador_travado > LIMITE_TRAVAMENTO)
    {
        _status.estado = SENSOR::INSTAVEL;
        _status.mostrar_erro_oled = true;
    }
    else
    {
        _status.estado = SENSOR::OK;
        _status.mostrar_erro_oled = false;
        _status.erro_lora_enviado = false;
    }
}

//====================================================================================
//                   ACESSO AO STATUS
//====================================================================================

const StatusDiagnosticoDHT11& Diagnostico_DHT11_ObterStatus()
{
    return _status;
}