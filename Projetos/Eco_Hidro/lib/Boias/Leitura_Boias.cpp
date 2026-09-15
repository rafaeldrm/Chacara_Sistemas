
//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Boias.h"
#include "DEBUG_MODE.h"

#include "Driver_Boias.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static uint16_t mapaAtual = 0;
static uint16_t mapeamentoBoias = 0;

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Boias_Inicializa()
{ 
    debugSetupTexto("Inicializando Leitura das Boias", NEGRITO | SUBLINHADO | AZUL | LINHA);
    mapaAtual = 0;
    mapeamentoBoias = 0;
}

//====================================================================================
//                   LOOP
//====================================================================================

void Leitura_Boias_Atualiza()
{
    mapaAtual = Leitura_Boias_Montar_Mapa_Boias();
}

//====================================================================================
//                   MAPEANDO AS BOIAS
//====================================================================================

uint16_t Leitura_Boias_Montar_Mapa_Boias()
{
    mapeamentoBoias = 0;
    for(uint16_t i = 0; i < REGISTRO_BOIAS::TOTAL_BOIAS; i++)
    {
        if (Driver_Boias_Ler((REGISTRO_BOIAS::BOIA_BIT)i))
        {
            mapeamentoBoias |= (uint16_t(1) << i);
        }
    }

    return mapeamentoBoias;
}

//====================================================================================
//                   FUNÇÃO DE LEITURA DA BÓIA ATIVA
//====================================================================================

bool Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT boia)
{
    return (mapaAtual & (uint16_t(1) << boia)) != 0;
}

//====================================================================================
//                   MAPA COMPLETO DE TODAS AS BÓIAS
//====================================================================================

uint16_t Leitura_Boias_Mapa_Completo()
{
    debugVariavel("Boias Ordem ", mapaAtual, NEGRITO, VERDE, AMARELO);
    return mapaAtual;
}