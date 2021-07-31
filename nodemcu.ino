#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ThingSpeak.h>
#include <ArduinoJson.h>

char* ssid = "HUAWEI nova 3i";
char* password = "yf19961996";
WiFiClient client;
String api = "WRWU1SJKOGCBA1QI";
unsigned long Channel = 923597;
void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  ThingSpeak.begin(client);
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected");
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  
}

void loop()
{
  // Send a JSON-formatted request with key "type" and value "request"
  // then parse the JSON-formatted response with keys "gas" and "distance"
  DynamicJsonDocument doc(1024);
  double  moist = 0;
  // Sending the request
  doc["type"] = "request";
  serializeJson(doc,Serial);
  // Reading the response
  boolean messageReady = false;
  String message = "";
  while(messageReady == false) { // blocking but that's ok
    if(Serial.available()) {
      message = Serial.readString();
      messageReady = true;
    }
  }
  // Attempt to deserialize the JSON-formatted message
  DeserializationError error = deserializeJson(doc,message);
  if(error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  moist = doc["moist"];
  // Prepare the data for serving it over HTTP
  String output = "distance: " + String(moist) + "\n";
  // Serve the data as plain text, for example
  ThingSpeak.writeField(Channel,1,moist,api);
  delay(10000);
}
