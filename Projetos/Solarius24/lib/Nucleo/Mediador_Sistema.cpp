#include "Mediador_Sistema.h"
#include "Estados_Sistema_Solarius24.h"
#include "DEBUG_MODE.h"
// Incluímos os headers das gerências para o mediador saber o que existe
#include "Gerencia_Rede.h"
#include "Gerencia_Voltagem.h"
#include "Gerencia_Carga_Urgencia.h"
#include "Gerencia_Baterias.h"
#include "Sistema_Seguranca.h"

//====================================================================================
//                   BANCADAS DE BATERIAS
//====================================================================================

static void Regras_Baterias_Loop() {

    static BATERIA::SITUACAO estadoAnterior = BATERIA::SITUACAO::NORMAL;
    auto estadoAtual = BATERIA::atual();

    if (estadoAtual != estadoAnterior)
    {
        // Houve transição de estado
        switch (estadoAtual)
        {
            case BATERIA::SITUACAO::CALAMIDADE_INICIO:
                URGENCIA::definir(URGENCIA::MODO::CALAMIDADE);
                REDE::definir(REDE::STATUS::CALAMIDADE);
                VOLTAGEM::definir(VOLTAGEM::NIVEL::CALAMIDADE_INICIO);

   //             Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)BATERIA::SITUACAO::CALAMIDADE_INICIO);
                Salvar_Estagio_Calamidade_EEPROM(BATERIA::SITUACAO::CALAMIDADE_INICIO); //EEPROM (Memoria_Watchdog_EEPROM.h/cpp)
   
            break;

            case BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO:

   //             Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO);
                Salvar_Estagio_Calamidade_EEPROM(BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO); //EEPROM
                VOLTAGEM::definir(VOLTAGEM::NIVEL::CALAMIDADE_COMPLETO);
            break;

            case BATERIA::SITUACAO::CALAMIDADE_COMPLETO:
                
    //            Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)BATERIA::SITUACAO::CALAMIDADE_COMPLETO);
                Salvar_Estagio_Calamidade_EEPROM(BATERIA::SITUACAO::CALAMIDADE_COMPLETO); //EEPROM

            break;

            case BATERIA::SITUACAO::TENTATIVA_RECUPERACAO:
   //             Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)BATERIA::SITUACAO::TENTATIVA_RECUPERACAO);
                Salvar_Estagio_Calamidade_EEPROM(BATERIA::SITUACAO::TENTATIVA_RECUPERACAO); //EEPROM
                VOLTAGEM::definir(VOLTAGEM::NIVEL::TENTATIVA_RECUPERACAO);
                REDE::definir(REDE::STATUS::DORMINDO);
                
            break;

            case BATERIA::SITUACAO::OBSERVA:
                // Estado de observação: nenhuma ação especial necessária no mediador
            break;

            case BATERIA::SITUACAO::NORMAL:
                URGENCIA::definir(URGENCIA::MODO::NORMAL);

   //             Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)BATERIA::SITUACAO::NORMAL);
                Salvar_Estagio_Calamidade_EEPROM(BATERIA::SITUACAO::NORMAL); //EEPROM
            
            break;
        }

        estadoAnterior = estadoAtual;
    }
}

//====================================================================================
//                   REDE ELÉTRICA
//====================================================================================

static void Regras_Urgencia_Loop() {
    auto modoUrgencia = URGENCIA::atual();
    auto nivelVoltagem = VOLTAGEM::atual();

    // Se o usuário apertou o botão ou a IA mandou (MODO AUTOMATICO)
    if (modoUrgencia == URGENCIA::MODO::AUTOMATICO) {
        // "Atropela" a voltagem atual e força o estado para AUTOMATICO
        if (nivelVoltagem != VOLTAGEM::NIVEL::AUTOMATICO) {
            debugAviso("MEDIADOR: Urgência detectada! Elevando teto de carga para 14.3V.");
            VOLTAGEM::definir(VOLTAGEM::NIVEL::AUTOMATICO);
            REDE::definir(REDE::STATUS::VERIFICANDO_5S);
        }
    }
}

//====================================================================================
//                   VOLTAGEM DAS BATERIAS
//====================================================================================

static void Regras_Voltagem_Loop() {
    auto statusVoltagem = VOLTAGEM::atual();
    auto statusRede = REDE::atual();


    // Regra: Tensão caiu -> Acorda ZMPT
    if (statusVoltagem == VOLTAGEM::NIVEL::AUXILIAR 
        || statusVoltagem == VOLTAGEM::NIVEL::EMERGENCIAL   
        || statusVoltagem == VOLTAGEM::NIVEL::CRITICO) 
    {
        if (statusRede == REDE::STATUS::DORMINDO) {
            REDE::definir(REDE::STATUS::VERIFICANDO_5S);
        }
    }

    // Regra: Tensão recuperada -> ZMPT dorme
    if (statusVoltagem == VOLTAGEM::NIVEL::NORMAL) {
            if (statusRede != REDE::STATUS::DORMINDO && statusRede != REDE::STATUS::CARGA_FINALIZADA) {
                REDE::definir(REDE::STATUS::CARGA_FINALIZADA);
            }
        }

    if (statusVoltagem == VOLTAGEM::NIVEL::FIM_AUTOMATICO) {
            REDE::definir(REDE::STATUS::CARGA_FINALIZADA);
            URGENCIA::definir(URGENCIA::MODO::NORMAL);
        }
}

//====================================================================================
//                   LOOP
//====================================================================================

void MEDIADOR_LOOP() {

    Regras_Baterias_Loop();
    Regras_Urgencia_Loop();
    Regras_Voltagem_Loop();
    
}