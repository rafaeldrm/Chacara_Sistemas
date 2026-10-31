//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Diagnostico_INA3221.h"


//-------------------------- DADOS ------------------------------------------//

struct EmergenciaBaterias
{
    bool ativo;
    bool modo_degradado;
};


//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Emergencia_Baterias_Inicializa();

void Emergencia_Baterias_Atualiza(const StatusDiagnosticoINA3221& diag);


//-------------------------- ACESSO AOS DADOS -------------------------------//

const EmergenciaBaterias& Emergencia_Baterias_Obter();