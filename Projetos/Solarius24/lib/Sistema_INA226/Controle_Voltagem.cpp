//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Voltagem.h"
#include "DEBUG_MODE.h"

#include "Leitura_Voltagem.h"
#include "Diagnostico_INA226.h"
#include "Emergencia_Voltagem.h"

#include "Estados_Modulos.h"
#include "Sistema_Seguranca.h"

//====================================================================================
//                   VARIÁVEIS FIXAS INTERNAS
//====================================================================================

static constexpr uint16_t TENSAO_LIGAR_USINA      = 123; //12.3V
static constexpr uint16_t TENSAO_DESLIGAR_USINA   = 140; //14.0V
static constexpr uint16_t TENSAO_FORCADO_USINA    = 143; //14.3V
static constexpr uint16_t TENSAO_EMERGENCIA       = 121; //12.1V
static constexpr uint16_t TENSAO_CRITICA          = 120; //12.0V
static constexpr uint16_t TENSAO_RECUPERACAO      = 133; //13.3V

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static uint16_t tensao_atual = 0;   //valor atual registrado
static uint16_t ultima_tensao_valida = 0; //último valor registrado
static bool tem_dado = false; //se tiver valor (verdadeiro), se não é falso

static uint32_t tempoAnterior = 0; //registra valor "atual" do millis
static uint16_t intervalo = 15000; // intervalo de 15 segundo

//====================================================================================
// INIT
//====================================================================================

void Controle_Voltagem_Inicializa()
{
    tensao_atual = 0;
    ultima_tensao_valida = 0;
    tem_dado = false;

    debugOk("Controle Voltagem pronto");
}

//====================================================================================
// LEITURA INTELIGENTE
//====================================================================================

bool Controle_Voltagem_Atualizar_Tensao()
{
    const auto& diag = Diagnostico_INA226_ObterStatus();
    const auto& emergencia = Emergencia_Voltagem_ObterEstado();

    if (emergencia.ativo)
    {
        tem_dado = false;
        return false;
    }

    if (!Leitura_Voltagem_Tem_Novo_Dado())
    {
        tem_dado = false;
        return false;
    }

    uint16_t leitura = Leitura_Voltagem_ObterDados().media;
    Leitura_Voltagem_Consumir();

    if (diag.estado == SENSOR::OK)
    {
        tensao_atual = leitura;
        ultima_tensao_valida = leitura;
        tem_dado = true;
        return true;
    }

    if (diag.estado == SENSOR::INSTAVEL)
    {
        tensao_atual = ultima_tensao_valida;
        tem_dado = true;

        debugAviso("FALLBACK: usando última tensão válida");
        return true;
    }

    tem_dado = false;
    return false;
}

//====================================================================================
// GET
//====================================================================================

uint16_t Controle_Voltagem_Obter_Tensao()
{
    return tensao_atual;
}

bool Controle_Voltagem_Tem_Dado()
{
    return tem_dado;
}

//====================================================================================
// NORMAL
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Normal()
{
    if (!tem_dado) return ESTADO::VOLTAGEM::NIVEL::NORMAL;

    if (tensao_atual <= TENSAO_LIGAR_USINA)
    {
        debugAviso("NORMAL -> AUXILIAR");
        return ESTADO::VOLTAGEM::NIVEL::AUXILIAR;
    }

    return ESTADO::VOLTAGEM::NIVEL::NORMAL;
}

//====================================================================================
// AUXILIAR
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Auxiliar()
{
    if (!tem_dado) return ESTADO::VOLTAGEM::NIVEL::AUXILIAR;

    if (tensao_atual <= TENSAO_EMERGENCIA)
    {
        debugAviso("AUXILIAR -> EMERGENCIAL");
        return ESTADO::VOLTAGEM::NIVEL::EMERGENCIAL;
    }

    if (tensao_atual >= TENSAO_DESLIGAR_USINA)
    {
        debugOk("AUXILIAR -> NORMAL");
        return ESTADO::VOLTAGEM::NIVEL::NORMAL;
    }

    return ESTADO::VOLTAGEM::NIVEL::AUXILIAR;
}

//====================================================================================
// EMERGENCIAL
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Emergencial()
{
    if (!tem_dado) return ESTADO::VOLTAGEM::NIVEL::EMERGENCIAL;

    if (tensao_atual >= TENSAO_RECUPERACAO)
    {
        return ESTADO::VOLTAGEM::NIVEL::RECUPERADO;
    }

    if (tensao_atual <= TENSAO_CRITICA)
    {
        debugErro("EMERGENCIAL -> CRITICO");
        return ESTADO::VOLTAGEM::NIVEL::CRITICO;
    }

    return ESTADO::VOLTAGEM::NIVEL::EMERGENCIAL;
}

//====================================================================================
// CRITICO
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Critico()
{
    if (!tem_dado) return ESTADO::VOLTAGEM::NIVEL::CRITICO;

    if (tensao_atual >= TENSAO_RECUPERACAO)
    {

        return ESTADO::VOLTAGEM::NIVEL::RECUPERANDO_CARGA;
    }

    return ESTADO::VOLTAGEM::NIVEL::CRITICO;
}

//====================================================================================
//                   RECUPERANDO CARGA
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Recuperando_Carga()
{
    if (!tem_dado) 
        return ESTADO::VOLTAGEM::NIVEL::RECUPERANDO_CARGA;

    uint32_t tempoAtual = millis();

    // Caiu tensão → aborta processo
    if (tensao_atual < TENSAO_RECUPERACAO)
    {
        tempoAnterior = 0; // RESET TOTAL
        return ESTADO::VOLTAGEM::NIVEL::EMERGENCIAL;
    }

    // Primeira entrada no estado
    if (tempoAnterior == 0)
    {
        tempoAnterior = tempoAtual;
    }

    // Tempo atingido
    if (tempoAtual - tempoAnterior >= intervalo)
    {
        tempoAnterior = 0; // PREPARA PRÓXIMO CICLO
        return ESTADO::VOLTAGEM::NIVEL::RECUPERADO;
    }

    return ESTADO::VOLTAGEM::NIVEL::RECUPERANDO_CARGA;
}

//====================================================================================
//                   RECUPERADO
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Recuperado()
{
    if (!tem_dado) return ESTADO::VOLTAGEM::NIVEL::RECUPERADO;

    if (tensao_atual >= TENSAO_DESLIGAR_USINA)
    {
        debugOk("RECUPERADO -> NORMAL");
        return ESTADO::VOLTAGEM::NIVEL::NORMAL;
        }

    return ESTADO::VOLTAGEM::NIVEL::RECUPERADO;
}

//====================================================================================
// AUTOMATICO
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Automatico()
{
    if (!tem_dado) return ESTADO::VOLTAGEM::NIVEL::AUTOMATICO;

    if (tensao_atual >= TENSAO_FORCADO_USINA)
        return ESTADO::VOLTAGEM::NIVEL::FIM_AUTOMATICO;

    return ESTADO::VOLTAGEM::NIVEL::AUTOMATICO;
}

//====================================================================================
// FIM AUTOMATICO
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Fim_Automatico()
{
    return ESTADO::VOLTAGEM::NIVEL::NORMAL;
}

//====================================================================================
// CALAMIDADE
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Calamidade_Inicio()
{
    return ESTADO::VOLTAGEM::NIVEL::CALAMIDADE_INICIO;
}

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Calamidade_Completo()
{
    return ESTADO::VOLTAGEM::NIVEL::CALAMIDADE_COMPLETO;
}

//====================================================================================
// RECUPERAÇÃO
//====================================================================================

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Tentativa_Recuperar_Sistema()
{
    if (!tem_dado) return ESTADO::VOLTAGEM::NIVEL::TENTATIVA_RECUPERACAO;

    if (tensao_atual >= TENSAO_RECUPERACAO)
    {
        debugOk("RECUPERAÇÃO -> CRITICO");
        return ESTADO::VOLTAGEM::NIVEL::CRITICO;
    }

    return ESTADO::VOLTAGEM::NIVEL::TENTATIVA_RECUPERACAO;
}