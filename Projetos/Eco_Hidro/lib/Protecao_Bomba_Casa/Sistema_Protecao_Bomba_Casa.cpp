//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Protecao_Bomba_Casa.h" 
#include "DEBUG_MODE.h" 

#include "Controle_Protecao_Bomba_Casa.h"
#include "Leitura_Movimento_Bomba.h"
#include "Estados_Sistema_Eco_Hidro.h"

#include "Driver_DHT11.h"
#include "Leitura_Temperatura.h"
#include "Driver_Sensor_IR.h"

#include "Driver_Boias.h"
#include "Leitura_Boias.h"

//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  



//====================================================================================  
//                   SETUP  
//====================================================================================  
 
void PROTECAO_BOMBA_CASA_BOOT()
{

//    Driver_DHT11_Inicializa();

    Leitura_Temperatura_Inicializa();

    Driver_Sensor_IR_Inicializa();

//    Leitura_Movimento_Bomba_Inicializa();

//    Controle_Protecao_Bomba_Casa_Inicializa();

} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
void PROTECAO_BOMBA_CASA_LOOP()
{

    if (!Controle_Protecao_Bomba_Casa_Atualiza())
        return;

 Controle_Protecao_Bomba_Casa_Anti_Travamento();

 Controle_Protecao_Bomba_Casa_Temperatura();

 Controle_Protecao_Bomba_Casa_Tempo_Limite_Anti_Transbordamento();

 Controle_Protecao_Bomba_Casa_Anti_Succao_Ar_Sistema();

 Controle_Protecao_Bomba_Casa_Redundancia_Torre_Desligamento();

}