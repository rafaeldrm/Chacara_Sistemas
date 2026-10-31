#include "Servico_Telemetria_Maquina_Lava_Roupas.h"
#include "Leitura_Corrente_Maquina.h"
#include "DEBUG_MODE.h"
#include "Registrador_Status.h"


//====================================================================================
//                   CONFIGURAÇÃO
//====================================================================================

static unsigned long millisUltimaCorrenteAlta = 0;
constexpr uint32_t ATRASO_DESLIGAMENTO = 60000UL; // 60 segundos de tolerância

// Limiar de detecção (ajustável depois com testes reais)
constexpr uint16_t LIMIAR_LIGA = 300;
constexpr uint16_t LIMIAR_DESLIGA = 150;

//====================================================================================
//                   SETUP
//====================================================================================

void Servico_Telemetria_Maquina_Lava_Roupas_Setup()
{
    Flags::TornarFalsa(Flags::Posicao::MAQUINA_LIGADA);


}


//====================================================================================
//                   LOOP
//====================================================================================

void Servico_Telemetria_Maquina_Lava_Roupas_Loop(const DadosCorrenteMaquina& corrente)
{
    uint16_t corrente_Telemetria = corrente.corrente;
    
    // 1. Capturamos o estado que ESTÁ salvo no bit antes de qualquer alteração
    bool estavaLigada = Flags::EhVerdadeira(Flags::MAQUINA_LIGADA);

    // 2. Lógica de Ativação (Histerese)
    if (corrente_Telemetria > LIMIAR_LIGA) {
        millisUltimaCorrenteAlta = millis();
        if (!estavaLigada) {
            Flags::TornarVerdadeira(Flags::MAQUINA_LIGADA);
        }
    }

    // 3. Lógica de Desligamento com Tolerância (60s)
    // Só entra aqui se o bit estiver true e a corrente baixa
    if (estavaLigada && corrente_Telemetria < LIMIAR_DESLIGA) {
        if (millis() - millisUltimaCorrenteAlta > ATRASO_DESLIGAMENTO) {
            Flags::TornarFalsa(Flags::MAQUINA_LIGADA);
        }
    }

    // 4. Detecção de Mudança para Telemetria
    // Comparamos o estado novo (no bit) com o estado que capturamos no início (estavaLigada)
    if (Flags::EhVerdadeira(Flags::MAQUINA_LIGADA) != estavaLigada) 
    {
        if (Flags::EhVerdadeira(Flags::MAQUINA_LIGADA)) {
            debugOk("Lavadora: Ciclo iniciado!");
            // RADIO_LORA.Enviar("LAVADORA_ON");
        } else {
            debugAviso("Lavadora: Ciclo finalizado!");
            // RADIO_LORA.Enviar("LAVADORA_OFF");
        }
    }
}