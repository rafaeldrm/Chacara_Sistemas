//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Leitura_Movimento_Bomba.h"
#include "DEBUG_MODE.h"

#include "Driver_Sensor_IR.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

// Configurações (Usando const para segurança de tipo)
constexpr int8_t TOTAL_LEITURAS    = 20;
constexpr int8_t INTERVALO_LEITURA = 40;   // ms (Total ~800ms de teste)
constexpr int8_t MIN_TRANSICOES    = 4;    // Mínimo de variações para considerar girando

//====================================================================================
//                   SETUP
//====================================================================================

void Leitura_Movimento_Bomba_Inicializa()
{
    // Não tem setup específico por enquanto, mas pode ser usado para inicializar o sensor IR se necessário
}

//====================================================================================
//                   MONITORA BOMBA ÁGUA CASA NA PARTIDA
//====================================================================================

bool Sensor_IR_Bomba_Verificar_Partida() {
   
        Driver_Liga_Sensor_IR();
        delay(50);
        Driver_Lendo_Sensor_IR(); // descarta primeira leitura

    uint8_t ultimoEstado = Driver_Lendo_Sensor_IR();
    int transicoes = 0;

    for (int i = 0; i < TOTAL_LEITURAS; i++) {
        delay(INTERVALO_LEITURA); // Pequena pausa para amostragem

        uint8_t atual = Driver_Lendo_Sensor_IR();

        // Se o estado mudou (era 0 virou 1, ou vice-versa)
        if (atual != ultimoEstado) {
            transicoes++;
            ultimoEstado = atual;
        }

        // Se já detectou movimento suficiente, sai mais cedo (otimização)
        if (transicoes >= MIN_TRANSICOES) {
            debugOk("IR Bomba: Movimento detectado! (OK)");
            Driver_Desliga_Sensor_IR();
            return true;
        }
    }
    
    debugErro("IR Bomba: FALHA! Helice parada.");
    Driver_Desliga_Sensor_IR();
    return false;
}