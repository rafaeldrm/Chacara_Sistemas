//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>
#include "Leitura_Temperatura.h"
#include "Diagnostico_DHT11.h"

//-------------------------- DADOS ------------------------------------------//

struct ClimatizacaoMPPT
{
    bool cooler_ligado;
    bool controle_ativo;
};

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

void Controle_Climatizacao_MPPT_Inicializa();

void Controle_Climatizacao_MPPT_Atualiza(const DadosLeituraDHT11& leitura, const StatusDiagnosticoDHT11& status);

//-------------------------- ACESSO AOS DADOS -------------------------------//

const ClimatizacaoMPPT& Controle_Climatizacao_MPPT_ObterEstado();