#include "Sistema_Seguranca.h"
#include <avr/wdt.h> //Biblioteca Watchdog (Segurança contra travamentos do microcontrolador)
#include <EEPROM.h>  //Biblioteca da EEPROM
#include "DEBUG_MODE.h" //Biblioteca de ativamento e desativamento de mensagens e delays para leitura de testes
#include "Estado_Globais.h"
#include "Controle_Baterias.h"

//====================================================================================
//                   VARIÁVEIS
//====================================================================================

 uint16_t menorValorMemoriaRam = 65535;
// Limite mínimo de RAM antes de forçar reset (Bytes)
constexpr uint16_t LIMITE_RAM_CRITICO = 400;

// Endereço de memória na EEPROM (Byte 0)
// Dando um nome para a área onde será gravada na EEPROM
const uint8_t ENDERECO_ESTADO_SISTEMA = 0;

//==============================================================================================================================================================
//                                                                         
//                                                               MEMÓRIA RAM
//
//==============================================================================================================================================================

//====================================================================================
//                   DIAGNÓSTICO — MEMÓRIA RAM LIVRE
//====================================================================================

// Mede a quantidade de RAM livre disponível no microcontrolador.
// Baseado na distância entre o Stack Pointer atual e o final do Heap.

uint16_t Diagnostico_RamLivre() {
    extern char __heap_start;
    extern char *__brkval;

    char* heapFim = (__brkval == nullptr) ? &__heap_start : __brkval;

    return (uint16_t)SP - (uint16_t)heapFim;
}   


//====================================================================================
//                   (MEMÓRIA RAM) LOOP
//====================================================================================

void Monitorar_RAM_Atualiza()
{
    static uint32_t ultimoTempo = 0;
    const uint32_t INTERVALO = 5UL * 60UL * 1000UL; // 5 minutos

    uint32_t agora = millis();

    if (agora - ultimoTempo < INTERVALO)
        return;

    ultimoTempo = agora;

    uint16_t livre = Diagnostico_RamLivre();

    if (livre < menorValorMemoriaRam)
        menorValorMemoriaRam = livre;

    debugVariavel("RAM LIVRE :: ", livre, NEGRITO, VERDE, LARANJA);

    if (livre < LIMITE_RAM_CRITICO)
    {
        debugErro("RAM CRÍTICA! RESETANDO...");
        while (true); // watchdog resolve
    }
}

//==============================================================================================================================================================
//                                                                         
//                                                               WATCHDOG
//
//==============================================================================================================================================================

//====================================================================================
//                   (WATCHDOG) SETUP ATIVA WATCHDOG
//====================================================================================

void Watchdog_Sistema_Inicializa()
{

// --- SEGURANÇA WATCHDOG ATIVADO---
    wdt_enable(WDTO_8S);
    // --- SEGURANÇA WATCHDOG ATIVADO---
    debugOk("Watchdog Sistema pronta");
}

//====================================================================================
//                   (WATCHDOG) LOOP RENOVA WATCHDOG
//====================================================================================

void Watchdog_Sistema_Atualiza() {

    wdt_reset(); //resetando o watchdog pra evitar reset do Arduino sem motivos
    
}


//==============================================================================================================================================================
//                                                                         
//                                                               EEPROM
//
//==============================================================================================================================================================

//====================================================================================
//                   (EEPROM)  FUNÇÃO QUE VERIFICA QUAL ESTÁGIO DE CALAMIDADE ESTÁ
//====================================================================================

bool Estado_Bateria_DevePersistir(ESTADO::BATERIA::SITUACAO estado)
{
    switch (estado)
    {
        case ESTADO::BATERIA::SITUACAO::CALAMIDADE_INICIO:
        case ESTADO::BATERIA::SITUACAO::CALAMIDADE_DISJUNTOR_DESLIGADO:
        case ESTADO::BATERIA::SITUACAO::CALAMIDADE_COMPLETO:
        case ESTADO::BATERIA::SITUACAO::TENTATIVA_RECUPERACAO:
            return true;

        default:
            return false;
    }
}

//====================================================================================
//                   (EEPROM)  SETUP
//====================================================================================

void EEPROM_Inicializa()
{
    debugSetupArquivo();
    debugSetupDivisao(TEXTO_AMARELO, "EEPROM CARREGANDO", TEXTO_VERDE, "=", 120);

    uint8_t valorLido = EEPROM.read(ENDERECO_ESTADO_SISTEMA);

    // Proteção contra lixo na EEPROM
    if (valorLido > static_cast<uint8_t>(ESTADO::BATERIA::SITUACAO::TENTATIVA_RECUPERACAO))
    {
        debugAviso("EEPROM inválida! Ignorando valor...");
        return;
    }

    ESTADO::BATERIA::SITUACAO estadoRecuperado =
        (ESTADO::BATERIA::SITUACAO)valorLido;

    // Só aplica se for calamidade
    if (Estado_Bateria_DevePersistir(estadoRecuperado))
    {
        ESTADO::BATERIA::definir(estadoRecuperado);

        debugAviso("Sistema reiniciado em estado crítico!");
        debugVariavel("ESTADO RECUPERADO :: ",
                      ESTADO::BATERIA::texto(estadoRecuperado),
                      NEGRITO, VERDE, AMARELO);
    }

    if (estadoRecuperado == ESTADO::BATERIA::SITUACAO::CALAMIDADE_COMPLETO)
{
    Controle_Baterias_Ativar_Modo_Calamidade();
}

    debugSetupDivisao(TEXTO_AMARELO, "EEPROM CARREGADO", TEXTO_MAGENTA, "=", 120);
}

//====================================================================================
//                   (EEPROM)  SALVAR NA EEPROM
//====================================================================================

// Use o Enum do Estado_Sistema.h
void Salvar_Estagio_Calamidade_EEPROM(ESTADO::BATERIA::SITUACAO estagio)
{
    static ESTADO::BATERIA::SITUACAO ultimoGravado = ESTADO::BATERIA::SITUACAO::NORMAL;

    // ignora OBSERVA
    if (estagio == ESTADO::BATERIA::SITUACAO::OBSERVA)
        return;

    // NORMAL só salva se for mudança real (limpeza pós-recuperação)
    if (estagio == ESTADO::BATERIA::SITUACAO::NORMAL)
    {
        EEPROM.update(ENDERECO_ESTADO_SISTEMA, static_cast<uint8_t>(estagio));
        ultimoGravado = estagio;

        debugAviso("EEPROM: Sistema voltou ao NORMAL!");
        return;
    }

    // só salva estados críticos e recuperação
    if (!Estado_Bateria_DevePersistir(estagio))
        return;

    if (estagio != ultimoGravado)
    {
        EEPROM.update(ENDERECO_ESTADO_SISTEMA, static_cast<uint8_t>(estagio));
        ultimoGravado = estagio;

        debugAviso("EEPROM: Estado crítico salvo!");
        debugVariavel("ESTADO :: ",
                      ESTADO::BATERIA::texto(estagio),
                      NEGRITO, VERDE, LARANJA);
    }
}