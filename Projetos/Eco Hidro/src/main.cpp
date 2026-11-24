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
#include "Escalonador.h"

#include "Monitor_Latencia.h"
#include "Monitora_RAM.h"


//====================================================================================
//                   VARIAVEL EXTERNA (SCRIPT)
//====================================================================================

//Contador automático de UPLOADS para o Arduino
//o script incrementa +1 no valor toda vez que fizer o upload do projeto para o arduino, compilação normal não ocorre contagem
//é tipo um hodometro de quantos uploads já foram feitos para aquele arduino específico
//por script em python automático no plataformio (/scripts/contador_upload.py)
static uint16_t HODOMETRO_BOOTLOADER = 325;  //número é atualizado automaticamente a cada upload, esse valor representa quantos bootloader o arduino recebeu.
//static uint32_t loop_contagem = 0;
//====================================================================================
//                   SETUP
//====================================================================================

void setup()
 {

//    Memoria_Ram::pintar();          // PRIMEIRA coisa, antes de criar objetos/alocar

//    Monitor_Latencia::Iniciar();
    
    COMUNICACAO_BOOT();

//    Serial.print(F("Inicio pintura: "));
//    Serial.println((uint16_t)Memoria_Ram::minimoLivre());

    DEBUG_MODE();
    
    ATUADORES_BOOT();

//    LORA_BOOT();
   
    BOIAS_BOOT();
 
    VOLTAGEM_BOOT();
 
    CASA_TORRE_BOOT();

    PROTECAO_BOMBA_CASA_BOOT();
 
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
//    Monitora_RAM_Atualiza();
    
//    uint32_t contagem = loop_contagem ++;
//    Serial.print(F("LOOP : "));
//    Serial.println(contagem);
//   delay(250);
//    LORA_LOOP();

    BOIAS_LOOP();

    VOLTAGEM_LOOP();

    CASA_TORRE_LOOP();

    RESERVATORIO_LOOP();

    LUZ_ESTACIONAMENTO_LOOP();

    LUZ_ACESSO_RESERVATORIO_LOOP();

    PROTECAO_BOMBA_CASA_LOOP();

    UNIDADE_POTENCIA_LOOP();

    MEDIADOR_LOOP();

    ESCALONADOR_LOOP();

//    Monitor_Latencia::Finalizar();

}