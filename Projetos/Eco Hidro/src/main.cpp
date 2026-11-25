
#include <Arduino.h>
#include "DEBUG_MODE.h"

#include "Configuracao_Subsistemas.h"

#include "Protocolo_Comunicacao_Arduino.h"

#include "Atuadores/Sistema_Atuadores.h"
#include "Sistema_Boias.h"
#include "Sistema_Casa_Torre.h"
#include "Sistema_Reservatorio.h"
#include "Sistema_Protecao_Bomba_Casa.h"
#include "Mediador_Sistema.h"
#include "Sistema_LoRa.h"
#include "Sistema_Voltagem.h"
#include "Sistema_Luz_Estacionamento.h"
#include "Sistema_Luz_Acesso_Reservatorio.h"
#include "Sistema_Unidade_Potencia.h"
#include "Escalonador.h"

#include "Monitor_Latencia.h"
#include "Monitora_RAM.h"

#include "Movimento_Bomba_Casa/Controle_Tranco_Bomba_Casa.h"


//==============================================================
//                   VARIÁVEL EXTERNA
//==============================================================

static uint16_t HODOMETRO_BOOTLOADER = 351;


//==============================================================
//                       SETUP
//==============================================================

void setup()
{
    COMUNICACAO_BOOT();

    DEBUG_MODE();

    ATUADORES_BOOT();

    BOIAS_BOOT();

#if ATIVAR_VOLTAGEM
    VOLTAGEM_BOOT();
#endif

    CASA_TORRE_BOOT();

#if ATIVAR_RESERVATORIO
    RESERVATORIO_BOOT();
#endif

#if ATIVAR_LUZ_ESTACIONAMENTO
    LUZ_ESTACIONAMENTO_BOOT();
#endif

#if ATIVAR_LUZ_ACESSO_RESERVATORIO
    LUZ_ACESSO_RESERVATORIO_BOOT();
#endif

    PROTECAO_BOMBA_CASA_BOOT();

    UNIDADE_POTENCIA_BOOT();

    MEDIADOR_BOOT();

#if ATIVAR_LORA
    LORA_BOOT();
#endif

    debugVariavel("Hodômetro de Bootloader : ", HODOMETRO_BOOTLOADER, NEGRITO, VERDE, AMARELO | LINHA);

    debugAviso("Finalizado o BOOT");
}

//==============================================================
//                        LOOP
//==============================================================

void loop()
{
    BOIAS_LOOP();

#if ATIVAR_VOLTAGEM
    VOLTAGEM_LOOP();
#endif

    CASA_TORRE_LOOP();

#if ATIVAR_RESERVATORIO
    RESERVATORIO_LOOP();
#endif

#if ATIVAR_LUZ_ESTACIONAMENTO
    LUZ_ESTACIONAMENTO_LOOP();
#endif

#if ATIVAR_LUZ_ACESSO_RESERVATORIO
    LUZ_ACESSO_RESERVATORIO_LOOP();
#endif

    PROTECAO_BOMBA_CASA_LOOP();

    UNIDADE_POTENCIA_LOOP();

#if ATIVAR_LORA
    LORA_LOOP();
#endif

    MEDIADOR_LOOP();

    ESCALONADOR_LOOP();

#if ATIVAR_TRANCO_BOMBA_CASA
    Controle_Tranco_Bomba_Casa_Iniciando_Monitoramento_Helice();
#endif
}
