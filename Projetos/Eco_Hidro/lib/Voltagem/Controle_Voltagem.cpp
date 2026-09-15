//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Voltagem.h"
#include "DEBUG_MODE.h"

#include "Leitura_Bateria.h"
#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint8_t VOLTAGEM_CHEIA             = 144;  // 14.4 V  → entra em CHEIA
static constexpr uint8_t VOLTAGEM_MANTENDO_CARGA    = 143;  // 14.3 V  → mantém / consome
static constexpr uint8_t VOLTAGEM_CARGA_CONSUMIDA   = 142;  // 14.2 V  → consumo realizado
static constexpr uint8_t VOLTAGEM_IDEAL             = 130;  // 13.0 V
static constexpr uint8_t VOLTAGEM_NORMAL            = 126;  // 12.6 V
static constexpr uint8_t VOLTAGEM_CRITICO           = 121;  // 12.1 V

static uint8_t monitoraVoltagem = 0;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Voltagem_Inicializa()
{


}

//====================================================================================
//                   LOOP DE LEITURA DA VOLTAGEM
//====================================================================================

void Controle_Voltagem_Atualiza()
{
    monitoraVoltagem  = Leitura_Bateria_Entrega();
}

//====================================================================================
//                   VOLTAGEM NORMAL
//====================================================================================

void Controle_Voltagem_Normal()
{
    Controle_Voltagem_Atualiza();
   
    if (monitoraVoltagem >= VOLTAGEM_IDEAL)
    {
        VOLTAGEM::definir(VOLTAGEM::LEITURA::IDEAL);
    }
    else if (monitoraVoltagem <= VOLTAGEM_CRITICO) 
    {
        VOLTAGEM::definir(VOLTAGEM::LEITURA::CRITICO);
    }
}

//====================================================================================
//                   VOLTAGEM IDEAL
//====================================================================================

void Controle_Voltagem_Ideal()
{
    Controle_Voltagem_Atualiza();

    if (monitoraVoltagem >= VOLTAGEM_CHEIA)
    {
        VOLTAGEM::definir(VOLTAGEM::LEITURA::CHEIA);
    }
    else if (monitoraVoltagem <= VOLTAGEM_NORMAL) 
    {
        VOLTAGEM::definir(VOLTAGEM::LEITURA::NORMAL);
    }
}

//====================================================================================
//                   VOLTAGEM CHEIA
//====================================================================================

void Controle_Voltagem_Cheia()
{
    Controle_Voltagem_Atualiza();

    // Só desce para MANTENDO_CARGA quando cair abaixo de 14.3 V
    if (monitoraVoltagem <= VOLTAGEM_MANTENDO_CARGA)
    {
        VOLTAGEM::definir(VOLTAGEM::LEITURA::MANTENDO_CARGA);
    }
    // Se continuar >= 14.4 V, permanece em CHEIA (não faz nada)
}

//====================================================================================
//                   VOLTAGEM MANTENDO CARGA
//====================================================================================

void Controle_Voltagem_Mantendo_Carga()
{
    Controle_Voltagem_Atualiza();

    if (monitoraVoltagem >= VOLTAGEM_CHEIA)
    {
        // Subiu de novo → volta para CHEIA
        VOLTAGEM::definir(VOLTAGEM::LEITURA::CHEIA);
    }
    else if (monitoraVoltagem <= VOLTAGEM_CARGA_CONSUMIDA)
    {
        // Desceu o suficiente → consumo realizado
        VOLTAGEM::definir(VOLTAGEM::LEITURA::CARGA_CONSUMIDA);
    }
}

//====================================================================================
//                   VOLTAGEM CONSUMO REALIZADO
//====================================================================================

void Controle_Voltagem_Carga_Consumida()
{
    Controle_Voltagem_Atualiza();

    if (monitoraVoltagem >= VOLTAGEM_MANTENDO_CARGA)
    {
        // Subiu de novo para a zona de float → volta a manter carga
        VOLTAGEM::definir(VOLTAGEM::LEITURA::MANTENDO_CARGA);
    }
    else if (monitoraVoltagem <= VOLTAGEM_IDEAL)
    {
        // Caiu bastante → volta para IDEAL
        VOLTAGEM::definir(VOLTAGEM::LEITURA::IDEAL);
    }
}

//====================================================================================
//                   VOLTAGEM CRÍTICO
//====================================================================================

void Controle_Voltagem_Critico()
{
    Controle_Voltagem_Atualiza();

    if (monitoraVoltagem >= VOLTAGEM_NORMAL)
    {
        VOLTAGEM::definir(VOLTAGEM::LEITURA::NORMAL);
    }
}