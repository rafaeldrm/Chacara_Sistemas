//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Tempo_Climatizacao.h"
#include "DEBUG_MODE.h"
#include "Leitura_Temperatura.h"

//====================================================================================
//                   CONSTANTES
//====================================================================================

// Tempo dormindo (economia)
static constexpr uint32_t TEMPO_DORMINDO   = 2UL * 60UL * 1000UL; // 2 minutos

// Tempo máximo coletando (segurança)
static constexpr uint32_t TEMPO_COLETANDO  = 15UL * 1000UL; // 15 segundos

//====================================================================================
//                   ESTADO INTERNO
//====================================================================================

enum class EstadoTempo : uint8_t
{
    DORMINDO,
    LIBERADO
};

static EstadoTempo _estado = EstadoTempo::DORMINDO;
static uint32_t _tempoReferencia = 0;

static EstadoTempoClimatizacao _dados;

//====================================================================================
//                   SETUP
//====================================================================================

void Tempo_Climatizacao_Inicializa()
{
   

    _estado = EstadoTempo::DORMINDO;
    _tempoReferencia = millis();

    _dados.liberado = false;

   
}

//====================================================================================
//                   LOOP
//====================================================================================

void Tempo_Climatizacao_Atualiza()
{
    uint32_t agora = millis();

    switch (_estado)
    {
        //===============================================================================
        // MODO ECONOMIA
        //===============================================================================
        case EstadoTempo::DORMINDO:

            _dados.liberado = false;

            if (agora - _tempoReferencia >= TEMPO_DORMINDO)
            {
                _estado = EstadoTempo::LIBERADO;
                _tempoReferencia = agora;

                // 💎 Nova janela começa limpa
                Leitura_Temperatura_Reset();
            }

            break;

        //===============================================================================
        // JANELA DE COLETA
        //===============================================================================
        case EstadoTempo::LIBERADO:
        {
            _dados.liberado = true;

            const auto& leitura = Leitura_Temperatura_ObterDados();

            // 💎 PRIORIDADE: leitura terminou corretamente
            if (leitura.leitura_finalizada)
            {
                _estado = EstadoTempo::DORMINDO;
                _tempoReferencia = agora;
                break;
            }

            // 💎 FAIL-SAFE: tempo máximo estourou
            if (agora - _tempoReferencia >= TEMPO_COLETANDO)
            {
                _estado = EstadoTempo::DORMINDO;
                _tempoReferencia = agora;
            }

            break;
        }
    }
}

//====================================================================================
//                   ACESSO
//====================================================================================

const EstadoTempoClimatizacao& Tempo_Climatizacao_ObterEstado()
{
    return _dados;
}