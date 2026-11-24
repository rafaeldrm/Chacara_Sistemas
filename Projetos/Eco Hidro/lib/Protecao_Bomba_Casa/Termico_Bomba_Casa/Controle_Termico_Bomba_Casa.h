//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- DADOS ------------------------------------------//

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

 void Controle_Termico_Bomba_Casa_Inicializa();

 void Controle_Termico_Bomba_Casa_Superaquecimento();

 void Controle_Termico_Bomba_Casa_Monitoramento_Ativado();

 void Controle_Termico_Bomba_Casa_Modo_Seguranca_Primeiro_Ciclo();

 void Controle_Termico_Bomba_Casa_Modo_Seguranca_Segundo_Ciclo();

 void Controle_Termico_Bomba_Casa_Finalizado();
 

//-------------------------- ACESSO AOS DADOS -------------------------------//

 bool Controle_Termico_Bomba_Casa_Atualiza();