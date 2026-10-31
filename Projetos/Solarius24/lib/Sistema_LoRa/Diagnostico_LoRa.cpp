//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Diagnostico_LoRa.h"
#include "Driver_LoRa.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   ESTADO INTERNO
//====================================================================================

static struct
{
    uint16_t falhaEnvio;
    uint16_t falhaRecepcao;

    uint32_t ultimoUso;

    bool online;

} diag;

//====================================================================================
//                   INICIALIZAÇÃO
//====================================================================================

void LoRa_Diag_Inicializa()
{
    diag.falhaEnvio     = 0;
    diag.falhaRecepcao  = 0;
    diag.ultimoUso      = millis();
    diag.online         = false;
}

//====================================================================================
//                   REGISTROS DE EVENTOS
//====================================================================================

void LoRa_Diag_RegistrarEnvio(bool sucesso)
{
    diag.ultimoUso = millis();

    if (!sucesso)
    {
        diag.falhaEnvio++;
        diag.online = false;

        debugErro("LORA: Falha envio");
    }
    else
    {
        diag.online = true;
    }
}

//------------------------------------------------------------------------------

void LoRa_Diag_RegistrarRecepcao(bool sucesso)
{
    diag.ultimoUso = millis();

    if (!sucesso)
    {
        diag.falhaRecepcao++;

        debugErro("LORA: Falha recepcao (hash)");
    }
    else
    {
        diag.online = true;
    }
}

//====================================================================================
//                   ATUALIZAÇÃO
//====================================================================================

void LoRa_Diag_Atualiza()
{
    // Tentativa simples de recuperação
    if (diag.falhaEnvio >= 3)
    {
        debugAviso("LORA: Reiniciando modulo...");

        Driver_LoRa_Reiniciar();

        diag.falhaEnvio = 0;
        diag.online = false;
    }
}

//====================================================================================
//                   CONSULTAS
//====================================================================================

bool LoRa_Diag_EstaOnline()
{
    return diag.online;
}

//------------------------------------------------------------------------------

uint16_t LoRa_Diag_FalhaEnvio()
{
    return diag.falhaEnvio;
}

//------------------------------------------------------------------------------

uint16_t LoRa_Diag_FalhaRecepcao()
{
    return diag.falhaRecepcao;
}

//------------------------------------------------------------------------------

uint32_t LoRa_Diag_TempoSemAtividade()
{
    return millis() - diag.ultimoUso;
}