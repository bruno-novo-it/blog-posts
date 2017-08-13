#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "Switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks
void lightOneOn();
void lightOneOff();
void lightTwoOn();
void lightTwoOff();
void lightThreeOn();
void lightThreeOff();
void lightFourOn();
void lightFourOff();

void allDevicesOn();
void allDevicesOff();

void redLightsOn();
void redLightsOff();
void yellowLightsOn();
void yellowLightsOff();

// Change this before you flash
const char* ssid = "Login_da_Wifi";
const char* password = "Senha_da_Wifi";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *lightOne = NULL;
Switch *lightTwo = NULL;
Switch *lightThree = NULL;
Switch *lightFour = NULL;

Switch *allDevices = NULL;

Switch *redLights = NULL;
Switch *yellowLights = NULL;

// Set Relay Pins
//int relayOne = 13;
//int relayTwo = 12;
//int relayThree = 2;
//int relayFour = 0;

void setup()
{
  Serial.begin(115200);

  // Initialise wifi connection
  wifiConnected = connectWifi();

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    // Define your switches here. Max 14
    // Format: Alexa invocation name, local port no, on callback, off callback
    lightOne = new Switch("Light One", 80, lightOneOn, lightOneOff);
    lightTwo = new Switch("Light Two", 81, lightTwoOn, lightTwoOff);
    lightThree = new Switch("Light Three", 82, lightThreeOn, lightThreeOff);
    lightFour = new Switch("Light Four", 83, lightFourOn, lightFourOff);

    allDevices = new Switch("All Devices", 84, allDevicesOn, allDevicesOff);

    redLights = new Switch("Red Lights", 85, redLightsOn, redLightsOff);
    yellowLights = new Switch("Yellow Lights", 86, yellowLightsOn, yellowLightsOff);

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lightOne);
    upnpBroadcastResponder.addDevice(*lightTwo);
    upnpBroadcastResponder.addDevice(*lightThree);
    upnpBroadcastResponder.addDevice(*lightFour);
    upnpBroadcastResponder.addDevice(*allDevices);
    upnpBroadcastResponder.addDevice(*redLights);
    upnpBroadcastResponder.addDevice(*yellowLights);

    //Set relay pins to outputs
    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(0, OUTPUT);

  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();

    lightOne->serverLoop();
    lightTwo->serverLoop();
    lightThree->serverLoop();
    lightFour->serverLoop();
    allDevices->serverLoop();
    redLights->serverLoop();
    yellowLights->serverLoop();
  }
}

void lightOneOn() {
  Serial.print("Switch 1 turn on ...");
  digitalWrite(13, HIGH);   // sets relayOne on
}

void lightOneOff() {
  Serial.print("Switch 1 turn off ...");
  digitalWrite(13, LOW);   // sets relayOne off
}

void lightTwoOn() {
  Serial.print("Switch 2 turn on ...");
  digitalWrite(12, HIGH);   // sets relayOne on
}

void lightTwoOff() {
  Serial.print("Switch 2 turn off ...");
  digitalWrite(12, LOW);   // sets relayOne on
}

void lightThreeOn() {
  Serial.print("Switch 3 turn on ...");
  digitalWrite(5, HIGH);   // sets relayOne on
}

void lightThreeOff() {
  Serial.print("Switch 3 turn off ...");
  digitalWrite(5, LOW);   // sets relayOne off
}

void lightFourOn() {
  Serial.print("Switch 4 turn on ...");
  digitalWrite(0, HIGH);   // sets relayOne on
}

void lightFourOff() {
  Serial.print("Switch 4 turn off ...");
  digitalWrite(0, LOW);   // sets relayOne on
}

void allDevicesOn() {
  Serial.print("Switch all devices on");
  digitalWrite(13,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(0,HIGH);
}

void allDevicesOff() {
  Serial.print("Switch all devices off");
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(5,LOW);
  digitalWrite(0,LOW);
}

void redLightsOn() {
  Serial.print("Switch Red Lights on");
  digitalWrite(13,HIGH);
  digitalWrite(5,HIGH);
}

void redLightsOff() {
  Serial.print("Switch Red Lights off");
  digitalWrite(13,LOW);
  digitalWrite(5,LOW);
}

void yellowLightsOn() {
  Serial.print("Switch Yellow Lights on");
  digitalWrite(12,HIGH);
  digitalWrite(0,HIGH);
}

void yellowLightsOff() {
  Serial.print("Switch Yellow Lights off");
  digitalWrite(12,LOW);
  digitalWrite(0,LOW);
}



// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}
