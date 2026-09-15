//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Tempo_Boias.h" 
#include "DEBUG_MODE.h" 
 
#include "Tempo.h" 
#include "Cronometro.h"
 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  

static constexpr uint32_t TEMPO_CONFIRMACAO = TEMPO::SEGUNDOS(20);

static CRONOMETRO::Temporizador boiasLeitura;
 
//====================================================================================  
//                   SETUP  
//====================================================================================  
 
void Tempo_Boias_Inicializa() 
{ 
    debugSetupTexto ("Inicializando Tempo de Verificação das Boias", NEGRITO | SUBLINHADO | AZUL | LINHA);
    boiasLeitura.Cronometro_Iniciar();
} 

//====================================================================================  
//                   PROCESSA TEMPO DE VERIFICAÇÃO DAS BÓIAS
//====================================================================================  
 
bool Tempo_Boias_Processa()
{
    return boiasLeitura.Cronometro_Ciclo_Continuo(TEMPO_CONFIRMACAO);
}