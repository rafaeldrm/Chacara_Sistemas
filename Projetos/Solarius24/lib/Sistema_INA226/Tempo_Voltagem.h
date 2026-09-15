#pragma once
#include <Arduino.h>

//=========================== INTERFACE ===========================//

    void Tempo_Voltagem_Inicializa();
    
    bool Tempo_Voltagem_Deve_Atualizar(uint16_t tensao_atual);