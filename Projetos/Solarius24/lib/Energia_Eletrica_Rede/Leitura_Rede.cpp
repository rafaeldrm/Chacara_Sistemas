//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Rede.h"
#include "Driver_ZMPT101B.h"
#include "Raiz_Inteira.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS 
//====================================================================================

    static RedeStatus _rede;

// Ajuste fino do sensor (calibração)
static constexpr uint16_t CALIBRACAO = 330;

// Meia onda (~4ms em 60Hz) → menos bloqueante
static constexpr uint16_t PERIODO_REDE = 4000;

// Histerese
static constexpr uint16_t TENSAO_LIGA  = 1000;
static constexpr uint16_t TENSAO_DESLIGA = 800;

// Reset diário
static constexpr uint32_t RESET_PERIODO = 24UL * 60UL * 60UL * 1000UL;

static uint32_t ultimoReset = 0;

// Média
static uint32_t _soma_rms = 0;
static uint8_t  _amostras = 0;

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Rede_Inicializa()
{
    debugSetupArquivo();
    debugSetupTexto("LEITURA ::REDE :: ZMPT101B :: INICIALIZANDO", NEGRITO | MAGENTA | LINHA);

     ultimoReset = 0;

    _rede =
    {
        .rms = 0,
        .rms_anterior = 0,
        .rms_medio = 0,
        .maximo = 0,
        .minimo = 0xFFFF,
        .tem_rede = false
    };

    _soma_rms = 0;
    _amostras = 0;

    debugOk("Variáveis de Leitura do Sensor ZMPT101B Limpas");

}

//====================================================================================
//                   ENCHER BUFFER DE LEITURA
//====================================================================================

void Leitura_Rede_Estabilizar() {
    debugAviso("ZMPT101B: Estabilizando leitura (Bloqueante)...");
    
    // Faz 100 leituras rápidas para encher o RMS
    for(int i = 0; i < 100; i++) {
        Leitura_Rede_Atualiza(); 
        delay(10); // Pequeno intervalo para pegar diferentes pontos da onda
    }
    
    debugOk("ZMPT101B: Leitura estabilizada.");
}

//====================================================================================
//                   LOOP
//====================================================================================

void Leitura_Rede_Atualiza()
{
    uint32_t acumuladorQuadrados = 0;
    uint32_t soma = 0;
    uint16_t contador = 0;

    //===============================================================================
    // RESET DIÁRIO
    //===============================================================================

    if (millis() - ultimoReset >= RESET_PERIODO) {
        _rede.maximo = 0;
        _rede.minimo = 0xFFFF;
        ultimoReset = millis();
    }

    //===============================================================================
    // OFFSET (mais leve)
    //===============================================================================

    static uint16_t offsetReal = 512; // valor inicial seguro

    uint32_t inicio = micros();

    while (micros() - inicio < PERIODO_REDE) {
        uint16_t leituraRaw = Driver_ZMPT101B_Ler();
        soma += leituraRaw;
        contador++;
    }

    if (contador > 0)
        offsetReal = soma / contador;

    //===============================================================================
    // RMS
    //===============================================================================

    inicio = micros();
    contador = 0;

    while (micros() - inicio < PERIODO_REDE) {

        uint16_t leituraRaw = Driver_ZMPT101B_Ler();

        int32_t sinal = (int32_t)leituraRaw - offsetReal;

        uint32_t quadrado = (uint32_t)(sinal * sinal);

        acumuladorQuadrados += quadrado;
        contador++;
    }

    if (contador == 0) return;

    uint32_t mediaQuadrados = acumuladorQuadrados / contador;

    uint16_t rmsBruto = Raiz_Inteira(mediaQuadrados);

    uint16_t tensaoDecivolts = ((uint32_t)rmsBruto * CALIBRACAO) / 10;

    //===============================================================================
    // HISTÓRICO
    //===============================================================================

    _rede.rms_anterior = _rede.rms;
    _rede.rms = tensaoDecivolts;

    //===============================================================================
    // MÉDIA
    //===============================================================================

    _soma_rms += tensaoDecivolts;
    _amostras++;

    if (_amostras >= 5)
    {
        _rede.rms_medio = _soma_rms / _amostras;

        _soma_rms = 0;
        _amostras = 0;
    }

    //===============================================================================
    // HISTERESSE
    //===============================================================================

    static bool estadoAnterior = false;

    if (tensaoDecivolts > TENSAO_LIGA)
        estadoAnterior = true;
    else if (tensaoDecivolts < TENSAO_DESLIGA)
        estadoAnterior = false;

    _rede.tem_rede = estadoAnterior;

    //===============================================================================
    // LIMITES (telemetria)
    //===============================================================================

    if (tensaoDecivolts > 1000 && tensaoDecivolts < 3000)
    {
        if (tensaoDecivolts > _rede.maximo)
            _rede.maximo = tensaoDecivolts;

        if (tensaoDecivolts < _rede.minimo)
            _rede.minimo = tensaoDecivolts;
    }
}
//====================================================================================
//                   ACESSO AOS DADOS
//====================================================================================

const RedeStatus& Leitura_Rede_ObterStatus()
{
    return _rede;
}