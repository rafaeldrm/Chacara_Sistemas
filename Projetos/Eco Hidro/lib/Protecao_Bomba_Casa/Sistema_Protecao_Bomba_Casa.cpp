//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Protecao_Bomba_Casa.h" 
#include "DEBUG_MODE.h" 

#include "Controle_Protecao_Bomba_Casa.h"
#include "Movimento_Bomba_Casa/Leitura_Movimento_Bomba_Casa.h"

#include "Termico_Bomba_Casa/Driver_DHT11.h"
#include "Termico_Bomba_Casa/Leitura_Temperatura.h"
#include "Movimento_Bomba_Casa/Driver_Sensor_IR.h"
#include "Termico_Bomba_Casa/Controle_Termico_Bomba_Casa.h"
#include "Termico_Bomba_Casa/Gerencia_Termico_Bomba_Casa.h"
#include "Giro_Preventivo_Bomba_Casa/Controle_Giro_Preventivo_Bomba_Casa.h"
#include "Giro_Preventivo_Bomba_Casa/Gerencia_Giro_Preventivo_Bomba_Casa.h"

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

    Controle_Protecao_Bomba_Casa_Inicializa();

    Controle_Termico_Bomba_Casa_Inicializa();

    Gerencia_Termico_Bomba_Casa_Inicializa();

    Controle_Giro_Preventivo_Bomba_Casa_Inicializa();

    Gerencia_Giro_Preventivo_Bomba_Casa_Inicializa();
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
void PROTECAO_BOMBA_CASA_LOOP()
{

 //Controle_Protecao_Bomba_Casa_Anti_Travamento();

 //Controle_Protecao_Bomba_Casa_Temperatura();

 //Controle_Protecao_Bomba_Casa_Tempo_Limite_Anti_Transbordamento();

// Controle_Protecao_Bomba_Casa_Anti_Succao_Ar_Sistema();

    Gerencia_Termico_Bomba_Casa_Atualiza();

    Gerencia_Giro_Preventivo_Bomba_Casa_Atualiza();
}