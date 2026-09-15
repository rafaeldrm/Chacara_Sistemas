//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Sistema_Voltagem.h" 
#include "DEBUG_MODE.h" 

#include "Driver_Sensor_Voltagem.h"
#include "Leitura_Bateria.h"
#include "Gerencia_Voltagem.h"
#include "Controle_Voltagem.h"
#include "Tempo_Voltagem.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 

 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void VOLTAGEM_BOOT() 
{ 
    
//    Driver_Sensor_Voltagem_Inicializa();

    Leitura_Bateria_Inicializa();

//    Gerencia_Voltagem_Inicializa();

//    Controle_Voltagem_Inicializa();

    Tempo_Voltagem_Inicializa();
 
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
void VOLTAGEM_LOOP()
{
    if (Tempo_Voltagem_Processa()) //libera a cada tempo em tempo (1 minuto)
    {

            Leitura_Bateria_Iniciar_Media(); //ativa como verdadeiro a flag para a camada Leitura poder ter passe livre.

    }

    if (Leitura_Bateria_Liberado())
    {

            Leitura_Bateria_Processa_Media();

    }

    if (Leitura_Bateria_Finalizada())
    {

            Gerencia_Voltagem_Atualiza();

    }

}