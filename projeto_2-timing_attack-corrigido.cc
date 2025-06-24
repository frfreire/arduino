/*
====================================================================
TIMING ATTACK DEMONSTRATION - VERSÃO COMPLETA
====================================================================
Este código demonstra:
1. Sistema vulnerável a timing attacks
2. Sistema corrigido com timing constante
3. Comparação entre ambos os métodos

Funcionalidades:
- Tecla 'A': Alterna entre modo vulnerável/seguro
- Tecla 'B': Ativa/desativa análise de timing
- Tecla 'C': Mostra informações do sistema
- Tecla 'D': Modo demonstração automática
- Tecla '*': Reset
- Tecla '#': Confirma senha

Componentes necessários:
- Arduino Uno
- Keypad 4x4 
- LCD 16x2
- LED Verde (Pino 12)
- LED Vermelho (Pino 13)
- Resistores 220Ω

Montagem:
- Keypad: Pinos 2-9
- LCD: Pinos A0-A5  
- LEDs: Pinos 12 e 13
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

// Configuração dos componentes
const int LED_VERDE = 12;
const int LED_VERMELHO = 13;

// Configuração do sistema
const String SENHA_CORRETA = "1234";
String senhaDigitada = "";
const int MAX_TENTATIVAS = 3;
int tentativasRestantes = MAX_TENTATIVAS;

// Configuração dos modos
bool modoVulneravel = true;  // true = vulnerável, false = seguro
bool modoAnalise = false;    // true = mostra timing no serial
bool modoDemo = false;       // true = demonstração automática

// Variáveis de timing
unsigned long tempoInicio = 0;
unsigned long tempoFim = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  
  mostrarTelaInicial();
  mostrarInstrucoes();
}

void loop() {
  char tecla = keypad.getKey();
  
  if (tecla) {
    processarTecla(tecla);
  }
  
  // Modo demonstração automática
  if (modoDemo) {
    executarDemonstracao();
  }
}

void processarTecla(char tecla) {
  switch (tecla) {
    case 'A':
      alternarModoSeguranca();
      break;
    case 'B':
      alternarModoAnalise();
      break;
    case 'C':
      mostrarInformacoesSistema();
      break;
    case 'D':
      alternarModoDemo();
      break;
    case '*':
      resetarSistema();
      break;
    case '#':
      if (senhaDigitada.length() > 0) {
        verificarSenha();
      }
      break;
    default:
      if (tecla >= '0' && tecla <= '9') {
        adicionarDigito(tecla);
      }
      break;
  }
}

void mostrarInstrucoes() {
  Serial.println("====================================================================");
  Serial.println("SISTEMA DE DEMONSTRACAO - TIMING ATTACKS");
  Serial.println("====================================================================");
  Serial.println("CONTROLES:");
  Serial.println("A - Alternar modo (Vulneravel/Seguro)");
  Serial.println("B - Ativar/Desativar analise de timing");
  Serial.println("C - Mostrar informacoes do sistema");
  Serial.println("D - Modo demonstracao automatica");
  Serial.println("* - Reset do sistema");
  Serial.println("# - Confirmar senha");
  Serial.println("0-9 - Digitar senha");
  Serial.println("====================================================================");
  Serial.println("Senha correta: " + SENHA_CORRETA);
  Serial.println("Modo atual: " + String(modoVulneravel ? "VULNERAVEL" : "SEGURO"));
  Serial.println("====================================================================");
}

void mostrarTelaInicial() {
  lcd.clear();
  lcd.print("SISTEMA ");
  lcd.print(modoVulneravel ? "VULN" : "SEGURO");
  lcd.setCursor(0, 1);
  lcd.print("Digite senha:");
  senhaDigitada = "";
  apagarLEDs();
}

void alternarModoSeguranca() {
  modoVulneravel = !modoVulneravel;
  
  lcd.clear();
  lcd.print("MODO: ");
  if (modoVulneravel) {
    lcd.print("VULNERAVEL");
    Serial.println("\n>>> MODO VULNERAVEL ATIVADO <<<");
    Serial.println("Sistema para no primeiro erro - timing variavel");
  } else {
    lcd.print("SEGURO");
    Serial.println("\n>>> MODO SEGURO ATIVADO <<<");
    Serial.println("Sistema sempre verifica toda senha - timing constante");
  }
  
  delay(2000);
  mostrarTelaInicial();
}

void alternarModoAnalise() {
  modoAnalise = !modoAnalise;
  
  lcd.clear();
  lcd.print("ANALISE: ");
  lcd.print(modoAnalise ? "ON" : "OFF");
  lcd.setCursor(0, 1);
  lcd.print("Timing no serial");
  
  if (modoAnalise) {
    Serial.println("\n>>> MODO ANALISE ATIVADO <<<");
    Serial.println("Timing sera exibido no Serial Monitor");
  } else {
    Serial.println("\n>>> MODO ANALISE DESATIVADO <<<");
  }
  
  delay(1500);
  mostrarTelaInicial();
}

void mostrarInformacoesSistema() {
  Serial.println("\n====== INFORMACOES DO SISTEMA ======");
  Serial.println("Modo de seguranca: " + String(modoVulneravel ? "VULNERAVEL" : "SEGURO"));
  Serial.println("Analise de timing: " + String(modoAnalise ? "ATIVADA" : "DESATIVADA"));
  Serial.println("Senha correta: " + SENHA_CORRETA);
  Serial.println("Tentativas restantes: " + String(tentativasRestantes));
  Serial.println("Senha atual: " + senhaDigitada);
  Serial.println("=====================================");
}

void alternarModoDemo() {
  modoDemo = !modoDemo;
  
  lcd.clear();
  if (modoDemo) {
    lcd.print("MODO DEMO ON");
    lcd.setCursor(0, 1);
    lcd.print("Aguarde...");
    Serial.println("\n>>> MODO DEMONSTRACAO ATIVADO <<<");
    Serial.println("Executando testes automaticos...");
  } else {
    lcd.print("MODO DEMO OFF");
    Serial.println("\n>>> MODO DEMONSTRACAO DESATIVADO <<<");
  }
  
  delay(1500);
  mostrarTelaInicial();
}

void adicionarDigito(char digito) {
  if (senhaDigitada.length() < 8) {
    senhaDigitada += digito;
    atualizarDisplay();
  }
}

void atualizarDisplay() {
  lcd.clear();
  lcd.print("Senha:");
  lcd.setCursor(0, 1);
  
  for (int i = 0; i < senhaDigitada.length(); i++) {
    lcd.print("*");
  }
  lcd.print(" (# p/ OK)");
}

void verificarSenha() {
  tempoInicio = millis();
  
  if (modoAnalise) {
    Serial.println("\n--- ANALISE DE TIMING ---");
    Serial.println("Modo: " + String(modoVulneravel ? "VULNERAVEL" : "SEGURO"));
    Serial.println("Senha digitada: " + senhaDigitada);
    Serial.println("Senha correta:  " + SENHA_CORRETA);
    Serial.print("Verificando... ");
  }
  
  bool senhaCorreta;
  if (modoVulneravel) {
    senhaCorreta = verificarSenhaVulneravel(senhaDigitada);
  } else {
    senhaCorreta = verificarSenhaSegura(senhaDigitada);
  }
  
  tempoFim = millis();
  unsigned long tempoDecorrido = tempoFim - tempoInicio;
  
  if (modoAnalise) {
    Serial.println("Concluido!");
    Serial.println("Tempo decorrido: " + String(tempoDecorrido) + "ms");
    analisarResultado(tempoDecorrido);
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

// IMPLEMENTACAO VULNERAVEL - Para no primeiro erro
bool verificarSenhaVulneravel(String senha) {
  if (senha.length() != SENHA_CORRETA.length()) {
    delay(50);
    return false;
  }
  
  // VULNERABILIDADE: Para imediatamente no primeiro erro
  for (int i = 0; i < senha.length(); i++) {
    delay(100); // Simula processamento por caractere
    
    if (senha[i] != SENHA_CORRETA[i]) {
      // FALHA: Retorna imediatamente
      return false;
    }
  }
  
  delay(100); // Processamento final
  return true;
}

// IMPLEMENTACAO SEGURA - Timing constante
bool verificarSenhaSegura(String senha) {
  bool resultado = true;
  int maxLength = max(senha.length(), SENHA_CORRETA.length());
  
  // CORRECAO: Sempre verifica todas as posicoes
  for (int i = 0; i < maxLength; i++) {
    delay(100); // Tempo constante por posicao
    
    // Verifica se indices sao validos
    if (i >= senha.length() || i >= SENHA_CORRETA.length()) {
      resultado = false;
      // NAO retorna aqui - continua verificando
    } else if (senha[i] != SENHA_CORRETA[i]) {
      resultado = false;
      // NAO retorna aqui - continua verificando
    }
  }
  
  // Adiciona delay adicional para mascarar variacoes
  delay(100);
  
  return resultado;
}

void analisarResultado(unsigned long tempo) {
  Serial.println("--- ANALISE DA VULNERABILIDADE ---");
  
  if (modoVulneravel) {
    // Estima caracteres corretos baseado no timing
    int caracteresCorretos = (tempo - 150) / 100;
    if (caracteresCorretos < 0) caracteresCorretos = 0;
    if (caracteresCorretos > SENHA_CORRETA.length()) {
      caracteresCorretos = SENHA_CORRETA.length();
    }
    
    Serial.println("Caracteres corretos estimados: " + String(caracteresCorretos));
    
    if (caracteresCorretos > 0 && caracteresCorretos < SENHA_CORRETA.length()) {
      Serial.print("Prefixo descoberto: ");
      for (int i = 0; i < caracteresCorretos; i++) {
        Serial.print(SENHA_CORRETA[i]);
      }
      Serial.println();
      
      Serial.println("VULNERABILIDADE DETECTADA:");
      Serial.println("- Timing varia com numero de caracteres corretos");
      Serial.println("- Atacante pode descobrir senha digito por digito");
      Serial.println("- Cada tentativa revela informacao adicional");
    }
  } else {
    Serial.println("Sistema seguro - timing constante");
    Serial.println("- Tempo nao varia com entrada");
    Serial.println("- Nenhuma informacao vazada");
    Serial.println("- Resistente a timing attacks");
  }
  
  Serial.println("=====================================");
}

void executarDemonstracao() {
  String senhasTesteVuln[] = {"0000", "1000", "1200", "1230", "1234"};
  String senhasTesteSeguro[] = {"0000", "5555", "9999", "1234"};
  
  Serial.println("\n=== DEMONSTRACAO AUTOMATICA ===");
  
  // Teste modo vulneravel
  if (modoVulneravel) {
    Serial.println("Testando modo VULNERAVEL:");
    for (int i = 0; i < 5; i++) {
      senhaDigitada = senhasTesteVuln[i];
      Serial.print("Testando: " + senhasTesteVuln[i] + " -> ");
      verificarSenha();
      delay(2000);
      resetarSistema();
    }
  } else {
    Serial.println("Testando modo SEGURO:");
    for (int i = 0; i < 4; i++) {
      senhaDigitada = senhasTesteSeguro[i];
      Serial.print("Testando: " + senhasTesteSeguro[i] + " -> ");
      verificarSenha();
      delay(2000);
      resetarSistema();
    }
  }
  
  modoDemo = false;
  Serial.println("=== DEMONSTRACAO CONCLUIDA ===");
  mostrarTelaInicial();
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
  lcd.print("Tent.: " + String(tentativasRestantes));
  
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, LOW);
  
  Serial.println("ACESSO NEGADO! Tentativas restantes: " + String(tentativasRestantes));
  
  delay(2000);
  apagarLEDs();
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
  
  Serial.println("SISTEMA BLOQUEADO POR SEGURANCA!");
  
  delay(3000);
  resetarSistema();
}

void resetarSistema() {
  tentativasRestantes = MAX_TENTATIVAS;
  senhaDigitada = "";
  apagarLEDs();
  mostrarTelaInicial();
  
  if (modoAnalise) {
    Serial.println("Sistema resetado - Pronto para nova analise");
  }
}

void apagarLEDs() {
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
}