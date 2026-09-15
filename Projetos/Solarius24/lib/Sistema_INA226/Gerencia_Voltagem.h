#pragma once
#include <Arduino.h>
#include "Estado_Globais.h" 

void Gerencia_Voltagem_Setup();

void Gerencia_Voltagem_Loop();

ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Normal();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Auxiliar();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Emergencial();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Critico();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Calamidade_Inicio();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Calamidade_Completo();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Tentativa_Recuperar_Sistema();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Automatico();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Fim_Automatico();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Recuperando_Carga();
ESTADO::VOLTAGEM::NIVEL Controle_Voltagem_Logica_Recuperado();