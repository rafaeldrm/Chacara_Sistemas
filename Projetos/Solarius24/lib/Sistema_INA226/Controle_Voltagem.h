#pragma once
#include <Arduino.h>
#include "Estado_Globais.h"

//=========================== LEITURA CENTRAL ===========================//

void Controle_Voltagem_Inicializa();

bool Controle_Voltagem_Atualizar_Tensao();

uint16_t Controle_Voltagem_Obter_Tensao();

bool Controle_Voltagem_Tem_Dado();

//=========================== LÓGICAS DE ESTADO =========================//

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Normal();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Auxiliar();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Emergencial();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Critico();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Automatico();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Fim_Automatico();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Calamidade_Inicio();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Calamidade_Completo();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Tentativa_Recuperar_Sistema();