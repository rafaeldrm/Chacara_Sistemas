
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

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Boias_Inicializa()
{ 
    debugSetupDivisao(COR::AMARELO, "LEITURA BÓIAS INICIALIZANDO", COR::MAGENTA, "=", 45);
    
    mapaAtual = 0;
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
    uint16_t mapeamentoBoias = 0;
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

    return mapaAtual;
}

//====================================================================================
//                   Leitura Visual das Boias em Binário
//====================================================================================

void Leitura_Boias_Visual_Binario(uint16_t mapa)
 { 
    char binario[17]; 
    for (int8_t i = 15; i >= 0; i--) 
    { 
        binario[15 - i] = (mapa & (uint16_t(1) << i)) ? '1' : '0';
     } 
     binario[16] = '\0'; 
     debugVariavel( "Bóias posição binária : ", binario, NEGRITO, VERDE, AMARELO ); 
    }