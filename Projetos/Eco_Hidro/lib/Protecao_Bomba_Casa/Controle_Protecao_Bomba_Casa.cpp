//====================================================================================  
//                   DEPENDÊNCIAS  
//====================================================================================  
 
#include "Controle_Protecao_Bomba_Casa.h" 
#include "DEBUG_MODE.h" 

#include "Tempo.h"
#include "Cronometro.h"

#include "Leitura_Movimento_Bomba.h"
#include "Estados_Sistema_Eco_Hidro.h"

#include "Driver_DHT11.h"
#include "Leitura_Temperatura.h"
#include "Driver_Sensor_IR.h"

#include "Driver_Boias.h"
#include "Leitura_Boias.h"

//====================================================================================  
//                   VARIÁVEIS INTERNAS  
//====================================================================================  
 
static constexpr int16_t TEMPERATURA_LIMITE = 650; // 65.0°C
static constexpr uint32_t TEMPO_MAX_BOMBA = TEMPO::MINUTOS(27); //27UL * 60UL * 1000UL; // 27 min
 
static CRONOMETRO::Temporizador antitransbordamento;
static uint8_t falhas = 0;

static bool estoqueMinimaVazia = false;

//====================================================================================  
//                   SETUP  
//====================================================================================  
 
void Controle_Protecao_Bomba_Casa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "CONTROLE PROTEÇÃO BOMBA INICIALIZANDO", COR::MAGENTA, "=", 45);
}
 
//====================================================================================  
//                   LOOP  
//====================================================================================  

bool Controle_Protecao_Bomba_Casa_Atualiza()
{

    const auto estado = BOMBA_CASA::atual();
// Verifica se a bomba está em um estado que precisa monitorar (acionando ou acionado). Caso contrário retorna imediatamente ao loop principal, sem fazer nada.
    return
        estado == BOMBA_CASA::ESTAGIOS::LIGANDO_BOMBA ||
        estado == BOMBA_CASA::ESTAGIOS::BOMBA_LIGADA;
}

//====================================================================================
//                   SENSOR IR (VERIFICA BOMBA MOVIMENTAÇÃO PARTIDA)
//====================================================================================

void Controle_Protecao_Bomba_Casa_Anti_Travamento()
{

        const auto estado = BOMBA_CASA::atual();

    // 1. Verifica se bomba girou (somente na partida)
    if (estado == BOMBA_CASA::ESTAGIOS::LIGANDO_BOMBA) {
        if (!Sensor_IR_Bomba_Verificar_Partida()) {
            DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::SENSOR_IR_BOMBA_TRAVADO);
            BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
            return;
        }

         // INICIA CONTAGEM AQUI (contrador "regressivo" para desligamento da bomba)
            antitransbordamento.Cronometro_Iniciar();
    }

}

//====================================================================================
//                   DHT11 MONITORANDO TEMPERATURA BOMBA DURANTE FUNCIONAMENTO
//====================================================================================

void Controle_Protecao_Bomba_Casa_Temperatura()
{

    // 2. Verifica temperatura
    Leitura_Temperatura_Atualiza();

  // auto& dados = Leitura_Temperatura_ObterDados();

   int16_t temperatura = Leitura_Temperatura_Obter_Temperatura();

if (temperatura < 0) {
    falhas++;

    if (falhas >= 20) { // ~20s sem leitura válida
        DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::DHT11_ERRO);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
        return;
    }

} else {
    falhas = 0;

    if (temperatura >= TEMPERATURA_LIMITE) {
        DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::DHT11_TEMPERATURA);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
        return;
        }
    }
}
//====================================================================================
//                   TEMPO LIMITE SEGURANÇA ANTI-TRANSBORDAMENTO
//====================================================================================

void Controle_Protecao_Bomba_Casa_Tempo_Limite_Anti_Transbordamento()
{
    // 3. Contagem regressiva de tempo máximo de funcionamento
       // TEMPO MÁXIMO (caso o sensor boia máxima Torre e ou Estoque dê falha, protege a caixa ESTOQUE não entrar ar na válvula de retenção.)
    if (antitransbordamento.Cronometro_Dispara_Uma_Vez(TEMPO_MAX_BOMBA)) {
        DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::TEMPO_MAXIMO);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
        return;
    }
}
//====================================================================================
//                   CAIXA ESTOQUE- PROTEÇÃO CONTRA ENTRADA DE AR
//====================================================================================

void Controle_Protecao_Bomba_Casa_Anti_Succao_Ar_Sistema()
{
    estoqueMinimaVazia = !Leitura_Boias_Esta_Ativa(REGISTRO_BOIAS::ESTOQUE_MINIMA);

     // ESTOQUE ficando vazio é ruim pra válvula de retenção da bomba ficar puxando ar.
    if (estoqueMinimaVazia) {
        DESLIGAMENTO_BOMBA::definir(DESLIGAMENTO_BOMBA::MOTIVO::ESTOQUE_MINIMO);
        BOMBA_CASA::definir(BOMBA_CASA::ESTAGIOS::DESLIGANDO_BOMBA);
        return;
    }
}