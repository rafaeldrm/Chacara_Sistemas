//====================================================================================
//                   DEPENDÊNCIAS
//====================================================================================

#include <LoRa.h>
#include "LoRa_Transporte.h"
#include "DEBUG_MODE.h"

#include "Driver_LoRa.h"
//#include "Diagnostico_LoRa.h"

//====================================================================================
//                   VARIÁVEIS INTERNAS
//====================================================================================

//====================================================================================
//                   SETUP
//====================================================================================

void LoRa_Transporte_Inicializa()
{
        debugSetupDivisao(COR::AMARELO, "LORA TRANSPORTE INICIALIZANDO", COR::MAGENTA, "=", 45);
}

//====================================================================================
//                   ENVIA PACOTE DE MENSAGENS
//====================================================================================

bool LoRa_Enviar_Pacote(const LORA::Pacote& pacote)
{
    if (!Driver_LoRa_Online())
    {
        debugErro("LoRa >>> TENTATIVA DE ENVIO COM RÁDIO OFFLINE");
        return false;
    }



    debugAviso("LoRa >>> Preparando para enviar pacote...");

    LoRa.beginPacket();
    LoRa.write((const uint8_t*)&pacote, sizeof(LORA::Pacote));
    const int status = LoRa.endPacket();



    if (status != 1)
    {
        debugVariavel("LoRa >>> FALHA NO ENVIO DO PACOTE status: ", status, NENHUM, AMARELO, VERDE);
        return false;
    }



    debugOk("LoRa >>> PACOTE ENVIADO COM SUCESSO");
    LORA::PrintPacote(pacote);

    return true;
}

//====================================================================================
//                   RECEBE PACOTE DE MENSAGENS
//====================================================================================

bool LoRa_Receber_Pacote(LORA::Pacote& pacote)
{
    if (!Driver_LoRa_Online())
    {
        debugErro("LoRa <<< TENTATIVA DE RECEPÇÃO COM RÁDIO OFFLINE");
        return false;
    }



    const int size = LoRa.parsePacket();

    if (size == 0)
        return false;   // nada no ar: silêncio, para não encher o log



    debugVariavel("LoRa <<< Pacote detectado! Tamanho: ", size, NENHUM, AMARELO, VERDE);

    if (size != (int)sizeof(LORA::Pacote))
    {
        debugVariavel("LoRa <<< TAMANHO DE PACOTE INVÁLIDO!", size, NENHUM, AMARELO, VERDE);
        while (LoRa.available()) LoRa.read();
        return false;
    }



    const int bytesLidos = LoRa.readBytes((uint8_t*)&pacote, sizeof(LORA::Pacote));

    if (bytesLidos != (int)sizeof(LORA::Pacote))
    {
        debugVariavel("LoRa <<< LEITURA INCOMPLETA! Lidos: ", bytesLidos, NENHUM, AMARELO, VERDE);
        while (LoRa.available()) LoRa.read();
        return false;
    }



    debugOk("LoRa <<< PACOTE RECEBIDO (ainda não autenticado)");
    LORA::PrintPacote(pacote);

    return true;
}