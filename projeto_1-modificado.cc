// SIMULACAO COMPLETA - BUS PIRATE + ATAQUE IoT (OTIMIZADO)
// Demonstra interceptação e exploração de vulnerabilidades

int ledVitima = 13;    // LED Vermelho - Sistema sendo atacado
int ledPirate = 12;    // LED Verde - Bus Pirate interceptador  
int ledAlerta = 11;    // LED Azul - Alertas de segurança

bool sistemaLigado = false;
bool atacanteConectado = false;
unsigned long ultimoAtaque = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledVitima, OUTPUT);
  pinMode(ledPirate, OUTPUT);
  pinMode(ledAlerta, OUTPUT);
  
  Serial.println(F("===================================="));
  Serial.println(F("   SIMULACAO BUS PIRATE ATTACK     "));
  Serial.println(F("===================================="));
  Serial.println();
  
  inicializarSistema();
  conectarBusPirate();
}

void loop() {
  if (Serial.available()) {
    String comando = Serial.readString();
    comando.trim();
    
    if (comando == "1") {
      demonstrarReconhecimento();
    }
    else if (comando == "2") {
      demonstrarAtaqueCredencial();
    }
    else if (comando == "3") {
      demonstrarControleRemoto();
    }
    else if (comando == "4") {
      demonstrarExtracaoDebug();
    }
    else if (comando == "AUTO") {
      ataqueCompleto();
    }
    else {
      mostrarMenu();
    }
  }
  
  // Animação Bus Pirate
  if (millis() - ultimoAtaque < 5000) {
    digitalWrite(ledPirate, millis() % 500 < 250);
  }
}

void inicializarSistema() {
  Serial.println(F("FASE 1: INICIALIZACAO SISTEMA"));
  Serial.println(F("Sistema IoT iniciando..."));
  
  for(int i = 0; i < 3; i++) {
    digitalWrite(ledVitima, HIGH);
    delay(300);
    digitalWrite(ledVitima, LOW);
    delay(300);
  }
  
  Serial.println(F(" Sistema online"));
  Serial.println(F(" UART ativo"));
  Serial.println(F(" VULNERAVEL: Senha padrao exposta!"));
  Serial.println(F(">>> Senha: 1234"));
  Serial.println();
  sistemaLigado = true;
}

void conectarBusPirate() {
  Serial.println(F("FASE 2: ATACANTE CONECTA BUS PIRATE"));
  Serial.println(F("Localizando pinos TX/RX..."));
  delay(1000);
  
  digitalWrite(ledPirate, HIGH);
  Serial.println(F(" Bus Pirate conectado"));
  Serial.println(F(" Interceptacao ativa"));
  Serial.println(F(" Monitorando trafego..."));
  Serial.println();
  
  atacanteConectado = true;
  mostrarMenu();
}

void mostrarMenu() {
  Serial.println(F("===== MENU DO ATACANTE ====="));
  Serial.println(F("1 - Reconhecimento passivo"));
  Serial.println(F("2 - Ataque de credencial"));
  Serial.println(F("3 - Controle remoto"));
  Serial.println(F("4 - Extracao modo DEBUG"));
  Serial.println(F("AUTO - Ataque completo"));
  Serial.println(F("============================"));
}

void demonstrarReconhecimento() {
  Serial.println();
  Serial.println(F("RECONHECIMENTO PASSIVO:"));
  Serial.println(F("Bus Pirate interceptando..."));
  
  piscarAlerta(3);
  
  Serial.println(F("[INTERCEPTADO] Sistema iniciado"));
  delay(500);
  Serial.println(F("[INTERCEPTADO] Firmware v1.0"));
  delay(500);
  Serial.println(F("[INTERCEPTADO] Senha: 1234"));
  
  digitalWrite(ledAlerta, HIGH);
  Serial.println(F("CREDENCIAL EXPOSTA!"));
  Serial.println(F("Acesso total possivel"));
  Serial.println();
  
  ultimoAtaque = millis();
}

void demonstrarAtaqueCredencial() {
  Serial.println();
  Serial.println(F("ATAQUE DE CREDENCIAL:"));
  Serial.println(F("Usando senha descoberta..."));
  
  Serial.println(F("[ENVIADO] AUTH:1234"));
  delay(1000);
  
  digitalWrite(ledVitima, HIGH);
  Serial.println(F("[INTERCEPTADO] ACESSO_LIBERADO"));
  Serial.println(F("Sistema comprometido!"));
  Serial.println(F("Controle total obtido"));
  Serial.println();
  
  ultimoAtaque = millis();
}

void demonstrarControleRemoto() {
  Serial.println();
  Serial.println(F("CONTROLE REMOTO:"));
  Serial.println(F("Executando comandos..."));
  
  Serial.println(F("[ENVIADO] LED_ON"));
  digitalWrite(ledVitima, HIGH);
  delay(1000);
  
  Serial.println(F("[INTERCEPTADO] LED_LIGADO"));
  Serial.println(F("Dispositivo controlado"));
  
  delay(2000);
  
  Serial.println(F("[ENVIADO] LED_OFF"));
  digitalWrite(ledVitima, LOW);
  Serial.println(F("[INTERCEPTADO] LED_DESLIGADO"));
  Serial.println(F("Controle fisico obtido"));
  Serial.println();
  
  ultimoAtaque = millis();
}

void demonstrarExtracaoDebug() {
  Serial.println();
  Serial.println(F("EXTRACAO MODO DEBUG:"));
  Serial.println(F("Comando secreto..."));
  
  piscarAlerta(5);
  
  Serial.println(F("[ENVIADO] DEBUG"));
  delay(1000);
  
  Serial.println(F("[INTERCEPTADO] === INFO CONFIDENCIAL ==="));
  delay(500);
  Serial.println(F("[INTERCEPTADO] Senha: 1234"));
  delay(500);
  Serial.println(F("[INTERCEPTADO] Versao: BETA-INSECURE"));
  
  digitalWrite(ledAlerta, HIGH);
  Serial.println(F("Dados criticos extraidos!"));
  Serial.println(F("SISTEMA COMPROMETIDO"));
  Serial.println();
  
  ultimoAtaque = millis();
}

void ataqueCompleto() {
  Serial.println();
  Serial.println(F("ATAQUE AUTOMATICO COMPLETO..."));
  Serial.println();
  
  delay(1000);
  demonstrarReconhecimento();
  delay(3000);
  demonstrarAtaqueCredencial();  
  delay(3000);
  demonstrarControleRemoto();
  delay(3000);
  demonstrarExtracaoDebug();
  
  Serial.println();
  Serial.println(F("===== ATAQUE CONCLUIDO ====="));
  Serial.println(F(" Credenciais capturadas"));
  Serial.println(F(" Acesso total obtido"));
  Serial.println(F(" Controle remoto ativo"));
  Serial.println(F(" Dados criticos extraidos"));
  Serial.println(F(" SISTEMA COMPROMETIDO"));
  Serial.println(F("============================"));
  
  // Efeito final
  for(int i = 0; i < 10; i++) {
    digitalWrite(ledVitima, HIGH);
    digitalWrite(ledPirate, HIGH);
    digitalWrite(ledAlerta, HIGH);
    delay(200);
    digitalWrite(ledVitima, LOW);
    digitalWrite(ledPirate, LOW);
    digitalWrite(ledAlerta, LOW);
    delay(200);
  }
}

void piscarAlerta(int vezes) {
  for(int i = 0; i < vezes; i++) {
    digitalWrite(ledAlerta, HIGH);
    delay(150);
    digitalWrite(ledAlerta, LOW);
    delay(150);
  }
}