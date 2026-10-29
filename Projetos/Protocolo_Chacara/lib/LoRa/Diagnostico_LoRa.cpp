//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include "Diagnostico_LoRa.h"
#include "DEBUG_MODE.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

struct EstadoLoRaDiagnostico
{
    LORA::DIAGNOSTICO loRaDiagnostico;

};

static EstadoLoRaDiagnostico estado =
{
    LORA::DIAGNOSTICO::NORMAL,
};


//====================================================================================
//                   FSM DIAGNOSTICO
//====================================================================================
namespace LORA 
{
    void definir(DIAGNOSTICO novo)
   {
        if (estado.loRaDiagnostico == novo) return;
        estado.loRaDiagnostico = novo;
        debugVariavel("DIAGNOSTICO loraDiagnostico : ", texto(novo), NENHUM, VERDE, AMARELO);
    }

    DIAGNOSTICO atualDiagnostico()
    {
        return estado.loRaDiagnostico;
    }

    const __FlashStringHelper* texto(DIAGNOSTICO d)
    {
        switch(d)

        {
            case DIAGNOSTICO::NORMAL:                                return F("NORMAL");
            case DIAGNOSTICO::LORA_COM_FALHAS:                       return F("LORA_COM_FALHAS");
            case DIAGNOSTICO::LORA_REINICIANDO:                      return F("LORA_REINICIANDO");
            case DIAGNOSTICO::RESETANDO_ARDUINO:                     return F("RESETANDO_ARDUINO");
            case DIAGNOSTICO::LORA_DESATIVADO:                       return F("LORA_DESATIVADO");  
            
            default: return F("DESCONHECIDO");
        }   
    }

}

//====================================================================================
//                   SETUP
//====================================================================================

void Diagnostico_LoRa_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "DIAGNOSTICO LORA INICIALIZANDO", COR::MAGENTA, "=", 45);
}