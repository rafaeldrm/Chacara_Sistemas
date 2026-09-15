#include "Sistema_Acoes.h"
#include "Modulo_Reles.h"

#include "Estados_Modulos.h"
#include "Diagnostico_ZMPT101B.h"
#include "DEBUG_MODE.h"
#include "Executa_Disjuntor.h"

void ACOES_Rede_Aplicar_Saida(ESTADO::REDE::STATUS estado)
{
    switch (estado)
    {
        case ESTADO::REDE::STATUS::MONITORANDO_FONTE:
            FONTE_USINA.Ligar();
            break;

        default:
            FONTE_USINA.Desligar();
            break;
    }
}

void ACOES_Voltagem_Aplicar_Saida(ESTADO::VOLTAGEM::NIVEL estado)
 {
        switch (estado)
    {

        case ESTADO::VOLTAGEM::NIVEL::EMERGENCIAL:
            GELADEIRA.Desligar();
            break;

        case ESTADO::VOLTAGEM::NIVEL::CRITICO:
            GELADEIRA.Desligar();
            INVERSOR.Desligar();
            break;
        
        case ESTADO::VOLTAGEM::NIVEL::RECUPERANDO_CARGA:
            INVERSOR.Ligar();
            break;

        case ESTADO::VOLTAGEM::NIVEL::RECUPERADO:
            GELADEIRA.Ligar();
            break;

        case ESTADO::VOLTAGEM::NIVEL::CALAMIDADE_INICIO:
            GELADEIRA.Desligar();
            break;

        case ESTADO::VOLTAGEM::NIVEL::CALAMIDADE_COMPLETO:
            GELADEIRA.Desligar();
            INVERSOR.Desligar();
            break;

            
            default: 
            break;
}
 }

void SISTEMA_ACOES_LOOP(){
    
        ACOES_Voltagem_Aplicar_Saida(ESTADO::VOLTAGEM::atual());
        ACOES_Rede_Aplicar_Saida(ESTADO::REDE::atual());


}