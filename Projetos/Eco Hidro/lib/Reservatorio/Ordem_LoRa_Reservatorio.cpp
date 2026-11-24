//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Ordem_LoRa_Reservatorio.h"
#include "DEBUG_MODE.h"

#include "FSM/Estados_Sistema_Eco_Hidro.h"
#include "Eventos_FSM_LoRa/Estados_Eventos_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void Ordem_LoRa_Reservatorio_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "ORDEM LORA RESERVATORIO INICIALIZANDO", COR::MAGENTA, "=", 0);
}

//====================================================================================
//                   TRANSFERÊNCIA DE ÁGUA ENTRE RESERVATÓRIOS 1 E 2
//====================================================================================

static void Ordem_LoRa_Reservatorio_Transferencia_Agua()
{
    const auto ordemReservatorio = ORDENS_DRIMEL_SYSTEM::RESERVATORIO::atual();

    if (ordemReservatorio == ORDENS_DRIMEL_SYSTEM::RESERVATORIO::ORDEM::TRANSFERENCIA_ENTRE_RESERVATORIOS)
    {
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::LIBERADO);
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::LIBERADO);
        ORDENS_DRIMEL_SYSTEM::RESERVATORIO::definir(ORDENS_DRIMEL_SYSTEM::RESERVATORIO::ORDEM::NENHUM);
    }
}

//====================================================================================
//                   DESLIGA RESERVATORIO
//====================================================================================

static void Ordem_LoRa_Reservatorio_Desliga()
{
    const auto ordemReservatorio = ORDENS_DRIMEL_SYSTEM::RESERVATORIO::atual();

    if (ordemReservatorio == ORDENS_DRIMEL_SYSTEM::RESERVATORIO::ORDEM::DESLIGA)
    {
        RESERVATORIO::definir(RESERVATORIO::TRANSFERENCIA::FINALIZADO);
        BOMBA_12V::definir(BOMBA_12V::ESTAGIOS::DESLIGANDO_BOMBA);
        ORDENS_DRIMEL_SYSTEM::RESERVATORIO::definir(ORDENS_DRIMEL_SYSTEM::RESERVATORIO::ORDEM::NENHUM);
    }
}


//====================================================================================
//                   LOOP
//====================================================================================

void Ordem_LoRa_Reservatorio_Atualiza()
{
    Ordem_LoRa_Reservatorio_Transferencia_Agua();

    Ordem_LoRa_Reservatorio_Desliga();
}
