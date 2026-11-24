//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

#include "Protocolo_Chacara.h"

 //-------------------------- DADOS ------------------------------------------//

 //-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Ordem_Recebimento_Lora_Inicializa();

void Ordem_Recebimento_LoRa_Atualiza();   // escuta o rádio e executa as ordens recebidas

bool Ordem_Recebimento_LoRa(const LORA::Pacote& pacote); //interpreta mensagens recebidas do sistema residencial

