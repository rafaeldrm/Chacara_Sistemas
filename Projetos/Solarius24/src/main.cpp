//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include <Arduino.h> //biblioteca Arduino para C++
#include "DEBUG_MODE.h"

#include "Sistema_Baterias.h"
#include "Sistema_Climatizacao.h"
#include "Sistema_Rede.h"
#include "Sistema_Voltagem.h"
#include "Mediador_Sistema.h"
#include "Sistema_Carga_Urgencia.h"
#include "Sistema_Seguranca.h"
#include "Protocolo_Comunicacao_Arduino.h"
#include "Modulo_Reles.h"


//====================================================================================
//                   SETUP
//====================================================================================

void setup()
 {

    COMUNICACAO_SETUP(); //0_Arduino\Protocolo_Comunicacao_Arduino.h
    DEBUG_MODE(); //Utilidades\DEBUG_MODE.h
    Reles_Setup(); //2_HAL\Modulo_Reles.h
    EEPROM_Inicializa(); //Utilidades\Memoria_Watchdog_EEPROM.h
    // 4. LoRa
    // 5. LoRa BOOT Aviso
    BATERIAS_BOOT();
    CARGA_URGENCIA_BOOT();
    REDE_BOOT(); //Sistema\Sistema_Rede.h
    VOLTAGEM_BOOT(); //Sistema\Sistema_Voltagem.h
    CLIMATIZACAO_BOOT(); //Sistema\Sistema_Climatizacao.h
   
}

//====================================================================================
//                   LOOP
//====================================================================================

void loop() 
 {
    //========= RAM e WATCHDOG ===================//

    Monitorar_RAM_Atualiza(); //Sistema\Sistema_Seguranca.h
    Watchdog_Sistema_Atualiza(); //Sistema\Sistema_Seguranca.h

    //========= SISTEMA ENERGIA ==================//

    BATERIAS_LOOP(); //Sistema\Sistema_Baterias.h
    CARGA_URGENCIA_LOOP();
    VOLTAGEM_LOOP(); //  Sistema\Sistema_Voltagem.h  
    REDE_LOOP();     //  Sistema\Sistema_Rede.h

    //=========== COORDENADOR ======================//
    
    Mediador_Sistema_Loop(); //Nucleo\Mediador_Sistema.h

    CLIMATIZACAO_LOOP(); //Sistema\Sistema_Climatizacao.h
    //SISTEMA_TELEMETRIA_MAQUINA_LAVAR_ROUPAS_LOOP();
     //SISTEMA_OLED_LOOP();
    //SISTEMA_LORA_LOOP();

}





/* 
versão 3.0 (funcionando atualmente)
tipos de Arquivos : .ino
quantidade de arquivos : 5
linhas de código : 176
linhas de comentários : 204
espaços em branco : 82

RAM:   [=         ]  32.8% (used 2621 bytes from 8192 bytes)
Flash: [=         ]   5.1% (used 12850 bytes from 253952 bytes)


Versão 4.0 (em construção...)
tipos de arquivos: .h e .cpp
quantidade de arquivos : 129	
linhas de código : 4865
linhas de comentários : 1856
Espaços em brancos : 2418

Serial.print ativados
RAM:   [=         ]  10.9% (used 891 bytes from 8192 bytes)
Flash: [==        ]  17.2% (used 43572 bytes from 253952 bytes)

Serial.print desativados

RAM:   [=         ]   9.9% (used 811 bytes from 8192 bytes)
Flash: [=         ]   5.7% (used 14534 bytes from 253952 bytes)

*/