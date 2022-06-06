/*********
  Rui Santos
  Complete instructions at https://RandomNerdTutorials.com/esp32-web-server-gauges/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>

// Replace with your network credentials
const char* ssid = "Shikari WiFi";
const char* password = "Nsoft2011";

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 184);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char* PARAM_INPUT_1 = "relay";
const char* PARAM_INPUT_2 = "state";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Create a sensor object
Adafruit_BME280 bme; // BME280 connect to ESP32 I2C (GPIO 21 = SDA, GPIO 22 = SCL)

// Init BME280
void initBME(){
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

// Get Sensor Readings and return JSON object
String getSensorReadings(){
  //readings["temperature"] = String(random(40)); //bme.readTemperature()
  //readings["humidity"] =  String(random(100)); //bme.readHumidity()
  
  //readings["relay1"] =  String(random(0,2));
  //readings["relay2"] =  String(random(0,2));
  //readings["relay3"] =  String(random(0,2));
  //readings["relay4"] =  String(random(0,2));
  //readings["relay5"] =  String(random(0,2));
  //readings["relay6"] =  String(random(0,2));
  //readings["relay7"] =  String(random(0,2));
  //readings["relay8"] =  String(random(0,2));
  
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  int attmCnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
    attmCnt++;
    if(attmCnt == 10) {
      WiFi.begin("SDG8", "deltaplan2071");
    }else if(attmCnt == 20)
    {
      WiFi.begin("Programmer-DoICT-Jashore", "4321567890");
    }else if(attmCnt == 30){
      WiFi.begin(ssid, password);
      attmCnt = 0;
    }
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  // initBME();
  initWiFi();
  initSPIFFS();
  
  readings["temperature"] = String(random(40)); //bme.readTemperature()
  readings["humidity"] =  String(random(100)); //bme.readHumidity()
  
  readings["relay1"] =  String(random(0,2));
  readings["relay2"] =  String(random(0,2));
  readings["relay3"] =  String(random(0,2));
  readings["relay4"] =  String(random(0,101));
  readings["relay5"] =  String(random(0,2));
  readings["relay6"] =  String(random(0,2));
  readings["relay7"] =  String(random(0,2));
  readings["relay8"] =  String(random(0,101));
  
  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      readings[inputMessage1] =  inputMessage2;
      // digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("RELAY: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    events.send(getSensorReadings().c_str(),"new_readings", millis());
    request->send(200, "text/plain", "OK");
  });
  
//  server.on("/update-speed", HTTP_GET, [] (AsyncWebServerRequest *request) {
//    String pinNo;
//    String pinValue;
//    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
//    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
//      pinNo = request->getParam(PARAM_INPUT_1)->value();
//      pinValue = request->getParam(PARAM_INPUT_2)->value();
//      readings[pinNo] =  pinValue;
//      // digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
//    }
//    else {
//      pinNo = "No message sent";
//      pinValue = "No message sent";
//    }
//    Serial.print("RELAY: ");
//    Serial.print(inputMessage1);
//    Serial.print(" - Set to: ");
//    Serial.println(inputMessage2);
//    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
//    request->send(200, "text/plain", "OK");
//  });
  server.serveStatic("/", SPIFFS, "/");

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
    lastTime = millis();
  }
}
