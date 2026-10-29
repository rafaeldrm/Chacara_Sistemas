/*
ARQUIVO: Log_Reservatorio.h
OBJETIVO: Armazenar os logs de eventos do sistema de água para análise da IA.
*/

#pragma once
#include <Arduino.h>

namespace LOG {

    // Representa o motivo que disparou a ação na bomba
    enum class MOTIVO_BOMBA : uint8_t {
        NIVEL_CRITICO,   // Boia física acusou vazio
        NIVEL_OBSERVACAO, //Boia chegou pouco abaixo da metade
        ANTECIPACAO,     // IA decidiu ligar antes de você sair
        MANUAL,          // Você forçou pelo display ou voz
        ROTINA_LAVAGEM   // Dia de máquina de lavar (Seg/Ter/Qui/Sex)
    };

    struct Reservatorio {
        uint32_t timestamp;      // Horário do evento (do DS3231)
        
        // Tensão da bateria (ex: 12.5V). 
        // Dica: Salve como (voltagem * 10) em um uint8_t. 
        // 12.5V vira 125. Economiza 3 bytes comparado a um float.
        uint8_t tensaoBateria;   
        
        uint8_t nivelAgua;       // Porcentagem ou nível (0-100)
        
        MOTIVO_BOMBA motivo;     // Por que a bomba alterou o estado
        
        // Status usando Bitfield (campos de bits) para economizar o máximo de RAM
        // Cabe tudo em 1 byte
        struct Status {
            uint8_t bombaLigada   : 1; // 0 ou 1
            uint8_t boiaCasaMedia     : 1; // 0 ou 1
            uint8_t boiaCasaMinima   : 1; // 0 ou 1
            uint8_t boiaTorreMaxima   : 1; // 0 ou 1
            uint8_t reserva       : 4; // Espaço para futuras expansões
        } flags;

    } __attribute__((packed)); 

}