#include "Controle_Carga_Urgencia.h"
#include "Leitura_Botao.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   ATUALIZAÇÃO DAS ENTRADAS
//====================================================================================

static void AtualizarEntradas()
{
    LEITURA_BOTAO_URGENCIA.Atualizar();
}

//====================================================================================
//                   EVENTO LORA (FUTURO)
//====================================================================================

bool Evento_Urgencia_LoRa()
{
    // Futuro:
    // Aqui você vai ler o buffer do LoRa (Comunicacao_Comandos_LoRa)
    // e retornar true quando houver comando de urgência

    return false;
}

//====================================================================================
//                   EVENTO UNIFICADO
//====================================================================================

bool Evento_Urgencia_Disparado()
{
    return LEITURA_BOTAO_URGENCIA.FoiPressionado()
        || Evento_Urgencia_LoRa();
}

//====================================================================================
//                   MODO NORMAL
//====================================================================================

ESTADO::URGENCIA::MODO Controle_Carga_Urgencia_Logica_Normal()
{
    AtualizarEntradas();

    if (Evento_Urgencia_Disparado())
    {
       
        return ESTADO::URGENCIA::MODO::AUTOMATICO;
    }

    return ESTADO::URGENCIA::MODO::NORMAL;
}

//====================================================================================
//                   MODO AUTOMÁTICO
//====================================================================================

ESTADO::URGENCIA::MODO Controle_Carga_Urgencia_Logica_Automatico()
{

    return ESTADO::URGENCIA::MODO::AUTOMATICO;
}

//====================================================================================
//                   MODO CALAMIDADE
//====================================================================================

ESTADO::URGENCIA::MODO Controle_Carga_Urgencia_Logica_Calamidade()
{
    return ESTADO::URGENCIA::MODO::CALAMIDADE;
}