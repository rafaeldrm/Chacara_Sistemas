//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Sistema_Eco_Hidro.h"
#include "DEBUG_MODE.h"

#include "Leitura_Eco_Hidro_Boias.h"
#include "Controle_Eco_Hidro_Cloro_Transferencia.h"
#include "Controle_Eco_Hidro_Bomba_Casa.h"
#include "Gerencia_Eco_Hidro_Cloro_Transferencia.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================



//====================================================================================
//                   SETUP
//====================================================================================

void SISTEMA_ECO_HIDRO_BOOT()
{
    Leitura_Eco_Hidro_Boias_Inicializa();

    Gerencia_Eco_Hidro_Cloro_Transferencia_Inicializa();

    Controle_Eco_Hidro_Cloro_Transferencia_Inicializa();

    Controle_Eco_Hidro_Bomba_Casa_Inicializa();

}

//====================================================================================
//                   LOOP
//====================================================================================

void SISTEMA_ECO_HIDRO_LOOP()
{

    Gerencia_Eco_Hidro_Cloro_Transferencia_Atualiza();

}

