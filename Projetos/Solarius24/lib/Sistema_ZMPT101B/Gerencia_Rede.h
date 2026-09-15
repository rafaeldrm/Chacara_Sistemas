//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Estado_Globais.h"

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Gerencia_Rede_Inicializa();

void Gerencia_Rede_Atualiza();

//-------------------------- ESTADOS SISTEMA --------------------------------//

ESTADO::REDE::STATUS Controle_Rede_Logica_Verificando_5s();
ESTADO::REDE::STATUS Controle_Rede_Logica_Ausente();
ESTADO::REDE::STATUS Controle_Rede_Logica_Aguardando_5min();
ESTADO::REDE::STATUS Controle_Rede_Logica_Instavel_15min();
ESTADO::REDE::STATUS Controle_Rede_Logica_Monitorando_Fonte();
ESTADO::REDE::STATUS Controle_Rede_Logica_Carga_Finalizada();
ESTADO::REDE::STATUS Controle_Rede_Logica_Calamidade();