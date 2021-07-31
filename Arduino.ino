
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

LiquidCrystal_I2C lcd(0x27,20,4);  
String message = "";
bool messageReady = false;
const int trigPin = 26;
const int echoPin = 27;
long duration;
int distance;
const int moist_pin= A0;
int moist_value;
int moist_pct;
String mssg1;
String mssg2;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  lcd.init();                       
  lcd.init();
  lcd.backlight();
}

void loop() {
  distance = ult();
  delay(1000);
  lcd.clear();
  moist_pct = moisture();
  delay(1000);
  lcd.clear();
  StaticJsonDocument<200> doc;
  doc["Distance"] = distance;
  doc["Moisture%"] = moist_pct;
  serializeJson(doc,Serial);
  Serial.println();
  serializeJsonPretty(doc, Serial);
}

int ult(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance= (duration*0.034/2);
  mssg1=("Water Level cm");
  mssg2=distance;
  display(mssg1, mssg2);
  return distance;
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
void display(String mssg1, String mssg2){
  lcd.setCursor(0,0);
  lcd.print(mssg1);
  lcd.setCursor(0,1);
  lcd.print(mssg2);
}
