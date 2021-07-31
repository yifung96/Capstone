#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial s(D6,D5);

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  s.begin(9600);
  //while (!Serial) continue;
}
 
void loop() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid()){
    //Serial.println(F("Parsing failed!"));
    return;
  }
  Serial.println(F("JSON received and parsed"));
  //root.prettyPrintTo(Serial);
  Serial.println("Data 1 ");
  int data1=root["data1"];
  Serial.print(data1);
  Serial.print("   Data 2 ");
  int data2=root["data2"];
  Serial.print(data2);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
 
}
