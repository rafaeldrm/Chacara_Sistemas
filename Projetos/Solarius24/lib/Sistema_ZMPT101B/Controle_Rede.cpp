//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Rede.h"
#include "Leitura_Rede.h"
#include "Diagnostico_ZMPT101B.h"

#include "Estado_Globais.h"
#include "Estados_Modulos.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

    static uint32_t inicioContagem = 0;

    static constexpr uint32_t TEMPO_5S   = 5000UL;
    static constexpr uint32_t TEMPO_5MIN = 300000UL;
    static constexpr uint32_t TEMPO_15MIN = 900000UL;

//====================================================================================
//                   SETUP
//====================================================================================

void Controle_Rede_Inicializa()
{
   
}

//====================================================================================
//                   ATIVAÇÃO
//====================================================================================

void Controle_Rede_Ativa()
{
  

    inicioContagem = millis();

    ESTADO::REDE::definir(ESTADO::REDE::STATUS::VERIFICANDO_5S);
}

//====================================================================================
//                   STATUS
//====================================================================================

bool Controle_Rede_Pronto_Para_Ligar()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();

    return (ESTADO::REDE::atual() == ESTADO::REDE::STATUS::MONITORANDO_FONTE &&
            diag.estado == SENSOR::OK);
}

//====================================================================================
//                   VERIFICANDO 5 SEGUNDOS (BLOQUEANTE)
//====================================================================================

ESTADO::REDE::STATUS Controle_Rede_Logica_Verificando_5s()
{
    Leitura_Rede_Estabilizar(); 

    uint32_t tempoInicio = millis();

    while (millis() - tempoInicio < TEMPO_5S)
    {
        Leitura_Rede_Atualiza(); 
        
        const auto& status = Leitura_Rede_ObterStatus();
        const auto& diag   = Diagnostico_ZMPT101B_ObterStatus();

        // 1. SE O SENSOR FALHAR (ERRO OU INSTÁVEL)
        if (diag.estado != SENSOR::OK)
        {
            debugAviso("ZMPT101B FALHOU! Entrando em modo FORÇA BRUTA (v3.0 Style)");
            // Retornamos um estado que ignora o sensor, mas liga a fonte.
            // Funciona a Fonte Usina mesmo com o sensor INSTÁVEL ou com ERRO
            return ESTADO::REDE::STATUS::MONITORANDO_FONTE; 
        }

        // 2. SE O SENSOR ESTÁ OK, MAS A RUA CAIU (0V)
        if (!status.tem_rede)
        {
            debugAviso("Rede da concessionária AUSENTE (Lida pelo ZMPT).");
            return ESTADO::REDE::STATUS::AUSENTE;
        }
    }

    return ESTADO::REDE::STATUS::MONITORANDO_FONTE;
}
//====================================================================================
//                   REDE AUSENTE
//====================================================================================

ESTADO::REDE::STATUS Controle_Rede_Logica_Ausente()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();
    const auto& status = Leitura_Rede_ObterStatus();

    // Se o sensor está OK e REALMENTE não tem rede (0V)
    if (diag.estado == SENSOR::OK && !status.tem_rede)
    {
        return ESTADO::REDE::STATUS::AUSENTE; 
    }

    if (diag.estado != SENSOR::OK) {
        return ESTADO::REDE::STATUS::MONITORANDO_FONTE;
    }

    // Se o sensor deu ERRO, INSTÁVEL ou a rede VOLTOU:
    // Não importa o motivo, saímos do AUSENTE para tentar carregar.
    inicioContagem = millis();
    return ESTADO::REDE::STATUS::AGUARDANDO_5MIN;
}

//====================================================================================
//                   AGUARDANDO 5 MINUTOS (ESTABILIZAÇÃO)
//====================================================================================

ESTADO::REDE::STATUS Controle_Rede_Logica_Aguardando_5min()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();
    const auto& status = Leitura_Rede_ObterStatus();

    // Se o sensor der pau durante a espera, não espera mais: LIGA!
    if (diag.estado != SENSOR::OK) {
        return ESTADO::REDE::STATUS::MONITORANDO_FONTE;
    }

    // Se o sensor está OK, mas a rede caiu de novo, volta pro zero
    if (!status.tem_rede) {
        return ESTADO::REDE::STATUS::AUSENTE;
    }

    // Se a rede estiver firme pelo tempo programado...
    if (millis() - inicioContagem >= TEMPO_5MIN) {
        return ESTADO::REDE::STATUS::MONITORANDO_FONTE;
    }

    return ESTADO::REDE::STATUS::AGUARDANDO_5MIN;
}
//====================================================================================
//                   REDE INSTÁVEL (15 MINUTOS)
//====================================================================================

ESTADO::REDE::STATUS Controle_Rede_Logica_Instavel_15min()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();
    const auto& status = Leitura_Rede_ObterStatus();

    // Se o sensor der erro aqui, cancelamos a "punição" e vamos pra carga
    if (diag.estado != SENSOR::OK) {
        return ESTADO::REDE::STATUS::MONITORANDO_FONTE;
    }

    // Se a rede estiver OK e o sensor OK, esperamos o tempo longo
    if (status.tem_rede) {
        if (millis() - inicioContagem >= TEMPO_15MIN) {
            return ESTADO::REDE::STATUS::MONITORANDO_FONTE;
        }
    } 
    else {
        // Se a rede cair DE NOVO durante os 15min, reseta o cronômetro
        inicioContagem = millis(); 
    }

    return ESTADO::REDE::STATUS::INSTAVEL_15MIN;
}

//====================================================================================
//                   MONITORANDO FONTE USINA
//====================================================================================

ESTADO::REDE::STATUS Controle_Rede_Logica_Monitorando_Fonte()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();
    const auto& status = Leitura_Rede_ObterStatus();

    // 1. SÓ DESLIGA SE: O sensor estiver 100% OK e confirmar que a rede caiu (0V).
    if (diag.estado == SENSOR::OK && !status.tem_rede)
    {
        debugAviso("MONITORANDO >> Rede caiu confirmada. Desligando Usina.");
        return ESTADO::REDE::STATUS::AUSENTE;
    }

    // 2. SE O SENSOR FICAR INSTÁVEL (Oscilação de leitura detectada pelo Diagnóstico)
    // Aqui mantemos a sua ideia: se estiver instável, entra no "castigo" de 15min.
    if (diag.estado == SENSOR::INSTAVEL)
    {
        debugAviso("MONITORANDO >> Rede instável. Entrando em modo 15min.");
        return ESTADO::REDE::STATUS::INSTAVEL_15MIN;
    }

    // 3. SE O SENSOR DER ERRO OU ESTIVER OK COM REDE:
    // Se caiu no SENSOR::ERRO, ele pula os IFs acima e cai aqui. 
    // É o modo "Vai na Fé": liga a fonte e mantém o estado.

    return ESTADO::REDE::STATUS::MONITORANDO_FONTE;
}
//====================================================================================
//                   CARGA COMPLETA (FINALIZAÇÃO)
//====================================================================================

ESTADO::REDE::STATUS Controle_Rede_Logica_Carga_Finalizada()
{
    return ESTADO::REDE::STATUS::DORMINDO;
}

//====================================================================================
//                   MODO CALAMIDADE
//====================================================================================

ESTADO::REDE::STATUS Controle_Rede_Logica_Calamidade()
{
    return ESTADO::REDE::STATUS::CALAMIDADE;
}