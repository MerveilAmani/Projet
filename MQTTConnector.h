#include <MQTT.h>

const char mqttServer[] = "198.164.130.75"; // IP adress of IDO ThingsBoard Server (Broker) in the classroom
const int mqttServerPort = 1883; // broker mqtt port

const char key[] = "dtmLvCjylZf2ZLxRGRuJ"; // broker access token
const char secret[] = ""; // broker secret
const char device[] = "b00e5cc0-a8cf-11eb-a456-4566dede7767"; // broker device identifier

//Modify this section to reflect the number of telemetry keys your are using

//NumberOfTelemetryKeys is the number of values that are sent via MQTT to the broker
//MaxLenghtTelemetryKeyName is the maximum lenght of the key name

const int NumberOfTelemetryKeys = 2;
const int MaxLenghtTelemetryKeyName = 50;


// Array to hold the values (take a look in the loop function

float KeyValue[NumberOfTelemetryKeys];


// Define your key names here
char  TelemetryKey[NumberOfTelemetryKeys][MaxLenghtTelemetryKeyName]={
  "Temperature",
  "Humidité"
};

MQTTClient mqqtClient;  // Create a MQTT client to handle messagin between object and broker

void messageReceived(String &topic, String &payload) {
 
  Serial.println(payload);
  tir = payload.substring(11,15);
  Serial.print("PORTE -> ");
  Serial.println(tir);
  ONOFF = payload.substring(26,payload.length() -1);
  Serial.print("ONOFF -> ");
  Serial.println(ONOFF);
  Serial.println("Message received");
}

void MQTTConnect() {
  
  mqqtClient.begin(mqttServer, mqttServerPort, wifiClient);
  Serial.print("Connecting to broker...");
  mqqtClient.onMessage(messageReceived);
  while (!mqqtClient.connect(device, key, secret)) {
    Serial.print(".");
    delay(1000);
  }
  mqqtClient.subscribe("v1/devices/me/rpc/request/+");
  Serial.println("\nConnected to MQTT!\n");

}

void SendPayload(char TelemetryKey[NumberOfTelemetryKeys][MaxLenghtTelemetryKeyName],float KeyValue[NumberOfTelemetryKeys])
{

char attributes[200];

String payload = "{";
  for (int i=0; i < NumberOfTelemetryKeys; i++)
  {
    payload += "\"" + String(TelemetryKey[i]) + "\":";
    payload += String(KeyValue[i]);

    if(i != NumberOfTelemetryKeys-1)
    {
      payload +=",";
    }
  }
payload += "}";

  payload.toCharArray(attributes, 200);
  mqqtClient.publish("v1/devices/me/telemetry", attributes);
  Serial.println(attributes);

}
