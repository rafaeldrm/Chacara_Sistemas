#include "Mediador_Sistema.h"
#include "Estado_Globais.h"
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

void Regras_Baterias_Loop() {

    static ESTADO::BATERIA::SITUACAO estadoAnterior = ESTADO::BATERIA::SITUACAO::NORMAL;
    auto estadoAtual = ESTADO::BATERIA::atual();

    if (estadoAtual != estadoAnterior)
    {
        // Houve transição de estado
        switch (estadoAtual)
        {
            case ESTADO::BATERIA::SITUACAO::CALAMIDADE_INICIO:
                ESTADO::URGENCIA::definir(ESTADO::URGENCIA::MODO::CALAMIDADE);
                ESTADO::REDE::definir(ESTADO::REDE::STATUS::CALAMIDADE);
                ESTADO::VOLTAGEM::definir(ESTADO::VOLTAGEM::NIVEL::CALAMIDADE_INICIO);

   //             Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)ESTADO::BATERIA::SITUACAO::CALAMIDADE_INICIO);
                Salvar_Estagio_Calamidade_EEPROM(ESTADO::BATERIA::SITUACAO::CALAMIDADE_INICIO); //EEPROM (Memoria_Watchdog_EEPROM.h/cpp)
   
            break;

            case ESTADO::BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO:

   //             Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)ESTADO::BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO);
                Salvar_Estagio_Calamidade_EEPROM(ESTADO::BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO); //EEPROM
                ESTADO::VOLTAGEM::definir(ESTADO::VOLTAGEM::NIVEL::CALAMIDADE_COMPLETO);
            break;

            case ESTADO::BATERIA::SITUACAO::CALAMIDADE_COMPLETO:
                
    //            Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)ESTADO::BATERIA::SITUACAO::CALAMIDADE_COMPLETO);
                Salvar_Estagio_Calamidade_EEPROM(ESTADO::BATERIA::SITUACAO::CALAMIDADE_COMPLETO); //EEPROM

            break;

            case ESTADO::BATERIA::SITUACAO::TENTATIVA_RECUPERACAO:
   //             Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)ESTADO::BATERIA::SITUACAO::TENTATIVA_RECUPERACAO);
                Salvar_Estagio_Calamidade_EEPROM(ESTADO::BATERIA::SITUACAO::TENTATIVA_RECUPERACAO); //EEPROM
                ESTADO::VOLTAGEM::definir(ESTADO::VOLTAGEM::NIVEL::TENTATIVA_RECUPERACAO);
                ESTADO::REDE::definir(ESTADO::REDE::STATUS::DORMINDO);
                
            break;

            case ESTADO::BATERIA::SITUACAO::OBSERVA:
                // Estado de observação: nenhuma ação especial necessária no mediador
            break;

            case ESTADO::BATERIA::SITUACAO::NORMAL:
                ESTADO::URGENCIA::definir(ESTADO::URGENCIA::MODO::NORMAL);

   //             Comunicacao_Lora_Envia_Evento(LORA::CODIGO::SOLAR_ESTADO_CALAMIDADE,(uint16_t)ESTADO::BATERIA::SITUACAO::NORMAL);
                Salvar_Estagio_Calamidade_EEPROM(ESTADO::BATERIA::SITUACAO::NORMAL); //EEPROM
            
            break;
        }

        estadoAnterior = estadoAtual;
    }
}

//====================================================================================
//                   REDE ELÉTRICA
//====================================================================================

void Regras_Urgencia_Loop() {
    auto modoUrgencia = ESTADO::URGENCIA::atual();
    auto nivelVoltagem = ESTADO::VOLTAGEM::atual();

    // Se o usuário apertou o botão ou a IA mandou (MODO AUTOMATICO)
    if (modoUrgencia == ESTADO::URGENCIA::MODO::AUTOMATICO) {
        // "Atropela" a voltagem atual e força o estado para AUTOMATICO
        if (nivelVoltagem != ESTADO::VOLTAGEM::NIVEL::AUTOMATICO) {
            debugAviso("MEDIADOR: Urgência detectada! Elevando teto de carga para 14.3V.");
            ESTADO::VOLTAGEM::definir(ESTADO::VOLTAGEM::NIVEL::AUTOMATICO);
            ESTADO::REDE::definir(ESTADO::REDE::STATUS::VERIFICANDO_5S);
        }
    }
}

//====================================================================================
//                   VOLTAGEM DAS BATERIAS
//====================================================================================

void Regras_Voltagem_Loop() {
    auto statusVoltagem = ESTADO::VOLTAGEM::atual();
    auto statusRede = ESTADO::REDE::atual();


    // Regra: Tensão caiu -> Acorda ZMPT
    if (statusVoltagem == ESTADO::VOLTAGEM::NIVEL::AUXILIAR 
        || statusVoltagem == ESTADO::VOLTAGEM::NIVEL::EMERGENCIAL   
        || statusVoltagem == ESTADO::VOLTAGEM::NIVEL::CRITICO) 
    {
        if (statusRede == ESTADO::REDE::STATUS::DORMINDO) {
            ESTADO::REDE::definir(ESTADO::REDE::STATUS::VERIFICANDO_5S);
        }
    }

    // Regra: Tensão recuperada -> ZMPT dorme
    if (statusVoltagem == ESTADO::VOLTAGEM::NIVEL::NORMAL) {
            if (statusRede != ESTADO::REDE::STATUS::DORMINDO && statusRede != ESTADO::REDE::STATUS::CARGA_FINALIZADA) {
                ESTADO::REDE::definir(ESTADO::REDE::STATUS::CARGA_FINALIZADA);
            }
        }

    if (statusVoltagem == ESTADO::VOLTAGEM::NIVEL::FIM_AUTOMATICO) {
            ESTADO::REDE::definir(ESTADO::REDE::STATUS::CARGA_FINALIZADA);
            ESTADO::URGENCIA::definir(ESTADO::URGENCIA::MODO::NORMAL);
        }
}

//====================================================================================
//                   LOOP
//====================================================================================

void Mediador_Sistema_Loop() {

    Regras_Baterias_Loop();
    Regras_Urgencia_Loop();
    Regras_Voltagem_Loop();
    
}