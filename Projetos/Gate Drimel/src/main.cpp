#include <Arduino.h>

#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIAVEL EXTERNA (SCRIPT)
//====================================================================================

//Contador automático de UPLOADS para o Arduino
//o script incrementa +1 no valor toda vez que fizer o upload do projeto para o arduino, compilação normal não ocorre contagem
//é tipo um hodometro de quantos uploads já foram feitos para aquele arduino específico
//por script em python automático no plataformio (/scripts/contador_upload.py)
static uint16_t HODOMETRO_BOOTLOADER = 86;  //número é atualizado automaticamente a cada upload, esse valor representa quantos bootloader o arduino recebeu.

//====================================================================================
//                   SETUP
//====================================================================================

void setup() 
{

       //e mais outras coisas que forem necessárias para o setup do sistema
       debugVariavel("Hodômetro de Bootloader : ", HODOMETRO_BOOTLOADER, NEGRITO, VERDE, AMARELO | LINHA);
       debugAviso("Finalizado o BOOT");
}

//====================================================================================
//                   LOOP
//====================================================================================

void loop() 
{


}