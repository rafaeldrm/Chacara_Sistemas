//-------------------------- DEPENDÊNCIAS ----------------------------------// 

#pragma once 
#include <Arduino.h> 
#include "Tabela_Categorias_Nichos.h"

//-------------------------- ESTRUTURAS DE ESTADOS DO SISTEMA ----------------------------------// 

namespace NICHO 
{ 
    enum class ETAPA : uint8_t 
    { 
        NENHUM,             // Repouso (porta fechada, sistema em espera)
        ACESSANDO,          // Porta aberta (exibe contagem atual no OLED)
        CONTABILIZANDO,     // Fechou a porta: incrementa +1 na contagem
        PRONTO_PARA_LAVAR,  // Nicho cheio: ao abrir a porta, avisa "LAVAR ROUPAS"
        RESETANDO,          // Fechou a porta vindo de PRONTO_PARA_LAVAR: define contagem = 1
        FINALIZADO          // Trava de segurança de 1 minuto (Cooldown/Debounce)
    }; 

    void definir(uint8_t indice, ETAPA novo); 
    ETAPA atual(uint8_t indice); 
    const __FlashStringHelper* texto(ETAPA s); 
}