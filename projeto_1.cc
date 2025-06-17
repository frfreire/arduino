// Sistema de Controle LED via Serial
// ATENÇÃO: Este código tem vulnerabilidades INTENCIONAIS para aprendizado

String senha = "1234"; // PROBLEMA 1: senha visível no código
int ledPin = 13;
bool autenticado = false;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  
  // PROBLEMA 2: mostra informações sensíveis na inicialização
  Serial.println("=== SISTEMA INICIADO ===");
  Serial.println("Firmware v1.0 - Debug Mode");
  Serial.println("Senha default: " + senha); // MUITO PERIGOSO!
  Serial.println("Digite 'AUTH:senha' para autenticar");
  Serial.println("Comandos: LED_ON, LED_OFF, STATUS");
}

void loop() {
  if (Serial.available()) {
    String comando = Serial.readString();
    comando.trim();
    
    // PROBLEMA 3: registra todos os comandos digitados
    Serial.println("CMD_LOG: " + comando);
    
    // Verificação de autenticação
    if (comando.startsWith("AUTH:")) {
      String senhaDigitada = comando.substring(5);
      if (senhaDigitada == senha) {
        autenticado = true;
        Serial.println("ACESSO_LIBERADO");
      } else {
        // PROBLEMA 4: mostra a senha que a pessoa tentou
        Serial.println("ACESSO_NEGADO - Tentativa: " + senhaDigitada);
      }
      return;
    }
    
    // Comandos do sistema (só funciona se autenticado)
    if (autenticado) {
      if (comando == "LED_ON") {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED_LIGADO");
      }
      else if (comando == "LED_OFF") {
        digitalWrite(ledPin, LOW);
        Serial.println("LED_DESLIGADO");
      }
      else if (comando == "STATUS") {
        Serial.println("Sistema: ATIVO");
        Serial.println("LED: " + String(digitalRead(ledPin) ? "ON" : "OFF"));
        Serial.println("Tempo ligado: " + String(millis()) + "ms");
      }
      else if (comando == "DEBUG") {
        // PROBLEMA 5: comando secreto que vaza informações
        Serial.println("=== INFORMAÇÕES CONFIDENCIAIS ===");
        Serial.println("Senha do sistema: " + senha);
        Serial.println("Memória livre: 1024 bytes");
        Serial.println("Versão: 1.0-BETA-INSECURE");
      }
    } else {
      Serial.println("ERRO: Você precisa se autenticar primeiro");
    }
  }
  
  delay(100);
}