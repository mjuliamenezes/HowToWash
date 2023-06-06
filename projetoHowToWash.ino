#include <Ultrasonic.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Ultrasonic ultrasonic(5, 6);
const int ledPin = 13;
int distance;
bool procedimento = false;
const long intervalo = 6000;
const long intervalo_final = 10000;
String mensagens[10][2] = {
  {"Molhe as maos", " "}, 
  {"Aplique o sabao", " "}, 
  {"Esfregue a palma", "das maos"}, 
  {"Esfregue as", "costas das maos"},
  {"Esfregue entre", "os dedos"}, 
  {"Esfregue as", "costas dos dedos"}, 
  {"Esfregue os", "polegares"}, 
  {"Lave as pontas", "dos dedos"}, 
  {"Enxague as maos", " "}, 
  {"PARABENS!", "LAVAGEM COMPLETA"}
};
int instrucao = 0;
unsigned long tempoAtual = 0, tempoAnterior = 0;
int buzzer = 10;
LiquidCrystal_I2C lcd(0x3F,16,2);

void setup() {
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT);
  lcd.init();
  lcd.setBacklight(HIGH);
  pinMode(ledPin, OUTPUT);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HOW TO WASH?");
}
void barulho() {
    tone(buzzer,261);    
    delay(200);
    noTone(buzzer); 
    tone(buzzer,293);             
    delay(200);    
    noTone(buzzer); 
    tone(buzzer,329);      
    delay(200);
    noTone(buzzer);     
    tone(buzzer,349);    
    delay(200);    
    noTone(buzzer); 
    tone(buzzer,392);            
    delay(200);
    noTone(buzzer); 
}

void mostrarMensagem(int i) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(mensagens[i][0]);
  lcd.setCursor(0,1);
  lcd.print(mensagens[i][1]);

  if (i == 0 || i == 8) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

}

void loop(){
  distance = ultrasonic.read();
  while (distance <= 30) {
    procedimento = true;
    tempoAtual = millis();
    if(tempoAtual - tempoAnterior >= intervalo){
      tempoAnterior = tempoAtual;
      mostrarMensagem(instrucao);
      if (instrucao <= 9){
        instrucao++;
        if (instrucao > 9){
          instrucao=0;
          delay(10000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("HOW TO WASH?");
        }
      }
    }
    distance = ultrasonic.read();
  }
  if (distance=357){
    delay(1000);
    distance = ultrasonic.read();
  }
  if (procedimento == true && distance > 30 && distance < 357){
    distance = ultrasonic.read();
    Serial.println(distance);
    digitalWrite(ledPin, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("LAVAGEM");
    lcd.setCursor(0,1);
    lcd.print("INCOMPLETA!");
    barulho();
    distance = ultrasonic.read();

    tempoAtual = millis();
    if (tempoAtual - tempoAnterior >= intervalo_final){
      procedimento = false;
      instrucao = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("HOW TO WASH?");
    }
  }
}