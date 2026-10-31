//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Corrente_Maquina.h"
#include "Driver_SCT013.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   CONFIGURAÇÕES
//====================================================================================

// Quantidade de amostras (leve e suficiente)
static constexpr uint8_t AMOSTRAS = 50;

// Tempo entre leituras (ms)
static constexpr uint16_t INTERVALO_LEITURA = 500;

// Ajuste fino (calibrar depois)
static constexpr uint16_t FATOR = 10;
static constexpr uint16_t ESCALA = 120;

// Tensão da rede
static constexpr uint16_t TENSAO_REDE = 220;

// Filtro de ruído mínimo
static constexpr uint16_t LIMIAR_RUIDO = 10;

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static DadosCorrenteMaquina dados = {0, 0, false};

static uint32_t ultimo_tempo = 0;

//====================================================================================
//                   LEITURA DE PICO (LEVE)
//====================================================================================

static uint16_t Leitura_SCT013_Pico() {
    uint16_t max_val = 0;
    uint16_t min_val = 1023;

    for (uint8_t i = 0; i < AMOSTRAS; i++) {

        uint16_t leitura = Driver_SCT013_Ler_ADC();

        if (leitura > max_val) max_val = leitura;
        if (leitura < min_val) min_val = leitura;
    }

    return (max_val - min_val);
}

//====================================================================================
//                   LOOP PRINCIPAL
//====================================================================================

void Leitura_Corrente_Maquina_Loop() {

    // Controle de tempo
    if (millis() - ultimo_tempo < INTERVALO_LEITURA) return;
    ultimo_tempo = millis();

    uint16_t pico = Leitura_SCT013_Pico();

    // Filtro de ruído
    if (pico < LIMIAR_RUIDO) {
        dados.corrente = 0;
        dados.potencia = 0;
    } else {
        // Conversão aproximada (inteira)
        dados.corrente = (pico * FATOR) / ESCALA;

        // Potência estimada
        dados.potencia = (dados.corrente * TENSAO_REDE) / 100;
    }

    dados.novo_dado = true;

}

//====================================================================================
//                   GETTERS
//====================================================================================

const DadosCorrenteMaquina& Leitura_Corrente_Maquina_Pegar() {
    return dados;
}

//====================================================================================
//                   CONSUMO DO DADO
//====================================================================================

void Leitura_Corrente_Maquina_Consumir() {
    dados.novo_dado = false;
}