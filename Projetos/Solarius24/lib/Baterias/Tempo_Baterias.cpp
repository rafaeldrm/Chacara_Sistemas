//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Tempo_Baterias.h"
#include "DEBUG_MODE.h"

#include "Driver_INA3221.h"

#include "Estados_Sistema_Solarius24.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_NORMAL_LEITURA   =  15000UL; // 15 min
static constexpr uint32_t TEMPO_OBSERVACAO       = 15000UL;              // 15 s
static constexpr uint32_t TEMPO_RECUPERACAO      = 2UL * 60UL * 1000UL;  // 2 min

static uint32_t _tempoReferencia = 0;
static bool _sensorLigado = false;
static bool _permitirLeitura = false;

//====================================================================================
//                   SETUP
//====================================================================================

void Tempo_Baterias_Inicializa()
{
    debugSetupArquivo();
    debugSetupDivisao(TEXTO_AMARELO, "GERENCIADOR DE TEMPO INA3221 (BATERIAS)", TEXTO_MAGENTA, "u", 120);

    _tempoReferencia = millis();
    _sensorLigado = false;
    _permitirLeitura = false;

    debugOk("Variáveis do Tempo Baterias limpas!!!");
    
}

//====================================================================================
//                   LIGA O SENSOR
//====================================================================================

static void LigarSensor()
{
    if (!_sensorLigado)
    {
        SENSOR_INA3221_B1.Liga();
        SENSOR_INA3221_B2.Liga();

        _sensorLigado = true;
        debugLoopTexto("INA3221 LIGADO", NEGRITO | VERDE | LINHA);
    }
}

//====================================================================================
//                   DESLIGA O SENSOR
//====================================================================================

static void DesligarSensor()
{
    if (_sensorLigado)
    {
        SENSOR_INA3221_B1.Desliga();
        SENSOR_INA3221_B2.Desliga();

        _sensorLigado = false;
        debugLoopTexto("INA3221 DESLIGADO", NEGRITO | VERMELHO | LINHA);
    }
}

//====================================================================================
//                   LOOP PRINCIPAL
//====================================================================================

void Tempo_Baterias_Atualiza()
{
    uint32_t agora = millis();
    auto estado = BATERIA::atual();

    _permitirLeitura = false;

    switch (estado)
    {
        //========================================================
        // NORMAL → leitura a cada 15 min (sensor dorme)
        //========================================================

        case BATERIA::SITUACAO::NORMAL:

            if (!_sensorLigado && (agora - _tempoReferencia >= TEMPO_NORMAL_LEITURA))
            {
                LigarSensor();
                _tempoReferencia = agora;
            }

            if (_sensorLigado)
            {
                _permitirLeitura = true;

                // Faz uma leitura e já desliga
                DesligarSensor();
                _tempoReferencia = agora;
            }

        break;

        //========================================================
        // OBSERVAÇÃO → leitura constante (sensor sempre ligado)
        //========================================================

        case BATERIA::SITUACAO::OBSERVA:

            LigarSensor();

            if (agora - _tempoReferencia >= TEMPO_OBSERVACAO)
            {
                _permitirLeitura = true;
                _tempoReferencia = agora;
            }

        break;

        //========================================================
        // CALAMIDADE → leitura contínua (tempo zero)
        //========================================================

        case BATERIA::SITUACAO::CALAMIDADE_INICIO:
        case BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO:
        case BATERIA::SITUACAO::CALAMIDADE_COMPLETO:

            LigarSensor();
            _permitirLeitura = true;

        break;

        //========================================================
        // RECUPERAÇÃO → leitura a cada 2 min (sensor dorme)
        //========================================================

        case BATERIA::SITUACAO::TENTATIVA_RECUPERACAO:

            if (!_sensorLigado && (agora - _tempoReferencia >= TEMPO_RECUPERACAO))
            {
                LigarSensor();
                _tempoReferencia = agora;
            }

            if (_sensorLigado)
            {
                _permitirLeitura = true;

                DesligarSensor();
                _tempoReferencia = agora;
            }

        break;

        default:
        break;
    }
}

//====================================================================================
//                   PERMISSÃO DE LEITURA
//====================================================================================

bool Tempo_Baterias_PermiteLeitura()
{
    return _permitirLeitura;
}