//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Controle_Rede_Inicializa();

void Controle_Rede_Ativa();

bool Controle_Rede_Pronto_Para_Ligar();

void Controle_Rede_Verificando_5segundos();

void Controle_Rede_Ausente();

void Controle_Rede_Aguardando_5minutos();

void Controle_Rede_Instavel_15minutos();

void Controle_Rede_Monitorando_Fonte();

void Controle_Rede_Carga_Finalizada();

void Controle_Rede_Calamidade();