//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Gerencia_Nicho_Roupas_Sujas.h"
#include "DEBUG_MODE.h"

#include "Controle_Nicho_Roupas_Sujas.h"
#include "Estado_Sistema_Lavanderia.h"

#include "Tabela_Categorias_Nichos.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================



//====================================================================================
//                   SETUP
//====================================================================================

void Gerencia_Nicho_Roupas_Sujas_Inicializa()
{



}

//====================================================================================
//                   LOOP
//====================================================================================

void Gerencia_Nicho_Roupas_Sujas_Atualiza()
{
    Gerencia_Nicho_Roupas_Sujas_Lendo_Nicho_A_Nicho();
}

static void Gerencia_Nicho_Roupas_Sujas_Lendo_Nicho_A_Nicho()
{
    for (uint8_t i = 0; i < (uint8_t)NICHO_CATEGORIA::TOTAL_CATEGORIAS; i++)
    {
        Nicho_Atual_Lido(i);
    }
}

static void Nicho_Atual_Lido(uint8_t indice)
{
    switch (NICHO::atual(indice))
    {
        case NICHO::ETAPA::NENHUM:
            Controle_Nicho_Roupas_Sujas_Nenhum(indice);
            break;

        case NICHO::ETAPA::ACESSANDO:
            Controle_Nicho_Roupas_Sujas_Acessando(indice);
            break;

        case NICHO::ETAPA::CONTABILIZANDO:
            Controle_Nicho_Roupas_Sujas_Contabilizando(indice);
            break;

        case NICHO::ETAPA::PRONTO_PARA_LAVAR:
            Controle_Nicho_Roupas_Sujas_Pronto_Para_Lavar(indice);
            break;

        case NICHO::ETAPA::RESETANDO:
            Controle_Nicho_Roupas_Sujas_Resetando(indice);
            break;

        case NICHO::ETAPA::FINALIZADO:
            Controle_Nicho_Roupas_Sujas_Finalizado(indice);
            break;

        default:

            NICHO::definir(indice, NICHO::ETAPA::NENHUM);
            break;
    }
}

