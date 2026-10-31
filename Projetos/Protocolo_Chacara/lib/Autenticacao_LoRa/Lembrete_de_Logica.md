# Autenticacao_LoRa — Eco_Hidro

Primeira estrutura da camada de proteção do LoRa.

Objetivo:
- chave compartilhada Eco_Hidro ↔ Drimel System;
- MAC baseado em SipHash-2-4;
- sequência de mensagens;
- estado de sincronização após BOOT;
- deduplicação das 3 cópias internas da mesma mensagem;
- classificação de anomalias para uma futura Analise_LoRa.

IMPORTANTE:
- Esta versão NÃO altera `Protocolo_Chacara.h`.
- O formato definitivo do pacote ainda não está implementado.
- O transporte LoRa não pertence a esta camada.
- O Drimel multi-sistema será tratado posteriormente.
- `SipHash_2_4` é tratado como biblioteca externa/adaptador.

Arquivos:
- Chaves_LoRa.h/.cpp
- Mac_LoRa.h/.cpp
- Sequencia_LoRa.h/.cpp
- Estado_Comunicacao_LoRa.h/.cpp
- Analise_LoRa.h/.cpp
- Autenticacao_LoRa.h/.cpp

Fluxo:
Mensagem de saída:
  dados → sequência → MAC → pacote final (posteriormente)

Mensagem de entrada:
  pacote → MAC → sequência → deduplicação → análise → aceita/rejeita

A camada não transmite nada.

A implementação atual de `Mac_LoRa` usa diretamente a API da biblioteca `SipHash_2_4` fornecida para a Eco_Hidro/AVR. O Due deverá receber outro adaptador compatível quando o lado do Drimel for tratado.
