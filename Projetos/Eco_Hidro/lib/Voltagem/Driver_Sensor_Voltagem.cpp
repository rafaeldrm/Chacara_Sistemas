//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Driver_Sensor_Voltagem.h" 
#include "DEBUG_MODE.h" 

#include "Mapa_Pinos_Arduino.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  

static constexpr uint8_t PINO_SENSOR_VOLTAGEM = pino(ARDUINO_CONEXAO::SENSOR::VOLTAGEM_BATERIA_12V); //pino analógico A5 para leitura da voltagem da bateria de 12V

static uint16_t leituraBruta = 0;

//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Driver_Sensor_Voltagem_Inicializa() 
{ 
    //Configurações iniciais para o sensor de voltagem, se necessário. No caso de um sensor analógico, geralmente não é necessário configurar nada específico.
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  

void Driver_Sensor_Voltagem_Atualiza()
{
    leituraBruta = analogRead(PINO_SENSOR_VOLTAGEM);
}

//====================================================================================
//                   ENTREGA DO VALOR PARA OUTRA CAMADA
//====================================================================================

uint16_t Driver_Sensor_Voltagem_Entrega()
{
    return leituraBruta;
}