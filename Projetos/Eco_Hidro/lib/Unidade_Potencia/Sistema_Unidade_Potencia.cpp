//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Sistema_Unidade_Potencia.h"
#include "DEBUG_MODE.h"

#include "Gerencia_Unidade_Potencia.h"
#include "Controle_Unidade_Potencia.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void UNIDADE_POTENCIA_BOOT()
{
		debugSetupArquivo();
		debugSetupDivisao(COR::AMARELO, "SISTEMA UNIDADE POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 45);
	
	Gerencia_Unidade_Potencia_Inicializa();

	Controle_Unidade_Potencia_Inicializa();
}

//====================================================================================
//                   LOOP
//====================================================================================

void UNIDADE_POTENCIA_LOOP()
{
    Gerencia_Unidade_Potencia_Atualiza();
}