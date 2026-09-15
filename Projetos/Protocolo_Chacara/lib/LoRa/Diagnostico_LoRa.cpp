//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Diagnostico_LoRa.h"
#include "Driver_LoRa.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

//====================================================================================
//                   CONFIGURAÇÕES
//====================================================================================

static constexpr uint8_t  MAX_FALHAS_REINICIO     = 3;
static constexpr uint32_t TEMPO_MINIMO_RESTART    = TEMPO::SEGUNDOS(10);
static constexpr uint32_t TEMPO_TIMEOUT_LORA      = TEMPO::SEGUNDOS(60);
static constexpr uint16_t VALOR_INCREMENTAL_FALHAS = 65535;

static CRONOMETRO::Temporizador ultimoUsoLoRa;
static CRONOMETRO::Temporizador ultimoReinicioLoRa;

//====================================================================================
//                   ESTADO INTERNO
//====================================================================================

static struct
{
    uint16_t falhaEnvio;
    uint16_t falhaRecepcao;

    bool online;

} diagnostico;

//====================================================================================
//                   FUNÇÕES INTERNAS
//====================================================================================

static void IncrementarFalha(uint16_t& contador)
{
    if (contador < VALOR_INCREMENTAL_FALHAS)
        contador++;
}

//====================================================================================
//                   INICIALIZAÇÃO
//====================================================================================

void LoRa_Diagnostico_Inicializa()
{
    diagnostico.falhaEnvio    = 0;
    diagnostico.falhaRecepcao = 0;
    diagnostico.online        = false;

    ultimoUsoLoRa.Cronometro_Iniciar();

    // Permite o primeiro reinício imediato caso necessário
    ultimoReinicioLoRa.Cronometro_Cancelar();
}

//====================================================================================
//                   REGISTROS DE EVENTOS
//====================================================================================

void LoRa_Diagnostico_RegistrarEnvio(bool sucesso)
{
    ultimoUsoLoRa.Cronometro_Iniciar();

    if (!sucesso)
    {
        IncrementarFalha(diagnostico.falhaEnvio);

        diagnostico.online = false;

        debugErro("LORA: Falha envio");
    }
    else
    {
        diagnostico.online = true;
    }
}

//====================================================================================
//                   REGISTRAR RECEPÇÃO
//====================================================================================

void LoRa_Diagnostico_RegistrarRecepcao(bool sucesso)
{
    ultimoUsoLoRa.Cronometro_Iniciar();

    if (!sucesso)
    {
        IncrementarFalha(diagnostico.falhaRecepcao);

        debugErro("LORA: Falha recepcao (hash)");
    }
    else
    {
        diagnostico.online = true;
    }
}

//====================================================================================
//                   ATUALIZA
//====================================================================================

void LoRa_Diagnostico_Atualiza()
{
    // Timeout sem comunicação
    if (ultimoUsoLoRa.Cronometro_Dispara_Uma_Vez(TEMPO_TIMEOUT_LORA))
    {
        diagnostico.online = false;
    }

    // Ainda não atingiu o número de falhas
    if (diagnostico.falhaEnvio < MAX_FALHAS_REINICIO)
        return;

    // Evita reinicializações em sequência
  //  if (ultimoReinicioLoRa.ativo &&
        !ultimoReinicioLoRa.Cronometro_Dispara_Uma_Vez(TEMPO_MINIMO_RESTART);
    {
        return;
    }

    debugAviso("LORA: Reiniciando modulo...");

    ultimoReinicioLoRa.Cronometro_Iniciar();

    if (Driver_LoRa_Reiniciar())
    {
        diagnostico.online = true;

        diagnostico.falhaEnvio    = 0;
        diagnostico.falhaRecepcao = 0;

        ultimoUsoLoRa.Cronometro_Iniciar();

        debugAviso("LORA: Modulo recuperado");
    }
    else
    {
        diagnostico.online = false;

        debugErro("LORA: Falha reinicializacao");
    }
}

//====================================================================================
//                   CONSULTAS
//====================================================================================

bool LoRa_Diagnostico_EstaOnline()
{
    return diagnostico.online;
}

//====================================================================================
//                   FALHA DE ENVIO
//====================================================================================

uint16_t LoRa_Diagnostico_FalhaEnvio()
{
    return diagnostico.falhaEnvio;
}

//====================================================================================
//                   FALHA DE RECEPÇÃO
//====================================================================================

uint16_t LoRa_Diagnostico_FalhaRecepcao()
{
    return diagnostico.falhaRecepcao;
}

//====================================================================================
//                   TEMPO SEM ATIVIDADES
//====================================================================================

uint32_t LoRa_Diagnostico_TempoSemAtividade()
{
    if (!ultimoUsoLoRa.Cronometro_Verificar())
        return 0;

  //  return millis() - ultimoUsoLoRa._inicio;
}

//====================================================================================
//                   TEMPO ÚLTIMO REINICIO
//====================================================================================

uint32_t LoRa_Diagnostico_TempoUltimoReinicio()
{
    if (!ultimoReinicioLoRa.Cronometro_Verificar())
        return 0;

   /// return millis() - ultimoReinicioLoRa.inicio;
}