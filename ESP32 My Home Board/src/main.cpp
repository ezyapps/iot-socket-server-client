
// #include <Arduino.h>
#include <WiFi.h>
// #include <WiFiMulti.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsClient.h> // WebSocket Client Library for WebSocket
#include "BluetoothSerial.h"

#include "SPIFFS.h"
// #include <Arduino_JSON.h>
#include <Adafruit_BME280.h>
// #include <Adafruit_Sensor.h>
#include <ArduinoJson.h> // Arduino JSON Library

// #include "sharedvariables.cpp"

#define RELAY1 2 
#define RELAY2 4
#define RELAY3 5
#define RELAY4 12
#define RELAY5 13
#define RELAY6 14
#define RELAY7 15
#define RELAY8 17
#define RELAY9 18
#define RELAY10 19
#define RELAY11 21
#define RELAY12 22

#define LDR 34
#define IRPin 35
#define DTH11 16

#define SW1 24
#define SW2 25
#define SW3 26
#define SW4 27
#define SW5 32
#define SW6 33
#define SW7 36
#define SW8 39

const char* ssid = "Shikari WiFi";
const char* password = "Nsoft2011";

const char* ap_ssid= "ESP32-Access-Point";
const char* ap_password = "123456789";

const String deviceid = "ksdfjdsfksdfhkjasdfhiwr2343hkl";
const String securitykey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6IjFlOTRmN2ZiLWY4ZDctNDUzZi1iMzNhLWZiYzZlMzU5YWMxYSIsImlhdCI6MTY1NTU2ODk1MH0.n64PsHeBkF25RBoYVlwY8jniTS1W_ZEj9rRD97T2KQ0";
const String bluetoothName = "ESP32 BlueTooth";
String btMessage = "";

// WiFiMulti wifiMulti;
// using namespace websockets;

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

WebSocketsClient wsClient; // websocket client class instance

//StaticJsonDocument<100> doc;
DynamicJsonDocument doc(1024), msg(1024);

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const char* PARAM_INPUT_1 = "variable";
const char* PARAM_INPUT_2 = "value";

AsyncWebServer server(80);
AsyncEventSource events("/events");


// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

bool MANUAL_SWITCH_MODE = false;
bool BLUETOOTH_MODE = false;


int inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp9, inp10;

Adafruit_BME280 bme; // BME280 connect to ESP32 I2C (GPIO 21 = SDA, GPIO 22 = SCL)

String getSensorReadings() {
  
  doc["temperature"] = String(random(40)); //bme.readTemperature()
  doc["humidity"] =  String(random(100)); //bme.readHumidity()
  doc["manual_sw_mode"] = (MANUAL_SWITCH_MODE)? "1":"0";
  doc["bt_sw_mode"] = (BLUETOOTH_MODE)? "1":"0";

  String jsonString = "";
  serializeJson(doc, jsonString);
  return jsonString;
}
void savePinStatus(){
  File file = SPIFFS.open("/pinstatus.txt", FILE_WRITE);
 
  if(!file){
    Serial.println("There was an error opening the file for writing");
    return;
  }  
  if(file.print(getSensorReadings())){
    Serial.println("PIN Status was written.");
  } else {
    Serial.println("Failed to write PIN Statuses.");
  }  
  file.close();
}

// Init BME280
void initBME() {
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

void applyChangesToPins(int delaytime) {
  int var1 = isnan(doc["var1"])? LOW : doc["var1"];
  if(var1 == digitalRead(RELAY1)) {
    if(var1 == LOW) digitalWrite(RELAY1, HIGH); else digitalWrite(RELAY1, LOW);
      delay(delaytime);
  }
  
  int var2 = isnan(doc["var2"])? LOW : doc["var2"];
  if(var2 == digitalRead(RELAY2)) {
    if(var2 == LOW) digitalWrite(RELAY2, HIGH); else  digitalWrite(RELAY2, LOW);
    delay(delaytime);
  }
  int var3 = isnan(doc["var3"])? LOW : doc["var3"];
  if(var3 == digitalRead(RELAY3)) {
    if(var3 == LOW) digitalWrite(RELAY3, HIGH); else digitalWrite(RELAY3, LOW);
    delay(delaytime);
  }
  int var4 = isnan(doc["var4"])? LOW : doc["var4"];
  if(var4 == digitalRead(RELAY4)) {
    if(var4 == LOW) digitalWrite(RELAY4, HIGH); else digitalWrite(RELAY4, LOW);
    delay(delaytime);
  }
  int var5 = isnan(doc["var5"])? LOW : doc["var5"];
  if(var5 == digitalRead(RELAY5)) {
    if(var5 == LOW) digitalWrite(RELAY5, HIGH); else digitalWrite(RELAY5, LOW);
    delay(delaytime);
  }
  int var6 = isnan(doc["var6"])? LOW : doc["var6"];
  if(var6 == digitalRead(RELAY6)) {
    if(var6 == LOW) digitalWrite(RELAY6, HIGH); else digitalWrite(RELAY6, LOW);
    delay(delaytime);
  }
  int var7 = isnan(doc["var7"])? LOW : doc["var7"];
  if(var7 == digitalRead(RELAY7)) {
    if(var7 == LOW) digitalWrite(RELAY7, HIGH); else digitalWrite(RELAY7, LOW);
    delay(delaytime);
  }
  int var8 = isnan(doc["var8"])? LOW : doc["var8"];
  if(var8 == digitalRead(RELAY8)) {
    if(var8 == LOW) digitalWrite(RELAY8, HIGH); else digitalWrite(RELAY8, LOW);
    delay(delaytime);
  }
  int var9 = isnan(doc["var9"])? LOW : doc["var9"];
  if(var9 == digitalRead(RELAY9)) {
    if(var9 == LOW) digitalWrite(RELAY9, HIGH); else digitalWrite(RELAY9, LOW);
    delay(delaytime);
  }
  int var10 = isnan(doc["var10"])? LOW : doc["var10"];
  if(var10 == digitalRead(RELAY10)) {
    if(var10 == LOW) digitalWrite(RELAY10, HIGH); else digitalWrite(RELAY10, LOW);
    delay(delaytime);
  }
  int var11 = isnan(doc["var11"])? LOW : doc["var11"];
  if(var11 == digitalRead(RELAY11)) {
    if(var11 == LOW) digitalWrite(RELAY11, HIGH); else digitalWrite(RELAY11, LOW);
    delay(delaytime);
  }
  int var12 = isnan(doc["var12"])? LOW : doc["var12"];
  if(var12 == digitalRead(RELAY12)) {
    if(var12 == LOW) digitalWrite(RELAY12, HIGH); else digitalWrite(RELAY12, LOW);
  }
  savePinStatus();
 }

 void shutdownAll() {
  for(int i=1; i<=12; i++){
    String varName = "var";
    varName += i;
    doc[varName] = "0";    
  }
  applyChangesToPins(400);
}

void switchOnAll() {
  for(int i=1; i<=12; i++){
    String varName = "var";
    varName += i;
    doc[varName] = "1";    
  }
  applyChangesToPins(400);
}

void readInputPinsAndSync(){
  int changed = 0, pinValue = LOW;
  delay(100);
  pinValue = digitalRead(SW1);
  Serial.print("SW1 value: "); Serial.println(String(pinValue));
  if(pinValue != inp1) {
    changed=1; 
    inp1 = pinValue; 
    if(pinValue == LOW ) doc["var1"]= "1"; else doc["var1"]= "0";
  }

  delay(100);
  pinValue = digitalRead(SW2);
  Serial.print("SW2 value: "); Serial.println(String(pinValue));
  if(pinValue != inp2) {
    changed=1; inp2 = pinValue; 
    if(pinValue == LOW ) doc["var2"]= "1"; else doc["var2"]= "0";
  }

  delay(100);
  pinValue = digitalRead(SW3);
  Serial.print("SW3 value: "); Serial.println(String(pinValue));
  if(pinValue != inp3) {changed=1; inp3 = pinValue; if(pinValue == LOW ) doc["var3"]= "1"; else doc["var3"]= "0";}
  
  delay(100);
  pinValue = digitalRead(SW4);
  Serial.print("SW4 value: "); Serial.println(String(pinValue));
  if(pinValue != inp4) {changed=1; inp4 = pinValue; if(pinValue == LOW ) doc["var4"]= "1"; else doc["var4"]= "0";}

  delay(100);
  pinValue = digitalRead(SW5);
  Serial.print("SW5 value: "); Serial.println(String(pinValue));
  if(pinValue != inp5) {changed=1; inp5 = pinValue; if(pinValue == LOW ) doc["var5"]= "1"; else doc["var5"]= "0";}
  delay(100);
  pinValue = digitalRead(SW6);
  Serial.print("SW6 value: "); Serial.println(String(pinValue));
  if(pinValue != inp6) {changed=1; inp6 = pinValue; if(pinValue == LOW ) doc["var6"]= "1"; else doc["var6"]= "0";}
  delay(100);
  pinValue = digitalRead(SW7);
  Serial.print("SW7 value: "); Serial.println(String(pinValue));
  if(pinValue != inp7) {changed=1; inp7 = pinValue; if(pinValue == LOW ) doc["var7"]= "1"; else doc["var7"]= "0";}
  delay(100);
  pinValue = digitalRead(SW8);
  Serial.print("SW8 value: "); Serial.println(String(pinValue));
  if(pinValue != inp8) {changed=1; inp8 = pinValue; if(pinValue == LOW ) doc["var8"]= "1"; else doc["var8"]= "0";}
  
  
  Serial.println("Input Changed: ");
  Serial.print(String(changed));
  if(changed > 0){
    applyChangesToPins(50);
    events.send(getSensorReadings().c_str(),"new_readings", millis());
    wsClient.sendTXT(getSensorReadings().c_str());    
  }  
}

void evaluateChangesOnSocketMsg(){
  for(int i=1; i<=25; i++){
    String varName = "var";
    varName += i;
    const char* msgValue = msg[varName];
    // Serial.print(varName); Serial.print(" : "); Serial.println(msgValue);
    if(msgValue != NULL)
    {
      doc[varName] = msg[varName];
      // const char* docValue = doc[varName];
      // Serial.print(msgValue);Serial.print(" --- ");Serial.println(docValue);
    }
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT)
  {
    DeserializationError error = deserializeJson(msg, payload); 
    if (error) { 
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }  
    const String message = msg["message"];
    Serial.println(message);
    error = deserializeJson(msg, message); 
    if (error) { 
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    String jsonString = "";
    serializeJson(msg, jsonString);
    Serial.println(jsonString);
    evaluateChangesOnSocketMsg();
    applyChangesToPins(50);
    events.send(getSensorReadings().c_str(),"new_readings", millis());
  }else if (type == WStype_CONNECTED) {
    Serial.println("Connected to Socket ...");
    // TODO Sync with IoTConnectBD Server
    
  } else if (type == WStype_DISCONNECTED){
    Serial.println("Socket connection has been lost...");
  }
 }

void loadLastPinStatus() {
  File file = SPIFFS.open("/pinstatus.txt");
  if(!file){
    Serial.println("There was an error opening the file for reading...");
    return;
  }

  if(file.available()) {
    String allData = file.readString();
    if(allData != "") {
      Serial.print("PIN Status from File: ");
      Serial.println(allData);
      
      DeserializationError error = deserializeJson(msg, allData); 
      if (error) { 
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      evaluateChangesOnSocketMsg();
      applyChangesToPins(50);   
      events.send(getSensorReadings().c_str(),"new_readings", millis());
    }
  }
  file.close();
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
    shutdownAll();
  }else {
    Serial.println("SPIFFS mounted successfully");
    loadLastPinStatus();
  }
}

// Initialize WiFi
void initWiFi() {
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  // if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
  //   Serial.println("STA Failed to configure");
  // }
  WiFi.mode(WIFI_AP_STA);
  
  WiFi.softAP(ap_ssid, ap_password);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi .. Shikari WiFi");
  int attmCnt = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
    attmCnt++;
    if(attmCnt == 10) {
      Serial.print("Connecting to WiFi .. SDG8");
      WiFi.begin("SDG8", "deltaplan2071");
    }else if(attmCnt == 20)
    {
      Serial.print("Connecting to WiFi .. Programmer-DoICT-Jashore");
      WiFi.begin("Programmer-DoICT-Jashore", "4321567890");
    }else if(attmCnt == 30){
      WiFi.begin(ssid, password);
      attmCnt = 0;
    }
  }
  
  Serial.println(WiFi.localIP());
}



void toggleSwitch(int swNo){
  String varName = "var";
  varName += swNo;
  doc[varName] = (doc[varName] == "1")? "0":"1";
  applyChangesToPins(20);
  events.send(getSensorReadings().c_str(),"new_readings", millis());
  wsClient.sendTXT(getSensorReadings().c_str());   
}

void initPinMode() {
  
  pinMode(RELAY1, OUTPUT);  pinMode(RELAY2, OUTPUT);  pinMode(RELAY3, OUTPUT);  pinMode(RELAY4, OUTPUT);  pinMode(RELAY5, OUTPUT);
  pinMode(RELAY6, OUTPUT);  pinMode(RELAY7, OUTPUT);  pinMode(RELAY8, OUTPUT);  pinMode(RELAY9, OUTPUT);  pinMode(RELAY10, OUTPUT);
  pinMode(RELAY11, OUTPUT);  pinMode(RELAY12, OUTPUT);  
  
  digitalWrite(SW1, HIGH); digitalWrite(SW2, HIGH); digitalWrite(SW3, HIGH); digitalWrite(SW4, HIGH);
  digitalWrite(SW5, HIGH); digitalWrite(SW6, HIGH); digitalWrite(SW7, HIGH); digitalWrite(SW8, HIGH);

  pinMode(LDR, INPUT); pinMode(IRPin, INPUT); pinMode(DTH11, INPUT);
  pinMode(SW1, INPUT_PULLUP);  pinMode(SW2, INPUT_PULLUP); pinMode(SW3, INPUT_PULLUP); pinMode(SW4, OUTPUT); 
  pinMode(SW5, INPUT_PULLUP);  pinMode(SW6, INPUT_PULLUP); pinMode(SW7, INPUT_PULLUP);  pinMode(SW8, INPUT_PULLUP);
  inp1=1; inp3=1; inp5=1; inp7=1; inp9=1; inp2=1; inp4=1; inp6=1; inp8=1; inp10=1;
  
  
  MANUAL_SWITCH_MODE = false;
  BLUETOOTH_MODE = false;
}

void readBluetoothInput(){
  
  if (SerialBT.available()){
    char incomingChar = SerialBT.read();
    if (incomingChar != '\n'){
      btMessage += String(incomingChar);
    }
    else{
      btMessage = "";
    }
    Serial.write(incomingChar);  
  }

    if (btMessage == "SW1")
    {
      toggleSwitch(1);
      SerialBT.println("Relay1 is Toggled\n");
    }    
    else if (btMessage == "SW2")
    {
      toggleSwitch(2);
      SerialBT.println("Relay2 is Toggled\n");
    }
    else if (btMessage == "SW3")
    {
      toggleSwitch(3);
      SerialBT.println("Relay3 is Toggled\n");
    }
    else if (btMessage == "SW4")
    {
      toggleSwitch(4);
      SerialBT.println("Relay4 is Toggled\n");
    }
    else if (btMessage == "SW5")
    {
      toggleSwitch(5);
      SerialBT.println("Relay5 is Toggled\n");
    }
    else if (btMessage == "SW6")
    {
      toggleSwitch(6);
      SerialBT.println("Relay6 is Toggled\n");
    }else if (btMessage == "SW7")
    {
      toggleSwitch(7);
      SerialBT.println("Relay7 is Toggled\n");
    }else if (btMessage == "SW8")
    {
      toggleSwitch(8);
      SerialBT.println("Relay8 is Toggled\n");
    }else if (btMessage == "ALL_OFF")
    {
      shutdownAll();
      SerialBT.println("All Relays are gone shutdown\n");
    }else if (btMessage == "ALL_ON")
    {
      switchOnAll();
      SerialBT.println("All Relays are switched on\n");
    }
    else if (btMessage == "STATUS")
    {
      for(int i=1; i<=12; i++) {
        String varName = "var";
        varName += i;
        doc[varName] = "0";
        String swStatusStr = "Relay " + String(i) + ":  " +  (doc[varName] == "0")? "ON":"OFF";
        SerialBT.println(swStatusStr); 
      }
    }
  
}



void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  // initBME();
  SerialBT.begin(bluetoothName);
  initWiFi();
  initPinMode();
  initSPIFFS();
  doc["temperature"] = String(random(40)); //bme.readTemperature()
  doc["humidity"] =  String(random(100)); //bme.readHumidity()
  

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/change_mode", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inpMsg1;
    String inpMsg2;
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inpMsg1 = request->getParam(PARAM_INPUT_1)->value();
      inpMsg2 = request->getParam(PARAM_INPUT_2)->value();
      if(inpMsg1 == "MANUAL_SWITCH"){
        MANUAL_SWITCH_MODE = (inpMsg2=="1")? true : false;
      }
      if(inpMsg1 == "BLUETOOTH_MODE"){
        BLUETOOTH_MODE = (inpMsg2=="1")? true : false;
      }
      events.send(getSensorReadings().c_str(),"new_readings", millis());
      wsClient.sendTXT(getSensorReadings().c_str());  
    }
    request->send(200, "text/plain", "OK");
  });
  
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      if(inputMessage1 == "ALL")
      {
        if(inputMessage2 == "1") switchOnAll();
        if(inputMessage2 == "0") shutdownAll();
      } else {
        doc[inputMessage1] =  inputMessage2;
        applyChangesToPins(50);
      }
      
      // digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
      //Serial.print("RELAY: ");
      //Serial.print(inputMessage1);
      //Serial.print(" - Set to: ");
      //Serial.println(inputMessage2);
      
      events.send(getSensorReadings().c_str(),"new_readings", millis());
      wsClient.sendTXT(getSensorReadings().c_str());
      
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    
    request->send(200, "text/plain", "OK");
  });
  

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
  String wsUrl = "/iotconnect?deviceid="; wsUrl += deviceid; wsUrl += "&secretkey=";  wsUrl += securitykey;
  wsClient.begin("192.168.1.10", 5000, wsUrl);
  wsClient.onEvent(webSocketEvent);
  wsClient.setReconnectInterval(5000);
}

void loop() {

  wsClient.loop();
  
  if(MANUAL_SWITCH_MODE == true){
    readInputPinsAndSync();
    delay(2000);
  }

  if (BLUETOOTH_MODE == true) {
    readBluetoothInput();  
  }
  
  if ((millis() - lastTime) > timerDelay) {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
    lastTime = millis();
  }
  
}
