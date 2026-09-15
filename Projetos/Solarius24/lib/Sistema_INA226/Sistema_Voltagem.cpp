//====================================================================================
// DEPENDÊNCIAS
//====================================================================================

#include "Sistema_Voltagem.h"

#include "Driver_INA226.h"
#include "Leitura_Voltagem.h"
#include "Diagnostico_INA226.h"
#include "Recuperacao_INA226.h"
#include "Emergencia_Voltagem.h"
#include "Tempo_Voltagem.h"
#include "Controle_Voltagem.h"
#include "Gerencia_Voltagem.h"

#include "DEBUG_MODE.h"

//====================================================================================
//                  PRIMEIRA ENTREGA DE MEDIA AO SISTEMA
//====================================================================================

static void Sistema_Voltagem_Start_Media()
{
    debugSetupTexto("FORÇANDO LEITURA VOLTAGEM, em SETUP...", NEGRITO | LARANJA | LINHA);

    for (uint8_t i = 0; i < 6; i++)
    {
        Leitura_Voltagem_Atualiza();
        delay(200);
    }

    debugOk("VOLTAGEM PRONTA");
}

//====================================================================================
// BOOT
//====================================================================================

void SISTEMA_VOLTAGEM_BOOT()
{
    debugSetupArquivo();
    debugSetupTexto("=== SISTEMA VOLTAGEM BOOT ===", NEGRITO | MAGENTA | LINHA);

    //============================================================
    // DRIVER
    //============================================================

    Driver_INA226_Inicializa();

    //============================================================
    // LEITURA
    //============================================================

    Leitura_Voltagem_Inicializa();

    //============================================================
    // DIAGNÓSTICO
    //============================================================

    Diagnostico_INA226_Inicializa();

    //============================================================
    // EMERGÊNCIA
    //============================================================

    Emergencia_Voltagem_Inicializa();

    //============================================================
    // TEMPO
    //============================================================

    Tempo_Voltagem_Inicializa();

    //============================================================
    // CONTROLE
    //============================================================

    Controle_Voltagem_Inicializa();

    //============================================================
    // GERÊNCIA
    //============================================================

    Gerencia_Voltagem_Setup();

    //============================================================
    // WARMUP FINAL
    //============================================================

    Sistema_Voltagem_Start_Media();
    

    debugSetupTexto("=== VOLTAGEM ONLINE ===", NEGRITO | VERDE | LINHA);
}

//====================================================================================
// LOOP
//====================================================================================

void SISTEMA_VOLTAGEM_LOOP()
{
    //============================================================
    // ATUALIZA LEITURA (CONTROLADO POR TEMPO)
    //============================================================

    if (Tempo_Voltagem_Deve_Atualizar(Controle_Voltagem_Obter_Tensao()))
    {
        Leitura_Voltagem_Atualiza();
    }

    //============================================================
    // DIAGNÓSTICO
    //============================================================

    Diagnostico_INA226_Atualiza(Leitura_Voltagem_ObterDados());

    //===========================================================
    //                   RECUPERAÇÃO
    //===========================================================
    
    Recuperacao_INA226_Executar(Diagnostico_INA226_ObterStatus());

    //============================================================
    // GERÊNCIA DE ESTADOS - (CONTROLE)
    //============================================================

    Gerencia_Voltagem_Loop();

    //============================================================
    // ATUALIZA CONTROLE (usa diagnóstico + fallback)
    //============================================================

  //  Controle_Voltagem_Atualizar_Tensao();

    //============================================================
    // EMERGÊNCIA
    //============================================================

    Emergencia_Voltagem_Atualiza(Diagnostico_INA226_ObterStatus());

    //===========================================================
    //                   LIMPAR LEITURA
    //===========================================================

    Leitura_Voltagem_Consumir();

    //===========================================================
    //                   SALVA E PROTEGE HISTÓRICO VALOR ANTERIOR
    //===========================================================
    
    if (Diagnostico_INA226_ObterStatus().estado == SENSOR::OK) {
    Leitura_Voltagem_Valor_Anterior();
    }  

}