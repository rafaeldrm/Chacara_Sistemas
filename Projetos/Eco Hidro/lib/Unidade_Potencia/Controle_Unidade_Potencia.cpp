//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Unidade_Potencia.h"
#include "DEBUG_MODE.h"

#include "Tempo.h"
#include "Cronometro.h"

#include "Estados_Sistema_Eco_Hidro.h"
#include "Modulo_Reles.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

static constexpr uint32_t TEMPO_CAPACITOR_INVERSOR = TEMPO::SEGUNDOS(10); //10UL * 1000;   // 10s //@@ Tempo.h

static CRONOMETRO::Temporizador aguardandoCapacitores; //@@ Cronometro.h

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Unidade_Potencia_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "CONTROLE UNIDADE POTENCIA INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   LIGANDO INVERSOR
//====================================================================================

void Controle_Unidade_Potencia_Acionamento_Reles()
{

       RELE_INVERSOR.Ligar();
       delay(200);
       RELE_ARMA_RELES_12V.Ligar();
       aguardandoCapacitores.Cronometro_Iniciar(); //@@ Cronometro.cpp
       UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::AGUARDANDO_ESTABILIZAR);

}

//====================================================================================
//                   AGUARDANDO ESTABILIZAR
//====================================================================================


void Controle_Unidade_Potencia_Aguardando_Estabilizar()
{

      if (aguardandoCapacitores.Cronometro_Dispara_Uma_Vez(TEMPO_CAPACITOR_INVERSOR)) //@@ Cronometro.cpp
      
       {

            UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::LIGADO);
        
        }

}

//====================================================================================
//                   LIGADO
//====================================================================================

//Função não existe, pois o estágio de LIGADO não requer ações específicas.
// O sistema permanece nesse estado até que uma nova ação seja necessária.
//quem o retira daqui é o Mediador.

//====================================================================================
//                   DESLIGANDO
//====================================================================================

void Controle_Unidade_Potencia_Desligando()
{
        RELE_ARMA_RELES_12V.Desligar();
        delay(200);
        RELE_INVERSOR.Desligar();
        UNIDADE_POTENCIA::definir(UNIDADE_POTENCIA::ESTAGIOS::REPOUSO);

}