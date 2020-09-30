#include <LiquidCrystal_I2C.h>
#include <Wire.h>
int LRojo= 2;
int LBlanco=3;
int LVerde=4;
int BRojo=5;
int BAzul=6;
int BBlanco=7;
int Bomba=9;
int ON=1;
int ALCD=0;
float Q=0.05; //litros por segundo
float VRestante=20.0; //LITROS ESTANQUE
float TRestante=0.01; //HORAS CADA RIEGO
float TRMin=0.0;
float TRSeg=0.0;
LiquidCrystal_I2C lcd (0x27,16,2);
void setup() {
  // put your setup code here, to run once:
  pinMode(LRojo,OUTPUT);
  pinMode(LBlanco,OUTPUT);
  pinMode(LVerde,OUTPUT);
  pinMode(BRojo,INPUT);
  pinMode(BAzul,INPUT);
  pinMode(BBlanco,INPUT);
  pinMode(Bomba,OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(16,2);
  lcd.backlight();
  lcd.clear();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  while (ON==1){
    //Printear Valores
      digitalWrite(LBlanco,LOW);
      digitalWrite(Bomba,HIGH);
      ALCD=ALCD+1;
      if (ALCD==50){
        ALCD=0;
          lcd.clear();
        }
    if (digitalRead(BBlanco)==1){
      VRestante=20.0; //Carga de estanque
      }
    if (VRestante<0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Estanque Vacio");
      break;
      }
    digitalWrite(LRojo,LOW);
    digitalWrite(LVerde,HIGH);
    float val=analogRead(3);
    float VRiego=(val*5/1024);
    Serial.println(VRiego);
    lcd.setCursor(0,0);
    lcd.print("VR:");
    lcd.print(VRestante);
    lcd.print(" ");
    lcd.print("R:");
    lcd.print(VRiego);
    lcd.setCursor(0,1);
    lcd.print("TR=");
    if (int(TRestante)<10){
      lcd.print("0");
      lcd.print(int(TRestante));
      }
    else if (TRestante>=10){
      lcd.print(int(TRestante));
      }
    lcd.print(":");
    if (int(TRMin)<10){
      lcd.print("0");
      lcd.print(int(TRMin));
      }
    else if (TRMin>=10){
      lcd.print(int(TRMin));
      }
    lcd.print(":");
    if (int(TRSeg)<10){
      lcd.print("0");
      lcd.print(int(TRSeg));
      lcd.print("    ");
      }
    else if (TRSeg>=10){
      lcd.print(int(TRSeg));
      lcd.print("    ");
      }
    if (digitalRead(BAzul)==1){
      TRestante=TRestante-8.0;
      delay(500);
      }
    if (digitalRead(BRojo)==1){
      ON=!ON;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Desactivado");
      digitalWrite(LRojo,HIGH);
      digitalWrite(LVerde,LOW);
      delay(1000);
      break;
      }
    if (digitalRead(BBlanco)==1){
      break;
      }
      //FIN
    TRestante=TRestante-(0.2/60)/60;
    TRMin= (TRestante-int(TRestante))*60;
    TRSeg= (TRMin-int(TRMin))*60;
    delay(150);

    if (TRestante<0){
      Riego();
      }
    }
  if (digitalRead(BRojo)==1){
    ON=!ON;
    delay(1000);
    }

  digitalWrite(Bomba,HIGH);
  digitalWrite(LBlanco,LOW);

  while (VRestante<0){
    digitalWrite(LRojo,HIGH);
    digitalWrite(LBlanco,HIGH);
    digitalWrite(LVerde,LOW);
    digitalWrite(Bomba,HIGH);
    if (digitalRead(BBlanco)==1){
      delay(1000);
      VRestante=20.0;
      lcd.clear();
      break;
      }
    }




}
void Riego(){
  float val=analogRead(3);
  float TRiego=(val*5)/(1024*Q); //en Segundos
  digitalWrite(LBlanco,HIGH);
  digitalWrite(Bomba,LOW);
  delay(TRiego*1000);
  VRestante=VRestante-Q*(TRiego);
  digitalWrite(Bomba,HIGH);
  digitalWrite(LBlanco,LOW);
  TRestante=30.0;
  delay(200);
  }
