#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <SoftwareSerial.h>
#include <BlynkSimpleEsp8266.h>
SoftwareSerial s(D6,D5);

BlynkTimer timer;
char auth[] = "xwnXIDsrkm2PrtiBzHDXM2cxds5v4y_e";
const char* ssid = "HUAWEI nova 3i";
const char* password = "yf19961996";
WiFiClient client;
unsigned long Channel = 923597;
const char* WAPIKey = "WRWU1SJKOGCBA1QI";

String d1,d2,d3,d4;
int n;
String data;
int v_pct, moist_pct, ec;

void setup() {
  s.begin(9600);
  Serial.begin(9600);
  delay(10);
  //WiFi.begin(ssid,password);
  Blynk.begin( auth, ssid, password);
  ThingSpeak.begin(client);
  timer.setInterval(1000L,sendsensor1);
  timer.setInterval(1000L,sendsensor2);
  timer.setInterval(1000L,sendsensor3);
}
void loop(){
  s.write("s");
  if (s.available()==0){
    Blynk.run();
    timer.run();
  }
  if (s.available()>0)
    {
      data=s.readString();
      Serial.println(data);
      n = data.length();
      d1=data.substring(0,1);
      d2=data.substring(n-1,n);
      ec=d2.toInt();
      if(d1=="a"){
        moist_pct=100;
        d4=data.substring(1, n-4);
        v_pct=d4.toInt();
      }
      else if (d1=="b"){
        d3=data.substring(n-3, n-1);
        moist_pct=d3.toInt();
        d4=data.substring(1, n-3);
        v_pct=d4.toInt();
      }
      else if(d1=="c"){
        d3=data.substring(n-2, n-1);
        moist_pct=d3.toInt();
        d4=data.substring(1, n-2);
        v_pct=d4.toInt();
      }
     }
   Serial.print(v_pct);
   Serial.print(moist_pct);
   Serial.println(ec);
   //ThingSpeak.setField(3,moist_pct);
   //ThingSpeak.setField(2,ec);
   //ThingSpeak.writeFields(Channel,WAPIKey);
 }
void sendsensor1(){
  int sdata= ec;
  Blynk.virtualWrite(V2, sdata);  
}
void sendsensor2(){
  int sdata= moist_pct;
  Blynk.virtualWrite(V3, sdata);  
}
void sendsensor3(){
  int sdata= v_pct;
  Blynk.virtualWrite(V4, sdata);}
  
