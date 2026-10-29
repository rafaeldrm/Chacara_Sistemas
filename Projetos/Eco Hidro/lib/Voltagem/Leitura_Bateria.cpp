//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Leitura_Bateria.h" 
#include "DEBUG_MODE.h" 

#include "Tempo.h"
#include "Cronometro.h"

#include "Driver_Sensor_Voltagem.h"

 
//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================

static constexpr uint8_t NUMERO_LEITURAS = 5;

static uint16_t bufferLeituras[NUMERO_LEITURAS] = {0};
static uint8_t indice = 0;

static uint32_t somaMediaLeituras = 0;

static uint16_t leituraADC = 0;

static uint16_t voltagemConvertida = 0;

static uint16_t voltagemMedia = 0;

static bool leituraFinalizada = false;

static bool inicioLeituraMedia = false;

static CRONOMETRO::Temporizador tempoLeitura;

static constexpr uint16_t TEMPO_AMOSTRA = TEMPO::SEGUNDOS(1);

//====================================================================================  
//                   SETUP  
//====================================================================================  
 
 void Leitura_Bateria_Inicializa() 
{ 
        debugSetupDivisao(COR::AMARELO, "DRIVER SENSOR VOLTAGEM INICIALIZANDO", COR::MAGENTA, "=", 45);
        
        tempoLeitura.Cronometro_Iniciar();
} 
 
//====================================================================================  
//                   LOOP  
//====================================================================================  
 
void Leitura_Bateria_Atualiza()
{
    leituraADC = Driver_Sensor_Voltagem_Entrega();

    voltagemConvertida = (leituraADC * 250UL) / 1023UL;
}

//====================================================================================
//                   LIBERA A LEITURA PARA MONTAR A MÉDIA
//====================================================================================

void Leitura_Bateria_Iniciar_Media()
{
    inicioLeituraMedia = true;

    leituraFinalizada = false;

    indice = 0;

    somaMediaLeituras = 0;
}

//====================================================================================
//                   GATILHO PARA LIBERACAO DE MEDIA MÓVEL
//====================================================================================

bool Leitura_Bateria_Liberado()
{
    return inicioLeituraMedia;
}

//====================================================================================
//                   PROCESSANDO
//====================================================================================

void Leitura_Bateria_Processa_Media()
{

    if (tempoLeitura.Cronometro_Ciclo_Continuo(TEMPO_AMOSTRA))
    {
        Leitura_Bateria_Atualiza();

        somaMediaLeituras -= bufferLeituras[indice];

        bufferLeituras[indice] = voltagemConvertida;

        somaMediaLeituras += voltagemConvertida;

        indice++;

        if (indice >= NUMERO_LEITURAS)
        {
            indice = 0;

            leituraFinalizada = true; //libera acesso para a camada sistema voltagem para funcionar gerencia/controle

            inicioLeituraMedia = false; //bloqueia a liberação para a camada sistema voltagem.
        }

        voltagemMedia = somaMediaLeituras / NUMERO_LEITURAS;
    }
}

//====================================================================================
//                   ENTREGA DO RESULTADO
//====================================================================================

uint16_t Leitura_Bateria_Entrega()
{
    return voltagemMedia; 
}

//====================================================================================
//                   ENTREGA DA LEITURA COM MEDIA PRONTA
//====================================================================================


bool Leitura_Bateria_Finalizada()
{
    if (leituraFinalizada)
    {
        leituraFinalizada = false;

        return true;
    }
    
    return false;
}