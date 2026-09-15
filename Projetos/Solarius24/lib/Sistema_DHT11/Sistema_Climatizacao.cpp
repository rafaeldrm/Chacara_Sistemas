//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Sistema_Climatizacao.h"
#include "DEBUG_MODE.h"

#include "Driver_DHT11.h"
#include "Leitura_Temperatura.h"
#include "Diagnostico_DHT11.h"
#include "Controle_Climatizacao_MPPT.h"
#include "Emergencia_Climatizacao_MPPT.h"
#include "Tempo_Climatizacao.h"

#include "Modulo_Reles.h"

//====================================================================================
//                   SETUP
//====================================================================================

void SISTEMA_CLIMATIZACAO_BOOT() {

    debugSetupArquivo();
    debugSetupTexto("=== SISTEMA CLIMATIZAÇÃO BOOT ===", NEGRITO | MAGENTA | LINHA);
  
    Driver_DHT11_Inicializa();    //Driver --- 1_Drivers/Driver_DHT11.h
    
    Leitura_Temperatura_Inicializa();  //Leitura --- 3_Leituras/Leitura_Temperatura.h

    Diagnostico_DHT11_Inicializa();  //Diagnóstico --- 4_Diagnostico/Diagnostico_DHT11.h

    Controle_Climatizacao_MPPT_Inicializa();   //Controle --- 6_Controles/Controle_Climatizacao_MPPT.h
   
    Emergencia_Climatizacao_MPPT_Inicializa();   //Emergência --- 5_Emergencia/Emergencia_Climatizacao_MPPT.h

    Tempo_Climatizacao_Inicializa();   //Tempo --- 9_Tempo/Temporizador_Climatizacao.h

    //RELE --- 2_HAL/Modulo_Reles.h
    COOLER_MPPT.Ligar();

     debugSetupTexto("=== CLIMATIZAÇÃO ONLINE ===", NEGRITO | VERDE | LINHA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void SISTEMA_CLIMATIZACAO_LOOP()
{
    //===============================================================================
    // TEMPO (sempre roda)
    //===============================================================================

    Tempo_Climatizacao_Atualiza();
    const auto& tempo = Tempo_Climatizacao_ObterEstado();

    //===============================================================================
    // LEITURA (só quando liberado)
    //===============================================================================

    if (tempo.liberado)
    {
        Leitura_Temperatura_Atualiza();
    }

    const auto& leitura = Leitura_Temperatura_ObterDados();

    //===============================================================================
    // DIAGNÓSTICO (sempre roda)
    //===============================================================================

    Diagnostico_DHT11_Atualiza(leitura);
    const auto& status = Diagnostico_DHT11_ObterStatus();

    //===============================================================================
    // CONTROLE (somente quando leitura ativa)
    //===============================================================================

    if (tempo.liberado)
    {
        Controle_Climatizacao_MPPT_Atualiza(leitura, status);
    }

    const auto& controle = Controle_Climatizacao_MPPT_ObterEstado();

    //===============================================================================
    // EMERGÊNCIA (sempre roda)
    //===============================================================================

    Emergencia_Climatizacao_MPPT_Atualiza(status);
    const auto& emergencia = Emergencia_Climatizacao_ObterEstado();

    //===============================================================================
    // DECISÃO FINAL (prioridade: emergência)
    //===============================================================================

    bool ligar_cooler;

    if (emergencia.ativo)
    {
        ligar_cooler = emergencia.cooler_ligado;
    }
    else
    {
        ligar_cooler = controle.cooler_ligado;
    }

    //===============================================================================
    // ATUAÇÃO
    //===============================================================================

    if (ligar_cooler)
        COOLER_MPPT.Ligar();
    else
        COOLER_MPPT.Desligar();
}