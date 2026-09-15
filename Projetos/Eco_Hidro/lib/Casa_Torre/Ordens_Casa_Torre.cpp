//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Ordens_Casa_Torre.h"
#include "DEBUG_MODE.h"

#include "Estados_Sistema_Eco_Hidro.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static void Ordens_Casa_Torre_Ciclo_Completo_Abastecimento();
static void Ordens_Casa_Torre_Meio_Ciclo_Abastecimento();
static void Ordens_Casa_Torre_Desliga_Bomba();


//====================================================================================
//                   SETUP
//====================================================================================

void Ordens_Casa_Torre_Inicializa()
{



}

//====================================================================================
//                   LOOP
//====================================================================================

void Ordens_Casa_Torre_Atualiza()
{

    switch (ORDENS_DRIMEL_SYSTEM::CASA_TORRE::atual())
    {

        case ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM:
            // Nenhuma ação necessária para a ordem NENHUM
            break;
        
        //============================================================

        case ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::CICLO_COMPLETO:
            Ordens_Casa_Torre_Ciclo_Completo_Abastecimento();
            break;

        //============================================================

        case ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::MEIO_CICLO:
            Ordens_Casa_Torre_Meio_Ciclo_Abastecimento();
            break;

        //============================================================

        case ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::DESLIGA:
            Ordens_Casa_Torre_Desliga_Bomba();
            break;

        //============================================================

        default:
           ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
            break;
    }
}



//====================================================================================
//                  LORA :: ORDEM DRIMEL_SYSTEM CICLO COMPLETO DE ABASTECIMENTO CASA / TORRE
//====================================================================================

static void Ordens_Casa_Torre_Ciclo_Completo_Abastecimento()
 {
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::PRIMEIRO_CICLO_ENCHIMENTO_CAIXA);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIBERADO);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
        POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_CASA);
    }

//------------------------  LORA :: ORDEM DRIMEL_SYSTEM MEIO CICLO DE ABASTECIMENTO CASA / TORRE ------------------------------------------------

static void Ordens_Casa_Torre_Meio_Ciclo_Abastecimento()
{

        CASA_TORRE::definir(CASA_TORRE::ETAPAS::SEGUNDO_CICLO_ENCHIMENTO_CAIXA);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::LIBERADO);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
        POTENCIA::definir(POTENCIA::SELECIONADO::BOMBA_CASA);

 }

//------------------------  LORA :: ORDEM DRIMEL_SYSTEM DESLIGA BOMBA CASA / TORRE ------------------------------------------------

static void Ordens_Casa_Torre_Desliga_Bomba()
{
        CASA_TORRE::definir(CASA_TORRE::ETAPAS::NORMAL);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
        ORDENS_DRIMEL_SYSTEM::CASA_TORRE::definir(ORDENS_DRIMEL_SYSTEM::CASA_TORRE::ORDEM::NENHUM);
        POTENCIA::definir(POTENCIA::SELECIONADO::DESLIGANDO);

}