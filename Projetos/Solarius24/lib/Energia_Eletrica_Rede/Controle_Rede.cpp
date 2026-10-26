//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Controle_Rede.h"
#include "Leitura_Rede.h"
#include "Diagnostico_ZMPT101B.h"

#include "Estados_Sistema_Solarius24.h"
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

    REDE::definir(REDE::STATUS::VERIFICANDO_5S);
}

//====================================================================================
//                   STATUS
//====================================================================================

bool Controle_Rede_Pronto_Para_Ligar()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();

    return (REDE::atual() == REDE::STATUS::MONITORANDO_FONTE &&
            diag.estado == SENSOR::OK);
}

//====================================================================================
//                   VERIFICANDO 5 SEGUNDOS (BLOQUEANTE)
//====================================================================================

void Controle_Rede_Verificando_5segundos()
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
            return REDE::definir(REDE::STATUS::MONITORANDO_FONTE); 
        }

        // 2. SE O SENSOR ESTÁ OK, MAS A RUA CAIU (0V)
        if (!status.tem_rede)
        {
            debugAviso("Rede da concessionária AUSENTE (Lida pelo ZMPT).");
            return REDE::definir(REDE::STATUS::AUSENTE);
        }
    }

    return REDE::definir(REDE::STATUS::MONITORANDO_FONTE);
}
//====================================================================================
//                   REDE AUSENTE
//====================================================================================

void Controle_Rede_Ausente()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();
    const auto& status = Leitura_Rede_ObterStatus();

    // Se o sensor está OK e REALMENTE não tem rede (0V)
    if (diag.estado == SENSOR::OK && !status.tem_rede)
    {
        return REDE::definir(REDE::STATUS::AUSENTE); 
    }

    if (diag.estado != SENSOR::OK) {
        return REDE::definir(REDE::STATUS::MONITORANDO_FONTE);
    }

    // Se o sensor deu ERRO, INSTÁVEL ou a rede VOLTOU:
    // Não importa o motivo, saímos do AUSENTE para tentar carregar.
    inicioContagem = millis();
    return REDE::definir(REDE::STATUS::AGUARDANDO_5MIN);
}

//====================================================================================
//                   AGUARDANDO 5 MINUTOS (ESTABILIZAÇÃO)
//====================================================================================

void Controle_Rede_Aguardando_5minutos()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();
    const auto& status = Leitura_Rede_ObterStatus();

    // Se o sensor der pau durante a espera, não espera mais: LIGA!
    if (diag.estado != SENSOR::OK) {
        return REDE::definir(REDE::STATUS::MONITORANDO_FONTE);
    }

    // Se o sensor está OK, mas a rede caiu de novo, volta pro zero
    if (!status.tem_rede) {
        return REDE::definir(REDE::STATUS::AUSENTE);
    }

    // Se a rede estiver firme pelo tempo programado...
    if (millis() - inicioContagem >= TEMPO_5MIN) {
        return REDE::definir(REDE::STATUS::MONITORANDO_FONTE);
    }

    return REDE::definir(REDE::STATUS::AGUARDANDO_5MIN);
}
//====================================================================================
//                   REDE INSTÁVEL (15 MINUTOS)
//====================================================================================

void Controle_Rede_Instavel_15minutos()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();
    const auto& status = Leitura_Rede_ObterStatus();

    // Se o sensor der erro aqui, cancelamos a "punição" e vamos pra carga
    if (diag.estado != SENSOR::OK) {
        return REDE::definir(REDE::STATUS::MONITORANDO_FONTE);
    }

    // Se a rede estiver OK e o sensor OK, esperamos o tempo longo
    if (status.tem_rede) {
        if (millis() - inicioContagem >= TEMPO_15MIN) {
            return REDE::definir(REDE::STATUS::MONITORANDO_FONTE);
        }
    } 
    else {
        // Se a rede cair DE NOVO durante os 15min, reseta o cronômetro
        inicioContagem = millis(); 
    }

    return REDE::definir(REDE::STATUS::INSTAVEL_15MIN);
}

//====================================================================================
//                   MONITORANDO FONTE USINA
//====================================================================================

void Controle_Rede_Monitorando_Fonte()
{
    const auto& diag = Diagnostico_ZMPT101B_ObterStatus();
    const auto& status = Leitura_Rede_ObterStatus();

    // 1. SÓ DESLIGA SE: O sensor estiver 100% OK e confirmar que a rede caiu (0V).
    if (diag.estado == SENSOR::OK && !status.tem_rede)
    {
        debugAviso("MONITORANDO >> Rede caiu confirmada. Desligando Usina.");
        return REDE::definir(REDE::STATUS::AUSENTE);
    }

    // 2. SE O SENSOR FICAR INSTÁVEL (Oscilação de leitura detectada pelo Diagnóstico)
    // Aqui mantemos a sua ideia: se estiver instável, entra no "castigo" de 15min.
    if (diag.estado == SENSOR::INSTAVEL)
    {
        debugAviso("MONITORANDO >> Rede instável. Entrando em modo 15min.");
        return REDE::definir(REDE::STATUS::INSTAVEL_15MIN);
    }

    // 3. SE O SENSOR DER ERRO OU ESTIVER OK COM REDE:
    // Se caiu no SENSOR::ERRO, ele pula os IFs acima e cai aqui. 
    // É o modo "Vai na Fé": liga a fonte e mantém o estado.

    return REDE::definir(REDE::STATUS::MONITORANDO_FONTE);
}
//====================================================================================
//                   CARGA COMPLETA (FINALIZAÇÃO)
//====================================================================================

void Controle_Rede_Carga_Finalizada()
{
    return REDE::definir(REDE::STATUS::DORMINDO);
}

//====================================================================================
//                   MODO CALAMIDADE
//====================================================================================

void Controle_Rede_Calamidade()
{
    return REDE::definir(REDE::STATUS::CALAMIDADE);
}