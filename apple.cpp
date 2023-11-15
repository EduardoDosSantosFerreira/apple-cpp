
/* ******************** Sistema de irrigação (MACIEIRA) ********************
   Criado por: Eduardo S Ferreira

   Guia de conexão:
   LCD RS: pino 12
   LCD Enable: pino 11
   LCD D4: pino 5o programa
   LCD D5: pino 4
   LCD D6: pino 3
   LCD D7: pino 2
   LCD R/W: GND
   LCD VSS: GND
   LCD VCC: VCC (5V)
   Potenciômetro de 10K terminal 1: GND
   Potenciômetro de 10K terminal 2: V0 do LCD (Contraste)
   Potenciômetro de 10K terminal 3: VCC (5V)
   Sensor de umidade do solo A0: Pino A0
   Sensor de temperatura A1: Pino A1
   Led Azul: Pino 7
   Led Vermelho: Pino 6
   Módulo Relé (Válvula): Pino 10

      
 ***************************************************************************** */

// inclui a biblioteca:
#include <LiquidCrystal.h>

// define os pinos de conexão entre o Arduino e o Display LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// variáveis do programa
const int pinoSensor = A0;
const int pinoValvula = 10;
const int limiarSeco = 60;
const int tempoRega = 5; // Tempo de rega em segundos
int umidadeSolo = 0;
int SensorTempPino=1;
int AlertaTempBaixa=7; // Alerta de temperatura baixa
int AlertaTempAlta=6; // Alerta de temperatura alta
int TempBaixa=23; // Define temperatura baixa como abaixo de vinte e três graus Celsius
int TempAlta=32; // Define temperatura alta como acima de trinta e dois graus Celsius

void setup() {
  
  pinMode(pinoValvula, OUTPUT);
  // Desliga a válvula
  digitalWrite(pinoValvula, HIGH);
  // Liga a válvula
  pinMode(AlertaTempBaixa, OUTPUT);
  // Define o pino de alerta de temperatura baixa como saida
  pinMode(AlertaTempAlta, OUTPUT);
  // Define o pino de alerta de temperatura alta como saida
  lcd.begin(16, 2);
  // Exibe a mensagem no Display LCD.
  lcd.print("    BEM-VINDO ");
  // Frequencia configurada para a comunicação com outros dispositivos
  Serial.begin(9600);
  delay(4000);
  lcd.clear();
}

void loop() {
    
   delay(1500);

	lcd.setCursor(0,0);
    lcd.print("Temp. Ambiente:");
    delay(1500);
    lcd.setCursor(0,1);
    lcd.print("      C        F");
// Faz a leitura da tensao no Sensor de Temperatura
	int SensorTempTensao=analogRead(SensorTempPino);

  	// Converte a tensao lida
	float Tensao=SensorTempTensao*5;
	Tensao/=1024;

  	// Converte a tensao lida em Graus Celsius
	float TemperaturaC=(Tensao-0.5)*100;

  	// Converte a temperatura em Graus Celsius para Fahrenheit
	float TemperaturaF=(TemperaturaC*9/5)+32;

  	// Muda o cursor para a primeira coluna e segunda linha do LCD
    lcd.setCursor(0,1);
      	// Imprime a temperatura em Graus Celsius
	lcd.print(TemperaturaC);
    
  	// Muda o cursor para a decima coluna e segunda linha do LCD
	lcd.setCursor(9,1);

  	// Imprime a temperatura em Graus Fahrenheit
  	lcd.print(TemperaturaF);

	// Acende ou apaga os alertas luminosos de temperatura baixa e alta
  	if (TemperaturaC>=TempAlta) {
  		digitalWrite(AlertaTempBaixa, LOW);
  		digitalWrite(AlertaTempAlta, HIGH);
    }
  	else if (TemperaturaC<=TempBaixa){
  		digitalWrite(AlertaTempBaixa, HIGH);
  		digitalWrite(AlertaTempAlta, LOW);
  	}
  	else {
  		digitalWrite(AlertaTempBaixa, LOW);
  		digitalWrite(AlertaTempAlta, LOW);
    }

  	// Aguarda 4 segundos
  	delay(4000);
   lcd.clear();


 lcd.print("ESTATISTICAS:");
    lcd.setCursor(0,1);
    lcd.print("MACIEIRA");
    delay(2500);
        lcd.clear();
    delay(1500);
    lcd.print("UMID.DO SOLO EM:");
    
    delay(1500);
  // Mede a umidade a cada segundo. Faz isso durante uma hora (3600 segundos).
  for(int i=0; i < 5; i++) {
    // Posiciona o cursor do LCD na coluna 0 linha 1
    // (Obs: linha 1 é a segunda linha, a contagem começa em 0
    lcd.setCursor(0, 1);
    // Faz a leitura do sensor de umidade do solo
    umidadeSolo = analogRead(pinoSensor);
    // Converte a variação do sensor de 0 a 1023 para 0 a 100
    umidadeSolo = map(umidadeSolo, 1023, 0, 0, 100);
    // Exibe a mensagem no Display LCD:
    lcd.print(umidadeSolo);
    lcd.print("%");
    // Espera um segundo
    delay(1000);
   
  }

     if(umidadeSolo <= limiarSeco) {
    // Posiciona o cursor do LCD na coluna 0 linha 1
    // (Obs: linha 1 é a segunda linha, a contagem começa em 0
    lcd.setCursor(0, 1);
        // Exibe a mensagem no Display LCD:
    lcd.print("    REGANDO     ");
    // Liga a válvula
    digitalWrite(pinoValvula, LOW);
    // Espera o tempo estipulado
    delay(tempoRega*1000);
    digitalWrite(pinoValvula, HIGH);
    lcd.clear();
  }
   
   else if (umidadeSolo > 75) {
    lcd.clear();
    lcd.print("SOLO ENCHARCADO");
    delay(2000);
    lcd.clear();
    lcd.print("REALIZE UMA");
    lcd.setCursor(0,1);
    lcd.print("DRENAGEM!");
    delay(2000);
    lcd.clear();
    delay(1500); 
  } 
    
    else {
    // Posiciona o cursor do LCD na coluna 0 linha 1
    // (Obs: linha 1 é a segunda linha, a contagem começa em 0
    lcd.setCursor(0, 1);
    lcd.clear();
    // Exibe a mensagem no Display LCD:
    lcd.print("SOLO SACIADO!");
    // Espera o tempo estipulado
    delay(3000);
    // Limpa as informações do visor
    lcd.clear();
    
    
  }
}