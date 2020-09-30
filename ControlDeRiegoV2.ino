#include <Wire.h>
#include <LiquidCrystal_I2C.h>
unsigned long t = 0;
int tiempo2=0;


float Q=0.05;
//Leds
#define L_RED 2
#define L_WHITE 3
#define L_GREEN 4

//Botones

#define BTN_RED 5
#define BTN_BLUE 6
#define BTN_WHITE 7
#define PUMP 9

uint8_t button_estate[3];

//Estado
#define S_PRINT  0
#define S_TRIEGO 1
#define S_VRIEGO 2
#define S_RESETE 3
#define S_IDLE 4
uint8_t estado = S_PRINT;

int TRiego=30;
float VRiego=1.0;
int VE=100;
float TLEFT=TRiego;
LiquidCrystal_I2C lcd (0x27,16,2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  lcd.begin(16,2);
  //t=millis();
  lcd.backlight();
pinMode(BTN_RED,INPUT_PULLUP);
pinMode(BTN_BLUE,INPUT_PULLUP);
pinMode(BTN_WHITE,INPUT_PULLUP);
pinMode(PUMP,OUTPUT);
pinMode(L_RED,OUTPUT);
pinMode(L_GREEN,OUTPUT);
pinMode(L_WHITE,OUTPUT);

//Estado inicial botones
  button_estate[0] = HIGH;
  button_estate[1] = HIGH;
  button_estate[2] = HIGH;

  printValores();
}
void printValores(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TR,VR,VE,TLEFT");
  lcd.setCursor(0,1);
  lcd.print(TRiego, DEC);
  lcd.print(" ");
  lcd.print(int(VRiego), DEC);
  lcd.print(" ");
  lcd.print(VE, DEC);
  lcd.print(" ");
  lcd.print(TLEFT);
  delay(200);
}

uint8_t flancoSubida(int btn) {
  uint8_t valor_nuevo = digitalRead(btn);
  uint8_t result = button_estate[btn]!=valor_nuevo && valor_nuevo == 1;
  button_estate[btn] = valor_nuevo;
  return result;
}

void Regar(){
  float Tregar=1000*VRiego*Q;
  int Tregarint= Tregar;
  digitalWrite(PUMP,1);
  delay(Tregarint);
  digitalWrite(PUMP,0);
  }

void printTRIEGO(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Regar Cada:");
  lcd.setCursor(0,1);
  lcd.print(TRiego);
  delay(200);
  }
void printVRIEGO(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Regar :");
  lcd.setCursor(0,1);
  lcd.print(VRiego);
  lcd.print(" Litros");
  delay(200);
  }
void loop() {
  switch(estado){
    //int Riegos= t/(3600000*TRiego);
    //float TLEFT=TRiego-t/3600000 + Riegos*TRiego;
  
    case S_PRINT:
      tiempo2+=1;
      printValores();
      if (flancoSubida(BTN_WHITE)){
        estado=S_TRIEGO;
        break;
        }
      if (tiempo2>20000){
        tiempo2=0;
        estado=S_IDLE;
        break;
        }
      break;

      
    case S_TRIEGO:
      printTRIEGO();
      if (flancoSubida(BTN_WHITE)){
        estado=S_VRIEGO;
        }
      if (flancoSubida(BTN_RED)){
        TRiego-=1;
        }
      if (flancoSubida(BTN_BLUE)){
        TRiego+=1;
        }
      break;

      
    case S_VRIEGO:
      printVRIEGO();
      if (flancoSubida(BTN_WHITE)){
        estado=S_RESETE;
        break;
        }
      if (flancoSubida(BTN_RED)){
        VRiego-=0.1;
        break;
        }
      if (flancoSubida(BTN_BLUE)){
        VRiego+=0.1;
        break;
        }
      break;

      
    case S_RESETE:
      if (flancoSubida(BTN_WHITE)){
        VE=100;
        estado=S_PRINT;
        tiempo2=0;
        break;
        }
      
      break;

      
    case S_IDLE:
      lcd.noBacklight();
      digitalWrite(L_WHITE,LOW);
      digitalWrite(L_RED,LOW);
      digitalWrite(L_GREEN,HIGH);
      if(TLEFT<0){
        digitalWrite(L_WHITE,HIGH);
        Regar();
        digitalWrite(L_WHITE,LOW);
        VE-=VRiego;
        break;
        }
      if (flancoSubida(BTN_WHITE)){
        estado=S_PRINT;
        tiempo2=0;
        digitalWrite(L_RED,HIGH);
        digitalWrite(L_WHITE,HIGH);
        break;
        }
      
      break;
    
    
    
    
    }
  // put your main code here, to run repeatedly:

}
