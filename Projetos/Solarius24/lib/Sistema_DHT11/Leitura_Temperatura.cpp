//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Temperatura.h"
#include "Driver_DHT11.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static DadosLeituraDHT11 _dados;

static uint16_t _soma_temp = 0;
static uint16_t _soma_umid = 0;
static uint8_t  _amostras  = 0;
static uint32_t _ultimoTempo = 0;

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Temperatura_Inicializa()
{

    debugSetupArquivo();
    debugSetupTexto("LEITURA :: TEMPERATURA :: DHT11 :: INICIANDO ::", NEGRITO | MAGENTA | LINHA);

    _dados =
    {
        .temperatura_atual = 0,
        .umidade_atual = 0,
        .temperatura_media_atual = 0,
        .umidade_media_atual = 0,
        .temperatura_antiga = 0,
        .umidade_antiga = 0,
        .leitura_finalizada = false,
    };

    _soma_temp = 0;
    _soma_umid = 0;
    _amostras  = 0;


    debugOk("Variáveis de Leitura do Sensor DHT11 Limpas");

}

//====================================================================================
//                   RESETA VARIÁVEIS
//====================================================================================

void Leitura_Temperatura_Reset()
{

     _soma_temp = 0;
     _soma_umid = 0;
     _amostras  = 0;

} 

//====================================================================================
//                   LOOP
//====================================================================================

void Leitura_Temperatura_Atualiza()
{

    uint32_t agora = millis();

    if (agora - _ultimoTempo < 2000)
        return;

    _ultimoTempo = agora;

    uint8_t temp;
    uint8_t umi;

    if (Driver_DHT11_Ler(temp, umi))
    {
        // Validação básica (barata)
        if (temp < 80 && umi <= 100)
        {
            uint16_t t = temp * 10;
            uint16_t u = umi * 10;

            // Atual atual
            _dados.temperatura_atual = t;
            _dados.umidade_atual     = u;

            // Atualiza acumuladores
            _soma_temp += t;
            _soma_umid += u;
            _amostras++;

            // Média a cada 5 amostras
           if (_amostras >= 5)
{
    uint16_t media_temp = _soma_temp / _amostras;
    uint16_t media_umid = _soma_umid / _amostras;

    _soma_temp = 0;
    _soma_umid = 0;
    _amostras  = 0;

    // MOVE histórico ANTES de atualizar
    _dados.temperatura_antiga = _dados.temperatura_media_atual;
    _dados.umidade_antiga     = _dados.umidade_media_atual;

    // Atualiza média atual
    _dados.temperatura_media_atual = media_temp;
    _dados.umidade_media_atual     = media_umid;

    // 💎 Atual final (padronizado)
    _dados.temperatura_atual = media_temp;
    _dados.umidade_atual     = media_umid;

    _dados.leitura_finalizada = true;
}
        }
        else
{
    _dados.leitura_finalizada = false;

    _soma_temp = 0;
    _soma_umid = 0;
    _amostras  = 0;
}
    }
    else
{
    _dados.leitura_finalizada = false;

    _soma_temp = 0;
    _soma_umid = 0;
    _amostras  = 0;
}

}

//====================================================================================
//                   ACESSO AOS DADOS
//====================================================================================

const DadosLeituraDHT11& Leitura_Temperatura_ObterDados()
{

    return _dados;
    
}