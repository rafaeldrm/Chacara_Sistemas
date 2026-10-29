//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Sistema_Baterias.h"
#include "DEBUG_MODE.h"

#include "Driver_INA3221.h"
#include "Leitura_Baterias.h"
#include "Diagnostico_INA3221.h"
#include "Emergencia_Baterias.h"
#include "Gerencia_Baterias.h"
#include "Controle_Baterias.h"
#include "Tempo_Baterias.h"

//====================================================================================
//                   SETUP
//====================================================================================

void SISTEMA_BATERIAS_BOOT()
{
    debugSetupArquivo();
    debugSetupTexto("=== SISTEMA BATERIAS BOOT ===", NEGRITO | MAGENTA | LINHA);
    
    SENSOR_INA3221_B1.Inicializa();

    SENSOR_INA3221_B2.Inicializa();

    Leitura_Baterias_Inicializa();

    Diagnostico_INA3221_Inicializa();

    Gerencia_Baterias_Inicializa();

    Controle_Baterias_Inicializa();

    Emergencia_Baterias_Inicializa();

    Tempo_Baterias_Inicializa();

    debugSetupTexto("=== BATERIAS ONLINE ===", NEGRITO | VERDE | LINHA);

}

//====================================================================================
//                   LOOP
//====================================================================================

void SISTEMA_BATERIAS_LOOP()
{
    // 1. Atualiza tempo
    Tempo_Baterias_Atualiza();

    // 2. Se NÃO liberou → sistema dorme
    if (!Tempo_Baterias_PermiteLeitura())
        return;

    // 3. LEITURA
    Leitura_Baterias_Loop();

    if (!Leitura_Baterias_TemNovoDado())
        return;

    // 4. DIAGNÓSTICO
    Diagnostico_INA3221_Atualiza(Leitura_Baterias_Obter_Pack());

    // 5. EMERGÊNCIA
    Emergencia_Baterias_Atualiza(Diagnostico_INA3221_ObterStatus());

    // 6. CONTROLE (máquina de estado)
    Gerencia_Baterias_Atualiza(); // Correção: Chamar a função de atualização, não a de inicialização

    // 7. Consome dado
    Leitura_Baterias_Consumir();
}