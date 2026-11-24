//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Termico_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"
#include "Leitura_Temperatura.h"

#include "Cronometro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr int16_t TEMPERATURA_LIMITE_MAXIMO = 450; // 45.0 °C
static constexpr int16_t TEMPERATURA_LIMITE_MINIMO = 280; // 28.0 °C

static uint8_t falhas = 0;

static int16_t temperatura = 0;

static CASA_TORRE::ETAPAS posicaoCasaTorreSalvo = CASA_TORRE::ETAPAS::NORMAL;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Termico_Bomba_Casa_Inicializa()
{
    debugSetupDivisao(COR::AMARELO, "CONTROLE TERMICO BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   LOOP
//====================================================================================

bool Controle_Termico_Bomba_Casa_Atualiza()
{
    const auto bombaCasa = BOMBA_CASA::atual();

    if (bombaCasa != BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA)
    {
        return false;
    }

    Leitura_Temperatura_Atualiza();

    temperatura = Leitura_Temperatura_Obter_Temperatura();
    debugVariavel("Temperatura Bomba : ", temperatura, NEGRITO, VERDE_NEON, LARANJA);

    if (temperatura < 0)
    {
        falhas++;

        if (falhas >= 20)
        {
            DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::DHT11_ERRO);
        }

        return false;
    }
    else
    {
        falhas = 0;
        return true;
    }
}

//====================================================================================
//                   MONITORAMENTO ATIVADO
//====================================================================================

void Controle_Termico_Bomba_Casa_Monitoramento_Ativado()
{
    // caso a bomba não esteja ligada nem continua com o resto das condições.
    if (!Controle_Termico_Bomba_Casa_Atualiza())
        return;

    //=========================================

    if (temperatura >= TEMPERATURA_LIMITE_MAXIMO)
    {

        const auto casaTorre = CASA_TORRE::atual();

        // Verifica se está no Primeiro Ciclo da Casa Torre
        if (casaTorre == CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA)
        {
            posicaoCasaTorreSalvo = casaTorre;
            PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::SUPERAQUECIMENTO);
        }

        //===============================================
        // Verifica se Está no Segundo Ciclo da Casa Torre
        if (casaTorre == CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA)
        {
            posicaoCasaTorreSalvo = casaTorre;
            PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::SUPERAQUECIMENTO);
        }
    }
}

//====================================================================================
//                   SUPERAQUECIMENTO
//====================================================================================

void Controle_Termico_Bomba_Casa_Superaquecimento()
{
    // 2. Verifica temperatura
    Leitura_Temperatura_Atualiza();

    temperatura = Leitura_Temperatura_Obter_Temperatura();
    debugVariavel("Temperatura Bomba : ", temperatura, NEGRITO, VERDE_NEON, LARANJA);

    if (temperatura < 0)
    {
        falhas++;

        //====================

        if (falhas >= 20)
        { // ~20s sem leitura válida
            DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::DHT11_ERRO);
            return;
        }
    }
    else
    {
        falhas = 0;

        //========================

        if (temperatura <= TEMPERATURA_LIMITE_MINIMO)
        {

            //================================

            if (posicaoCasaTorreSalvo == CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA)
            {
                PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::MODO_SEGURANCA_PRIMEIRO_CICLO);
            }

            //====================================================

            if (posicaoCasaTorreSalvo == CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA)
            {
                PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::MODO_SEGURANCA_SEGUNDO_CICLO);
            }
        }
    }
}

//====================================================================================
//                   MODO SEGURANCA PRIMEIRO CICLO
//====================================================================================

void Controle_Termico_Bomba_Casa_Modo_Seguranca_Primeiro_Ciclo()
{

    // caso a bomba não esteja ligada nem continua com o resto das condições.
    if (!Controle_Termico_Bomba_Casa_Atualiza())
        return;
  
        //=====================================
    
       if (temperatura >= TEMPERATURA_LIMITE_MAXIMO)
    {
        const auto casaTorre = CASA_TORRE::atual();

        // Verifica se está no Primeiro Ciclo da Casa Torre
        if (casaTorre == CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA)
        {
            posicaoCasaTorreSalvo = casaTorre;
            PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::SUPERAQUECIMENTO);
        }     
    }

}

//====================================================================================
//                   MODO SEGURANCA SEGUNDO CICLO
//====================================================================================

void Controle_Termico_Bomba_Casa_Modo_Seguranca_Segundo_Ciclo()
{
     // caso a bomba não esteja ligada nem continua com o resto das condições.
    if (!Controle_Termico_Bomba_Casa_Atualiza())
        return;
  
        //======================================
    
       if (temperatura >= TEMPERATURA_LIMITE_MAXIMO)
    {
        const auto casaTorre = CASA_TORRE::atual();

        //===============================================
        // Verifica se Está no Segundo Ciclo da Casa Torre
        if (casaTorre == CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA)
        {
            posicaoCasaTorreSalvo = casaTorre;
            PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::SUPERAQUECIMENTO);
        }
    }
}

//====================================================================================
//                   FINALIZADO
//====================================================================================

void Controle_Termico_Bomba_Casa_Finalizado()
{
    
    posicaoCasaTorreSalvo = CASA_TORRE::ETAPAS::NORMAL;
    PROTECAO_TERMICA_BOMBA_CASA::definir(PROTECAO_TERMICA_BOMBA_CASA::SITUACAO::REPOUSO);

}