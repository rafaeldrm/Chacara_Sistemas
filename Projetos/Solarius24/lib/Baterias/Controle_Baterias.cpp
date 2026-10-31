//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Baterias.h"
#include "DEBUG_MODE.h"

#include "Leitura_Baterias.h"
#include "Diagnostico_INA3221.h"

#include "Estados_Sistema_Solarius24.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

constexpr int32_t DELTA_ALERTA_mV   = 500;
constexpr int32_t DELTA_RECUPERA_mV = 200;

constexpr uint32_t TEMPO_CALAMIDADE  = 5UL * 60UL * 1000;
constexpr uint32_t TEMPO_RECUPERACAO = 10UL * 60UL * 1000;

static unsigned long inicioFalha = 0;
static unsigned long inicioRecuperacao = 0;

//====================================================================================
//                   STATUS SENSOR (VINDO DO DIAGNÓSTICO)
//====================================================================================

static inline bool Sensor_OK()
{
    return Diagnostico_INA3221_ObterStatus().estado == SENSOR::OK;
}

static inline bool Sensor_Instavel()
{
    return Diagnostico_INA3221_ObterStatus().estado == SENSOR::INSTAVEL;
}

static inline bool Sensor_Erro()
{
    return Diagnostico_INA3221_ObterStatus().estado == SENSOR::ERRO;
}

//====================================================================================
//                   DELTA INTELIGENTE (COM FALLBACK)
//====================================================================================

static int32_t Obter_Maior_Delta()
{
    if (Sensor_Erro())
    {
        debugErro("INA3221 ERRO - sem leitura confiável");
        return -1;
    }

    PackBaterias pack = Leitura_Baterias_Obter_Pack();

    //============================
    // MODO INSTÁVEL (FALLBACK)
    //============================
    if (Sensor_Instavel())
    {
        debugAviso("INA3221 INSTAVEL - usando histórico");

        int32_t delta1 = abs(pack.celula_anterior[1] - pack.celula_anterior[0]);
        int32_t delta2 = abs(pack.celula_anterior[3] - pack.celula_anterior[2]);

        return (delta1 > delta2) ? delta1 : delta2;
    }

    //============================
    // MODO NORMAL
    //============================
    DadosBancada b1 = Leitura_Baterias_Obter_Bancada1();
    DadosBancada b2 = Leitura_Baterias_Obter_Bancada2();

    return (b1.delta_mV > b2.delta_mV) ? b1.delta_mV : b2.delta_mV;
}

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Baterias_Inicializa()
{  
        debugSetupArquivo();
        debugSetupDivisao(TEXTO_AMARELO, "CONTROLE BATERIAS", TEXTO_MAGENTA, "-", 120);

        debugOk("CARREGADO COM SUCESSO, CONTROLE BATERIAS!!!");

}


//====================================================================================
//                   LOOP
//====================================================================================

void Controle_Baterias_Atualiza()
{
    debugLoopTexto("Controle Baterias INA3221 Leitura Loop", NENHUM);

    // Atualiza diagnóstico AQUI (ponto correto)
    Diagnostico_INA3221_Atualiza(Leitura_Baterias_Obter_Pack());
}

//====================================================================================
//                   NORMAL
//====================================================================================

void Controle_Baterias_Normal()
{
    int32_t deltaMax = Obter_Maior_Delta();

    //============================
    // ERRO SENSOR
    //============================
    if (deltaMax < 0)
    {
        debugErro("Falha sensor - indo para OBSERVAÇÃO");
        BATERIA::definir(BATERIA::SITUACAO::OBSERVA);
        return;
    }

    //============================
    // ALERTA
    //============================
    if (deltaMax >= DELTA_ALERTA_mV)
    {
        debugAviso("ALERTA: Desequilíbrio detectado");
        debugVariavel("Valor DELTA :: ", deltaMax, NEGRITO, VERDE, LARANJA);

        inicioFalha = millis();

        BATERIA::definir(BATERIA::SITUACAO::OBSERVA);
        return;
    }

    BATERIA::definir(BATERIA::SITUACAO::NORMAL);
}

//====================================================================================
//                   OBSERVAÇÃO
//====================================================================================

void Controle_Baterias_Observacao()
{
    int32_t deltaMax = Obter_Maior_Delta();

    if (deltaMax < 0)
    {
        debugErro("Sensor falhando durante observação");
        BATERIA::definir(BATERIA::SITUACAO::OBSERVA);
        return;
    }

    if (deltaMax <= DELTA_RECUPERA_mV)
    {
        debugLoopTexto("Sistema estabilizado", NENHUM);
        inicioFalha = 0;
        BATERIA::definir(BATERIA::SITUACAO::NORMAL);
        return;
    }

    if (millis() - inicioFalha >= TEMPO_CALAMIDADE)
    {
        inicioRecuperacao = 0;
        BATERIA::definir(BATERIA::SITUACAO::CALAMIDADE_INICIO);
        return;
    }

    BATERIA::definir(BATERIA::SITUACAO::OBSERVA);
}

//====================================================================================
//                   CALAMIDADE INÍCIO
//====================================================================================

void Controle_Baterias_Calamidade_Inicio()
{
    debugAviso("!!! MODO CALAMIDADE ATIVADO !!!");
   
    return BATERIA::definir(BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO);
}

//====================================================================================
//                   CALAMIDADE - DESLIGA DISJUNTOR
//====================================================================================

void Controle_Baterias_Calamidade_Disjuntor_Desligado()
{
    // Agora esta camada apenas sinaliza. 
    // O tempo de espera pode ser controlado por uma sub-máquina de estados aqui
    // ou pelo Sistema_Acoes.
   
    return BATERIA::definir(BATERIA::SITUACAO::CALAMIDADE_COMPLETO);
}

//====================================================================================
//                   CALAMIDADE COMPLETO
//====================================================================================

void Controle_Baterias_Logica_Calamidade_Completo()
{
    debugAviso("!!! SHUTDOWN COMPLETO !!!");


    return BATERIA::definir(BATERIA::SITUACAO::TENTATIVA_RECUPERACAO);
}

//====================================================================================
//                   RECUPERAÇÃO
//====================================================================================

void Controle_Baterias_Logica_Tentativa_Recuperar_Sistema()
{
    int32_t deltaMax = Obter_Maior_Delta();

    if (deltaMax < 0)
        return BATERIA::definir(BATERIA::SITUACAO::TENTATIVA_RECUPERACAO);

    if (deltaMax <= DELTA_RECUPERA_mV)
    {
        if (inicioRecuperacao == 0)
        {
            inicioRecuperacao = millis();
            debugLoopTexto("Aguardando estabilização...", NENHUM);
        }

        if (millis() - inicioRecuperacao >= TEMPO_RECUPERACAO)
        {
            
            debugLoopTexto("REATIVANDO SISTEMA", NENHUM);

            inicioFalha = 0;
            inicioRecuperacao = 0;

            return BATERIA::definir(BATERIA::SITUACAO::NORMAL);

        }
    }
    else
    {
        inicioRecuperacao = 0;
    }

    return BATERIA::definir(BATERIA::SITUACAO::TENTATIVA_RECUPERACAO);
}

//====================================================================================
//                   BOOT EM CALAMIDADE
//====================================================================================

void Controle_Baterias_Ativar_Modo_Calamidade()
{
    debugErro("BOOT EM CALAMIDADE");

    // No boot, você pode manter ações diretas para garantir segurança,
    // ou forçar os enums para que o Sistema_Acoes desligue tudo no primeiro ciclo.
}