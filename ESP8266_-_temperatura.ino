ADC_MODE(ADC_VCC);
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char *ssid = "MAX";
const char *password = "2473439656";

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

ESP8266WebServer server(80);

void handleRoot() {
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  sensors.requestTemperatures();
  float temperatura = (sensors.getTempCByIndex(0));
  int temp1 = temperatura;
  int temp2 = (temperatura - temp1) * 100;
  int nap1 = ESP.getVcc() / 1000;
  int nap2 = ESP.getVcc() - nap1*1000;
  
  snprintf(temp, 400,

"{\"Napięcie\" : %d.%d ,\
\"Czas\" : \"%02d:%02d:%02d\",\
 \"Temperatura\": \"d.%02d*C\"}",
    nap1, nap2, hr, min % 60, sec % 60, temp1, temp2
  );
  server.send ( 200, "application/json", temp );
}

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);     // kontrola diody
  WiFi.begin ( ssid,password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
  }

  server.on("/", handleRoot);
  server.begin();
  sensors.begin();
}

void loop(){
  server.handleClient();
  digitalWrite(LED_BUILTIN, LOW);// dioda wyłączona
}
