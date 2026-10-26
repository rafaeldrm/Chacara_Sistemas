//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include <Arduino.h>
#include "DEBUG_MODE.h"

#include "Protocolo_Comunicacao_Arduino.h"

#include "Sistema_Atuadores.h"
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

//====================================================================================
//                   VARIAVEL EXTERNA (SCRIPT)
//====================================================================================

//Contador automático de UPLOADS para o Arduino
//o script incrementa +1 no valor toda vez que fizer o upload do projeto para o arduino, compilação normal não ocorre contagem
//é tipo um hodometro de quantos uploads já foram feitos para aquele arduino específico
//por script em python automático no plataformio (/scripts/contador_upload.py)
static uint16_t HODOMETRO_BOOTLOADER = 94;  //número é atualizado automaticamente a cada upload, esse valor representa quantos bootloader o arduino recebeu.

//====================================================================================
//                   SETUP
//====================================================================================

void setup()
 {

    DEBUG_MODE();

    COMUNICACAO_BOOT();
    
    ATUADORES_BOOT();

    LORA_BOOT();
   
    BOIAS_BOOT();
 
    VOLTAGEM_BOOT();
 
    CASA_TORRE_BOOT();

// PROTECAO_BOMBA_CASA_BOOT();
 
    RESERVATORIO_BOOT();
 
   LUZ_ESTACIONAMENTO_BOOT();

    LUZ_ACESSO_RESERVATORIO_BOOT();

  //  PORTAO_BOOT();

    MEDIADOR_BOOT();

    UNIDADE_POTENCIA_BOOT();
 
 //e mais outras coisas que forem necessárias para o setup do sistema
   debugVariavel("Hodômetro de Bootloader : ", HODOMETRO_BOOTLOADER, NEGRITO, VERDE, AMARELO | LINHA);
   debugAviso("Finalizado o BOOT");

}

//====================================================================================
//                   LOOP
//====================================================================================

void loop() 
{

  LORA_LOOP();

   BOIAS_LOOP();

   VOLTAGEM_LOOP();

   CASA_TORRE_LOOP();

   RESERVATORIO_LOOP();

//   PROTECAO_BOMBA_CASA_LOOP();

  LUZ_ESTACIONAMENTO_LOOP();

   LUZ_ACESSO_RESERVATORIO_LOOP();

  //PORTAO_LOOP();

    MEDIADOR_LOOP();

    UNIDADE_POTENCIA_LOOP();

  } 



   // TESTANDO uma espécie de contador de tempo por loop que o arduino leva de processamento, 
   //assim posso medir se em algum momento ele chegue a demorar mais que o padrão normal
   //quando finalizar implemento ele de forma certinha em um arquivo compartilhável com função.
    //e mais outras coisas que forem necessárias para o loop do sistema
/*
    static uint32_t maior_tempo = 0;

uint32_t inicio = micros();

uint32_t tempo = micros() - inicio;

if (tempo > maior_tempo)
{
    maior_tempo = tempo;
}

Serial.println(maior_tempo);
*/
