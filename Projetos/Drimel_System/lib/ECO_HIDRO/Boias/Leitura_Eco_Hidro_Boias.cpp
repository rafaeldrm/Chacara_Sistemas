//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Eco_Hidro_Boias.h"
#include "DEBUG_MODE.h"

#include "Contrato_Boias_Chacara.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static uint16_t mapaAtual = 0;

static bool mapaValido = false;   // false = ainda não chegou NENHUM mapa da Eco Hidro (Drimel fica mudo)

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Eco_Hidro_Boias_Inicializa()
{
    mapaAtual = 0;

    mapaValido = false;
}

//====================================================================================
//                   LOOP (chamado quando chega a mensagem LoRa)
//====================================================================================

void Leitura_Eco_Hidro_Boias_Atualiza(uint16_t novoMapa)
{
    if (!mapaValido)
    {
        debugOk("Eco Hidro: primeiro mapa de boias recebido");
    }

    mapaAtual = novoMapa;

    mapaValido = true;
}

//====================================================================================
//                   TEM DADO? (QUEM USA O MAPA PERGUNTA ISSO PRIMEIRO)
//====================================================================================

bool Leitura_Eco_Hidro_Boias_Tem_Dado()
{
    return mapaValido;
}

//====================================================================================
//                   LEITURA BOIAS ESTA ATIVA
//====================================================================================

bool Leitura_Eco_Hidro_Boias_Esta_Ativa(REGISTRO_BOIAS::BOIA_BIT boia)
{
    return (mapaAtual & (uint16_t(1) << boia)) != 0;
}

//====================================================================================
//                   MAPA COMPLETO
//====================================================================================

uint16_t Leitura_Eco_Hidro_Boias_Mapa_Completo()
{
    return mapaAtual;
}