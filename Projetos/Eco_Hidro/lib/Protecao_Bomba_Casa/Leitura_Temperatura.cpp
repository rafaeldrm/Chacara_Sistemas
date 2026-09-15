//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Temperatura.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

#include "Driver_DHT11.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t INTERVALO_LEITURA = TEMPO::SEGUNDOS(2); //2000; // 2s
static constexpr uint8_t  TOTAL_AMOSTRAS    = 3;

static DadosLeituraTemperatura _dados;

static uint16_t _somaTemperatura = 0;
static uint16_t _somaUmidade = 0;
static uint8_t  _amostras  = 0;

static CRONOMETRO::Temporizador ultimaLeitura;

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Temperatura_Inicializa()
{
    _dados = {
        .temperatura = 0,
        .umidade = 0,
        .valido = false
    };

    _somaTemperatura = 0;
    _somaUmidade = 0;
    _amostras  = 0;

    ultimaLeitura.Cronometro_Iniciar();
}

//====================================================================================
//                   LOOP
//====================================================================================

void Leitura_Temperatura_Atualiza()
{

    if (ultimaLeitura.Cronometro_Ciclo_Continuo(INTERVALO_LEITURA))
        return;

    uint8_t temp;
    uint8_t umi;

    if (!Driver_DHT11_Ler(temp, umi))
        return;

    // validação básica
    if (temp >= 80 || umi > 100)
        return;

    uint16_t t = temp * 10;
    uint16_t u = umi * 10;

    _somaTemperatura += t;
    _somaUmidade += u;
    _amostras++;

    if (_amostras >= TOTAL_AMOSTRAS)
    {
        _dados.temperatura = _somaTemperatura / _amostras;
        _dados.umidade     = _somaUmidade / _amostras;
        _dados.valido      = true;

        _somaTemperatura = 0;
        _somaUmidade = 0;
        _amostras  = 0;
    }
}

//====================================================================================
//                   LEITURA TEMPERATURA ENTREGA
//====================================================================================

int16_t Leitura_Temperatura_Obter_Temperatura()
{
    if (!_dados.valido)
        return -1;

    return _dados.temperatura;
}

//====================================================================================
//                   ENTREGA PACOTE DE INFORMAÇÕES AO SISTEMA
//====================================================================================

const DadosLeituraTemperatura& Leitura_Temperatura_Obter_Dados()
{
    return _dados;
}