#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Ultrasonic.h>
SoftwareSerial s(10,11);

LiquidCrystal_I2C lcd(0x27,20,4); 
Ultrasonic ultrasonic(26, 27); 

const int moist_pin= A0;
const int ec_pin = A1;
const int wpump = 32, pumpind = 28;
const int wlevel=29;
const int runcheck=30;
const int wtank = 31;
int level, moist_value, moist_pct, volume_pct, ec;
String mssg1, mssg2, err;
long duration;
String data, data1, data2, data3, indi;
int waterlevel();
int moisture();
int EC();
void ind();
void waterpump();
void setup()
{
  pinMode(wtank, INPUT); 
  pinMode(wpump, OUTPUT);
  pinMode(pumpind, OUTPUT);
  digitalWrite(wpump, HIGH);
  pinMode(wlevel, OUTPUT);
  pinMode(runcheck, OUTPUT);
  Serial.begin(9600);
  s.begin(9600);
  lcd.init();                       
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcome to ");
  lcd.setCursor(0,1);
  lcd.print("Smart Pot");
  delay(1500);
  lcd.clear();
  }

void loop()
{
  digitalWrite(runcheck, HIGH);
  lcd.print("System Check");
  delay(1500);
  lcd.clear();
  delay(800);
  volume_pct=waterlevel();
  delay(1500);
  lcd.clear();
  moist_pct=moisture();
  delay(1500);
  lcd.clear();
  ec=EC();
  delay(1500);
  lcd.clear();
  ind();
  data1= String(volume_pct);
  data2= String(moist_pct);
  data3= String(ec);
  data=indi+data1+data2+data3;
  Serial.println(data);
  if(s.available()>0){
      s.print(data);
    }
  waterpump();
  delay(1500);
  lcd.clear();
  digitalWrite(runcheck, LOW);
  delay(30000);
}
  
int moisture(){
  moist_value=analogRead(moist_pin);
  moist_pct=map(moist_value, 1024, 0, 0, 100);
  Serial.print("Moisture: ");
  Serial.print(moist_pct);
  Serial.println("%");
  mssg1=("Moisture %");
  mssg2=moist_pct;
  display(mssg1, mssg2);
  return moist_pct;
}

int waterlevel(){
  
  level=ultrasonic.read();
  int maxv=165*7;
  int volume=(level-2)*165;
  volume_pct=(((float)maxv-(float)volume)/(float)maxv)*100;
  if (volume_pct>20 and volume_pct<=100){
    Serial.println(volume_pct);
    digitalWrite(wlevel, LOW);
    mssg2=volume_pct;
  }
  else if ( volume_pct<=20 and volume_pct >=0){
    Serial.println(volume_pct);
    digitalWrite(wlevel,HIGH);
    mssg2=volume_pct;
  }
  else{
    Serial.print(volume_pct);
    Serial.println("ERROR");
    mssg2="Error";
    volume_pct= 101;
  }
  mssg1=("Water Level %");
  display(mssg1, mssg2);
  return volume_pct;
}
void display(String mssg1, String mssg2){
  lcd.setCursor(0,0);
  lcd.print(mssg1);
  lcd.setCursor(0,1);
  lcd.print(mssg2);
}
int EC(){
  int voltage=analogRead(ec_pin);
  ec= voltage*(5.0/1023.0);
  Serial.print("Electrical Conductivity: ");
  Serial.print(ec);
  Serial.println("V");
  mssg1=("EC Test");
  mssg2=ec;
  display(mssg1, mssg2);
  return ec;
}
void waterpump(){
  int val=digitalRead(wtank);
  mssg1="Water Pump";
  if (moist_pct<50){
    if (val==0 or (volume_pct>20 and volume_pct<=100)){
      mssg2="ON";
      display(mssg1, mssg2);
      digitalWrite(pumpind, HIGH);
      digitalWrite(wpump, LOW);
      delay(6000);
      digitalWrite(wpump, HIGH);
      digitalWrite(pumpind, LOW);
      mssg2="OFF";
      display(mssg1, mssg2);
    }
    else{
      mssg2="W.Fail";
      digitalWrite(wpump, HIGH);
      display(mssg1, mssg2);
    }
  }
  else{
    mssg2="OFF";
    digitalWrite(wpump, HIGH);
    display(mssg1, mssg2);
  }
  return;
}
void ind(){
  if (moist_pct==100){
    indi="a";
  }
  else if (moist_pct<100 and moist_pct>=10){
    indi="b";
  }
  else if (moist_pct<10 and moist_pct>=0){
    indi="c";
  }
  else{
    indi="e";
  }
  return;
}
