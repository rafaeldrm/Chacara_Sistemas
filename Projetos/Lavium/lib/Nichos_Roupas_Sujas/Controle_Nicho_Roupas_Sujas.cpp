//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Nicho_Roupas_Sujas.h"
#include "Mapa_Pinos_Arduino.h"
#include "Tabela_Categorias_Nichos.h"
#include "Estado_Sistema_Lavanderia.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

//====================================================================================
//                   CONSTANTES E CONFIGURAÇÕES
//====================================================================================

constexpr uint8_t QUANTIDADE_NICHOS = static_cast<uint8_t>(NICHO_CATEGORIA::TOTAL_CATEGORIAS);
static constexpr uint32_t TEMPO_PROTECAO_LEITURA = TEMPO::MINUTOS(1); // 1 minuto de descanso contra falsos disparos
static CRONOMETRO::Temporizador contadorTempo;



//====================================================================================
//                   VARIÁVEIS INTERNAS DA CAMADA DE CONTROLE
//====================================================================================

static uint8_t  quantidadeAtual[QUANTIDADE_NICHOS]   = {0};
static uint32_t tempoEntradaEstado[QUANTIDADE_NICHOS] = {0};

//====================================================================================
//                   INICIALIZAÇÃO DE HARDWARE
//====================================================================================

void Controle_Nicho_Roupas_Sujas_Inicializa()
{

}

//====================================================================================
//                   MÁQUINA DE ESTADOS - REGRAS DE NEGÓCIO
//====================================================================================

// State 1: Repouso (Porta Fechada)
void Controle_Nicho_Roupas_Sujas_Nenhum(uint8_t indice)
{
    // Verifica se a porta foi ABERTA (HIGH = Ímã longe)
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

// State 2: Porta Aberta em uso normal
void Controle_Nicho_Roupas_Sujas_Acessando(uint8_t indice)
{
    // Aguarda a porta FECHAR (LOW = Ímã encostou)
    if (digitalRead(PINOS_REED[indice]) == LOW)
    {
        NICHO::definir(indice, NICHO::ETAPA::CONTABILIZANDO);
    }
}

// State 3: Porta acabou de fechar, incrementa e vai para o lockout
void Controle_Nicho_Roupas_Sujas_Contabilizando(uint8_t indice)
{
    quantidadeAtual[indice]++;
    
    debugVariavel("Nicho : ", configuracao[indice].nome, NENHUM, VERDE, AMARELO);
    debugVariavel("Qtd Atual: ", quantidadeAtual[indice], NENHUM, VERDE, AMARELO);

    contadorTempo.Cronometro_Iniciar();

    tempoEntradaEstado[indice] = contadorTempo;
    NICHO::definir(indice, NICHO::ETAPA::FINALIZADO);
}

// State 4: Porta Aberta quando o nicho já estava CHEIO (Retirada de roupas)
void Controle_Nicho_Roupas_Sujas_Pronto_Para_Lavar(uint8_t indice)
{
    // Aguarda você esvaziar, colocar a nova peça e FECHAR a porta (LOW)
    if (digitalRead(PINOS_REED[indice]) == LOW)
    {
        NICHO::definir(indice, NICHO::ETAPA::RESETANDO);
    }
}

// State 5: Esvaziou o nicho e deixou a peça nova
void Controle_Nicho_Roupas_Sujas_Resetando(uint8_t indice)
{
    // Define a contagem como 1 (a nova peça inserida)
    quantidadeAtual[indice] = 1;

    debugVariavel("Nicho Resetado: ", configuracao[indice].nome, NENHUM, VERDE, AMARELO);

    tempoEntradaEstado[indice] = millis();
    NICHO::definir(indice, NICHO::ETAPA::FINALIZADO);
}

// State 6: Bloqueio de 1 minuto (Cooldown contra vibrações da porta de papelão)
void Controle_Nicho_Roupas_Sujas_Finalizado(uint8_t indice)
{
    // Permanece travado neste estado até passar 60.000 ms (1 minuto)
    if (millis() - tempoEntradaEstado[indice] >= TEMPO_PROTECAO_LEITURA)
    {
        NICHO::definir(indice, NICHO::ETAPA::NENHUM);
    }
}

//====================================================================================
//                   FUNÇÕES AUXILIARES DE CONSULTA
//====================================================================================

uint8_t Controle_Nicho_Roupas_Sujas_Obter_Quantidade(uint8_t indice)
{
    if (indice >= QUANTIDADE_NICHOS) return 0;
    return quantidadeAtual[indice];
}