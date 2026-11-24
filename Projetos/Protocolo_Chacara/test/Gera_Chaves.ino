void setup() {
  // Inicializa a comunicação serial a 9600 bps
  Serial.begin(9600);
  
  // Inicializa o gerador aleatório usando o ruído do pino analógico A0 (desconectado)
  // Isso ajuda a evitar que o Arduino gere sempre a mesma sequência ao reiniciar
  randomSeed(analogRead(A0));
  
  Serial.println("--- GERADOR DE CHAVES DE 16 BITS ---");
  Serial.println("Pressione o botao RESET do Arduino ou envie qualquer caractere para gerar mais chaves.");
  Serial.println();
  
  gerarChaves(5); // Gera 5 chaves iniciais
}

void loop() {
  // Se você digitar qualquer coisa no terminal e enviar, ele gera novas chaves
  if (Serial.available() > 0) {
    while(Serial.available() > 0) { Serial.read(); } // Limpa o buffer do serial
    gerarChaves(5);
  }
}

void gerarChaves(int quantidade) {
  for (int i = 0; i < quantidade; i++) {
    // random(min, max) -> gera de min ate max-1. 
    // Para 16 bits, o limite maximo exclusivo eh 65536 (0 a 65535)
    uint16_t chave16bits = (uint16_t)random(0, 65536);
    
    Serial.print("Chave ");
    Serial.print(i + 1);
    Serial.print(" -> ");
    
    // Formato Decimal Inteiro
    Serial.print("DEC: ");
    Serial.print(chave16bits);
    
    // Formato Hexadecimal (4 caracteres)
    Serial.print(" | HEX: ");
    if(chave16bits < 0x1000) Serial.print("0"); // Adiciona zeros à esquerda se necessário
    if(chave16bits < 0x0100) Serial.print("0");
    if(chave16bits < 0x0010) Serial.print("0");
    Serial.print(chave16bits, HEX);
    
    // Formato Binário (16 bits)
    Serial.print(" | BIN: ");
    for (int bit = 15; bit >= 0; bit--) {
      Serial.print(bitRead(chave16bits, bit));
    }
    
    Serial.println();
  }
  Serial.println("---------------------------------------");
}
