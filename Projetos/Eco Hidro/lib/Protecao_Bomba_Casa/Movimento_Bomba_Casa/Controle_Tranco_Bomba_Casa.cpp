//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Tranco_Bomba_Casa.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Atuadores/Modulo_Reles.h"
#include "Leitura_Movimento_Bomba_Casa.h"
#include "Notificacao_Tranco_Bomba_Casa.h"

#include "Tempo.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_ESPERA_ENTRE_TRANCOS = TEMPO::SEGUNDOS(1);

static constexpr uint32_t TEMPO_RESFRIAMENTO_ENTRE_ESTRATEGIAS = TEMPO::MINUTOS(1);

static constexpr uint32_t TEMPO_ESPERA_ENTRE_TRANCOS_ULTIMA_TENTATIVA = TEMPO::SEGUNDOS(5);

static constexpr uint8_t MAX_TENTATIVAS = 3;

// Mínimo de alternâncias do IR para considerar movimento da hélice
static constexpr uint8_t MINIMO_TRANSICOES = 100;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Tranco_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "CONTROLE TRANCO BOMBA INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   LOOP
//====================================================================================

void Controle_Tranco_Bomba_Casa_Atualiza()
{

}

//====================================================================================
//                   Iniciando_Monitoramento_Helice
//====================================================================================

void Controle_Tranco_Bomba_Casa_Iniciando_Monitoramento_Helice()
{

  const uint8_t transicoes =
            Leitura_Movimento_Bomba_Casa_Verificar();

    //    Leitura_Movimento_Bomba_Casa_Desligar();
debugVariavel("Leitura Sensor IR : ", transicoes, NEGRITO, LARANJA, ROXO | LINHA);
   /*     if (transicoes >= MINIMO_TRANSICOES)
        {
            TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTADO::BOMBA_LIBERADA);

            return;
        }

        RELE_BOMBA_CASA.Desligar();
        TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTADO::BOMBA_TRAVADA);
*/}


//====================================================================================
//                   Travada
//====================================================================================

void Controle_Tranco_Bomba_Casa_Travada()
{
        Notificacao_Tranco_Bomba_Casa_Travada();
        TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTADO::PRIMEIRA_TENTATIVA_TRANCO);
}

//====================================================================================
//                   Primeira_Tentativa_Tranco
//====================================================================================

void Controle_Tranco_Bomba_Casa_Primeira_Tentativa_Tranco()
{
    for (uint8_t tentativa = 1; tentativa <= MAX_TENTATIVAS; tentativa++)
    {
        RELE_BOMBA_CASA.Desligar();

        delay(TEMPO_ESPERA_ENTRE_TRANCOS);

        RELE_BOMBA_CASA.Ligar();

        const uint8_t transicoes =
            Leitura_Movimento_Bomba_Casa_Verificar();

        Leitura_Movimento_Bomba_Casa_Desligar();

        if (transicoes >= MINIMO_TRANSICOES)
        {
            TRANCO_BOMBA_CASA::definir(
                TRANCO_BOMBA_CASA::ESTADO::BOMBA_LIBERADA
            );

            return;
        }

        RELE_BOMBA_CASA.Desligar();

        if (tentativa < MAX_TENTATIVAS)
        {
            delay(TEMPO_ESPERA_ENTRE_TRANCOS);
        }
    }

    // As três tentativas falharam.

    delay(TEMPO_RESFRIAMENTO_ENTRE_ESTRATEGIAS);

    TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTADO::SEGUNDA_TENTATIVA_TRANCO);
}

//====================================================================================
//                   Segunda_Tentativa_Tranco
//====================================================================================

void Controle_Tranco_Bomba_Casa_Segunda_Tentativa_Tranco()
{

        for (uint8_t tentativa = 1; tentativa <= MAX_TENTATIVAS; tentativa++)
        {
                RELE_BOMBA_CASA.Desligar();

                delay(TEMPO_RESFRIAMENTO_ENTRE_ESTRATEGIAS);

                RELE_BOMBA_CASA.Ligar();

                const uint8_t transicoes = Leitura_Movimento_Bomba_Casa_Verificar();

                Leitura_Movimento_Bomba_Casa_Desligar();

        if (transicoes >= MINIMO_TRANSICOES)
        {
                TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTADO::BOMBA_LIBERADA);

            return;
        }

        RELE_BOMBA_CASA.Desligar();

        if (tentativa < MAX_TENTATIVAS)
        {
            delay(TEMPO_RESFRIAMENTO_ENTRE_ESTRATEGIAS);
        }
    }

    // As três tentativas falharam.

    TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTADO::SENSOR_IR_DESATIVADO);
}

//====================================================================================
//                   Problemas
//====================================================================================

void Controle_Tranco_Bomba_Casa_Sensor_Ir_Desativado()
{

    //fica aguardando o Drimel System liberar a bomba. Pois o sensor Ir está inseguro.
}

//====================================================================================
//                   Liberada
//====================================================================================

void Controle_Tranco_Bomba_Casa_Liberada()
{
    TRANCO_BOMBA_CASA::definir(TRANCO_BOMBA_CASA::ESTADO::REPOUSO);
}