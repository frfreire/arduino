/*
====================================================================
SIMULAÇÃO DE TIMING ATTACK - SISTEMA DE SENHA VULNERÁVEL
====================================================================
Este código demonstra uma vulnerabilidade de timing attack
onde o tempo de resposta varia baseado na correção da senha.

VULNERABILIDADE: O sistema para de verificar no primeiro 
caractere incorreto, criando diferenças de timing mensuráveis.

Componentes necessários:
- Arduino Uno
- Keypad 4x4 
- LCD 16x2
- LED Verde (Acesso Permitido)
- LED Vermelho (Acesso Negado)
- Resistores 220Ω para LEDs

Montagem:
- Keypad: Pinos 2-9
- LCD: Pinos A0-A5  
- LED Verde: Pino 12
- LED Vermelho: Pino 13
====================================================================
*/

#include <Keypad.h>
#include <LiquidCrystal.h>

// Configuração do LCD (RS, EN, D4, D5, D6, D7)
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

// Configuração do Keypad 4x4
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Configuração dos LEDs
const int LED_VERDE = 12;  // Acesso permitido
const int LED_VERMELHO = 13; // Acesso negado

// Configuração da senha
const String SENHA_CORRETA = "1234";  // Senha do sistema
String senhaDigitada = "";
const int MAX_TENTATIVAS = 3;
int tentativasRestantes = MAX_TENTATIVAS;

// Variáveis para análise de timing
unsigned long tempoInicio = 0;
unsigned long tempoFim = 0;
bool modoAnalise = false;

void setup() {
  // Inicialização
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  
  // Tela inicial
  mostrarTelaInicial();
  
  Serial.println("====================================");
  Serial.println("SISTEMA DE SENHA - TIMING ATTACK DEMO");
  Serial.println("====================================");
  Serial.println("Senha correta: " + SENHA_CORRETA);
  Serial.println("Modo de análise ativado!");
  Serial.println("Pressione 'D' para modo de ataque");
  Serial.println("====================================");
}

void loop() {
  char tecla = keypad.getKey();
  
  if (tecla) {
    // Modo especial de análise - ativado pela tecla 'D'
    if (tecla == 'D') {
      modoAnalise = !modoAnalise;
      if (modoAnalise) {
        lcd.clear();
        lcd.print("MODO ANALISE ON");
        lcd.setCursor(0, 1);
        lcd.print("Timing visivel");
        Serial.println("\n>>> MODO ANÁLISE ATIVADO <<<");
      } else {
        mostrarTelaInicial();
        Serial.println("\n>>> MODO NORMAL <<<");
      }
      delay(1500);
      mostrarTelaInicial();
      return;
    }
    
    // Reset com '*'
    if (tecla == '*') {
      resetarSistema();
      return;
    }
    
    // Confirma entrada com '#'
    if (tecla == '#') {
      if (senhaDigitada.length() > 0) {
        verificarSenha();
      }
      return;
    }
    
    // Adiciona dígito à senha (apenas números)
    if (tecla >= '0' && tecla <= '9') {
      if (senhaDigitada.length() < 8) { // Máximo 8 dígitos
        senhaDigitada += tecla;
        atualizarDisplay();
      }
    }
  }
}

void mostrarTelaInicial() {
  lcd.clear();
  lcd.print("SISTEMA SEGURO");
  lcd.setCursor(0, 1);
  lcd.print("Digite senha:");
  senhaDigitada = "";
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
}

void atualizarDisplay() {
  lcd.clear();
  lcd.print("Senha:");
  lcd.setCursor(0, 1);
  
  // Mostra asteriscos para a senha
  for (int i = 0; i < senhaDigitada.length(); i++) {
    lcd.print("*");
  }
  lcd.print(" (# p/ OK)");
}

// FUNÇÃO VULNERÁVEL - Implementação com timing attack
bool verificarSenhaVulneravel(String senha) {
  // Esta função é INTENCIONALMENTE vulnerável!
  // Ela para na primeira diferença, criando timing diferente
  
  if (senha.length() != SENHA_CORRETA.length()) {
    delay(50); // Delay mínimo para senhas de tamanho incorreto
    return false;
  }
  
  // VULNERABILIDADE: Para no primeiro caractere diferente
  for (int i = 0; i < senha.length(); i++) {
    delay(100); // Simula tempo de processamento por caractere
    
    if (senha[i] != SENHA_CORRETA[i]) {
      // FALHA DE SEGURANÇA: Retorna imediatamente
      return false;
    }
  }
  
  delay(100); // Processamento final
  return true;
}

void verificarSenha() {
  // Registra tempo de início
  tempoInicio = millis();
  
  if (modoAnalise) {
    Serial.println("\n--- ANÁLISE DE TIMING ---");
    Serial.println("Senha digitada: " + senhaDigitada);
    Serial.println("Senha correta:  " + SENHA_CORRETA);
    Serial.print("Iniciando verificação... ");
  }
  
  // Chama função vulnerável
  bool senhaCorreta = verificarSenhaVulneravel(senhaDigitada);
  
  // Registra tempo final
  tempoFim = millis();
  unsigned long tempoDecorrido = tempoFim - tempoInicio;
  
  if (modoAnalise) {
    Serial.println("Concluído!");
    Serial.println("Tempo decorrido: " + String(tempoDecorrido) + "ms");
    
    // Análise da vulnerabilidade
    analisarTiming(tempoDecorrido);
  }
  
  if (senhaCorreta) {
    acessoPermitido();
  } else {
    acessoNegado();
    tentativasRestantes--;
    
    if (tentativasRestantes <= 0) {
      sistemaBloquado();
    }
  }
}

void analisarTiming(unsigned long tempo) {
  Serial.println("--- ANÁLISE DA VULNERABILIDADE ---");
  
  // Calcula quantos caracteres provavelmente estavam corretos
  // baseado no tempo (cada caractere = ~100ms + overhead)
  int caracteresCorretos = (tempo - 150) / 100; // 150ms = overhead base
  if (caracteresCorretos < 0) caracteresCorretos = 0;
  if (caracteresCorretos > SENHA_CORRETA.length()) {
    caracteresCorretos = SENHA_CORRETA.length();
  }
  
  Serial.println("Caracteres corretos estimados: " + String(caracteresCorretos));
  
  if (caracteresCorretos > 0) {
    Serial.print("Prefixo descoberto: ");
    for (int i = 0; i < caracteresCorretos; i++) {
      Serial.print(SENHA_CORRETA[i]);
    }
    Serial.println();
  }
  
  Serial.println("VULNERABILIDADE DETECTADA:");
  Serial.println("- Sistema para no primeiro erro");
  Serial.println("- Timing revela progresso da verificação");
  Serial.println("- Ataque pode descobrir senha digit por digit");
  Serial.println("=====================================\n");
}

void acessoPermitido() {
  lcd.clear();
  lcd.print("ACESSO PERMITIDO");
  lcd.setCursor(0, 1);
  lcd.print("Bem-vindo!");
  
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, LOW);
  
  Serial.println("ACESSO PERMITIDO!");
  
  delay(3000);
  resetarSistema();
}

void acessoNegado() {
  lcd.clear();
  lcd.print("ACESSO NEGADO");
  lcd.setCursor(0, 1);
  lcd.print("Tent.:" + String(tentativasRestantes));
  
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, LOW);
  
  Serial.println("ACESSO NEGADO! Tentativas restantes: " + String(tentativasRestantes));
  
  delay(2000);
  digitalWrite(LED_VERMELHO, LOW);
  mostrarTelaInicial();
}

void sistemaBloquado() {
  lcd.clear();
  lcd.print("SISTEMA BLOQUADO");
  lcd.setCursor(0, 1);
  lcd.print("Muitas tentativas");
  
  // Pisca LED vermelho
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_VERMELHO, HIGH);
    delay(200);
    digitalWrite(LED_VERMELHO, LOW);
    delay(200);
  }
  
  Serial.println("SISTEMA BLOQUEADO POR SEGURANÇA!");
  
  delay(3000);
  resetarSistema();
}

void resetarSistema() {
  tentativasRestantes = MAX_TENTATIVAS;
  senhaDigitada = "";
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  mostrarTelaInicial();
  
  if (modoAnalise) {
    Serial.println("\n>>> Sistema resetado - Pronto para nova análise <<<");
  }
}