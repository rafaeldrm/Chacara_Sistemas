//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Protecao_Bomba_Casa.h" 
#include "DEBUG_MODE.h" 

#include "Giro_Preventivo_Bomba_Casa/Controle_Giro_Preventivo_Bomba_Casa.h"
#include "Giro_Preventivo_Bomba_Casa/Gerencia_Giro_Preventivo_Bomba_Casa.h"
#include "Termico_Bomba_Casa/Driver_DHT11.h"
#include "Termico_Bomba_Casa/Leitura_Temperatura.h"
#include "Termico_Bomba_Casa/Controle_Termico_Bomba_Casa.h"
#include "Termico_Bomba_Casa/Gerencia_Termico_Bomba_Casa.h"
#include "Movimento_Bomba_Casa/Driver_Sensor_IR.h"
#include "Movimento_Bomba_Casa/Leitura_Movimento_Bomba_Casa.h"
#include "Movimento_Bomba_Casa/Controle_Tranco_Bomba_Casa.h"
#include "Movimento_Bomba_Casa/Gerencia_Tranco_Bomba_Casa.h"
#include "Anti_Transbordamento_Caixa_Torre/Controle_Anti_Transbordamento_Caixa_Torre.h"
#include "Anti_Succao_Ar_Caixa_Estoque/Controle_Anti_Succao_Ar_Caixa_Estoque.h"

//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  

//====================================================================================  
//                   SETUP  
//====================================================================================  
 
void PROTECAO_BOMBA_CASA_BOOT()
{
        debugSetupArquivo();
        debugSetupDivisao(COR::AMARELO, "SISTEMA PROTEÇÃO BOMBA CASA INICIALIZANDO", COR::MAGENTA, "=", 0);
        
    Driver_DHT11_Inicializa();

    Leitura_Temperatura_Inicializa();

    Driver_Sensor_IR_Inicializa();

    Leitura_Movimento_Bomba_Casa_Inicializa();

    
 #if ATIVAR_TRANCO_BOMBA_CASA  
    Controle_Tranco_Bomba_Casa_Inicializa();

    Gerencia_Tranco_Bomba_Casa_Inicializa();
#endif

 #if ATIVAR_TERMICO_BOMBA_CASA  
    Controle_Termico_Bomba_Casa_Inicializa();

    Gerencia_Termico_Bomba_Casa_Inicializa();
#endif

 #if ATIVAR_GIRO_PREVENTIVO_BOMBA_CASA   
    Controle_Giro_Preventivo_Bomba_Casa_Inicializa();

    Gerencia_Giro_Preventivo_Bomba_Casa_Inicializa();
#endif

    Controle_Anti_Transbordamento_Caixa_Torre_Inicializa();

    Controle_Anti_Succao_Ar_Caixa_Estoque_Inicializa();
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
void PROTECAO_BOMBA_CASA_LOOP()
{
#if ATIVAR_TERMICO_BOMBA_CASA  
    Gerencia_Termico_Bomba_Casa_Atualiza();
#endif

#if ATIVAR_GIRO_PREVENTIVO_BOMBA_CASA 
    Gerencia_Giro_Preventivo_Bomba_Casa_Atualiza();
#endif

#if ATIVAR_TRANCO_BOMBA_CASA
    Gerencia_Tranco_Bomba_Casa_Atualiza();
#endif


    Controle_Anti_Transbordamento_Caixa_Torre_Atualiza();

    Controle_Anti_Succao_Ar_Caixa_Estoque_Atualiza();
}