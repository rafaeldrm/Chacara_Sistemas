//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Nichos.h"
#include "DEBUG_MODE.h"

#include "Mapa_Pinos_Arduino.h"
#include "Tabela_Categorias_Nichos.h"
#include "Estado_Sistema_Lavanderia.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

constexpr uint8_t QUANTIDADE_NICHOS = static_cast<uint8_t>(NICHO_CATEGORIA::TOTAL_CATEGORIAS);

// Mapeamento dos pinos Reed Switch ordenados pelo enum NICHO_CATEGORIA
static const uint8_t PINOS_REED[QUANTIDADE_NICHOS] = {
    pino(ARDUINO_CONEXAO::REED_SWITCH::CUECAS),            // 0
    pino(ARDUINO_CONEXAO::REED_SWITCH::MEIAS_PASSEIO),     // 1
    pino(ARDUINO_CONEXAO::REED_SWITCH::MEIAS_CHACARA),     // 2
    pino(ARDUINO_CONEXAO::REED_SWITCH::BERMUDAS_CHACARA),  // 3
    pino(ARDUINO_CONEXAO::REED_SWITCH::BERMUDAS_PASSEIO),  // 4
    pino(ARDUINO_CONEXAO::REED_SWITCH::CAMISETAS_CHACARA), // 5
    pino(ARDUINO_CONEXAO::REED_SWITCH::CAMISETAS_PASSEIO), // 6
    pino(ARDUINO_CONEXAO::REED_SWITCH::TOALHAS),           // 7
    pino(ARDUINO_CONEXAO::REED_SWITCH::PIJAMAS)            // 8
};


static uint8_t  quantidadeAtual[QUANTIDADE_NICHOS]   = {0};
static uint32_t tempoEntradaEstado[QUANTIDADE_NICHOS] = {0};

//====================================================================================
//                   SETUP
//====================================================================================

void Nichos_Inicializa()
{

    for (uint8_t i = 0; i < QUANTIDADE_NICHOS; i++)
    {
        pinMode(PINOS_REED[i], INPUT_PULLUP);
        quantidadeAtual[i] = 0;
        tempoEntradaEstado[i] = 0;
    }

}

//====================================================================================
//                   LOOP
//====================================================================================

void Nichos_Atualiza()
{

      if (digitalRead(PINOS_REED[indice]) == HIGH)
    {
        // Se já bateu ou passou do limite, entra avisando para lavar
        if (quantidadeAtual[indice] >= configuracao[indice].limiteMaximo)
        {
            NICHO::definir(indice, NICHO::ETAPA::PRONTO_PARA_LAVAR);
        }
        else
        {
            NICHO::definir(indice, NICHO::ETAPA::ACESSANDO);
        }
    }
}

