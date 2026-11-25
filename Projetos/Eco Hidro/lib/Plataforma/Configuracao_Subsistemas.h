
//-------------------------- DEPENDÊNCIAS ----------------------------------//

#pragma once
#include <Arduino.h>

//-------------------------- FUNÇÕES PÚBLICAS -------------------------------//

//==============================================================
//              CONFIGURAÇÃO OPERACIONAL
//                    ECO_HIDRO v7.0
//
//  1 = FUNCIONALIDADE HABILITADA
//  0 = FUNCIONALIDADE DESABILITADA
//
//  Alterações exigem recompilação e novo upload.
//
//==============================================================


//==============================================================
//                  SUBSISTEMAS
//==============================================================

#define ATIVAR_RESERVATORIO                    1

#define ATIVAR_LUZ_ESTACIONAMENTO              0
#define ATIVAR_LUZ_ACESSO_RESERVATORIO         0

#define ATIVAR_VOLTAGEM                        0


//==============================================================
//              MONITORAMENTO DAS BOMBAS
//==============================================================

#define ATIVAR_GIRO_PREVENTIVO_BOMBA_CASA      0

#define ATIVAR_TRANCO_BOMBA_CASA               1

#define ATIVAR_TERMICO_BOMBA_CASA              0


//==============================================================
//                    COMUNICAÇÃO
//==============================================================

#define ATIVAR_LORA                            0
