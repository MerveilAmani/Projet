/* 
 Auteur: Merveil Amani
 Description: Ce code nous permet faire marché un systeme automatique pour ouvrir un tiroir a l'aide du module RFID, et d'envoyer les information (température et humidité) dans le tiroir et
 et du capteur de mouvement a la plateforme thingsboard ou on aura acces a l'information a tems rééel 
 Date: le 10 Mars 2021
*/
String tir;
String ONOFF ;

// Librairie Installé
#include "WIFIConnector.h"
#include "MQTTConnector.h"
#include <SPI.h> //defintion de ma librairie SPI.h pour le fonctionnement du module RFID
#include <MFRC522.h> //defintion de ma librairie MFRC522.h pour le fonctionnement du module RFID
#include <Servo.h> //defintion de ma librairie servo.h pour le fonctionnement du servomoteur
#include <DHT.h> //defintion de ma librairie DHT.h pour le fonctionnement du capteur de température et humidité (DHT22)

// Utilisation du #define pour l'attribution des broches
#define SS_PIN 11
#define RST_PIN 7
#define DHTPIN 2     
#define DHTTYPE DHT22   
#define cap 3
#define led 1
 
// Declaration des objets
Servo myservo; //declaration de ma variabe de type (servo)
Servo monServo;
MFRC522 mfrc522 (SS_PIN, RST_PIN);   // Create MFRC522 instance.
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT sensor for normal 16mhz Arduino

//declaration des variables
float hum;  
float temp; 
int mouv;
int etat_init = 1;

void setup() 
{
  Serial.begin(9600); //demarrage de la fréquence du moniteur seris
  SPI.begin();      // demarrage de la communication SPI
  mfrc522.PCD_Init();   // Initialisation de la communication MFRC522
  pinMode(cap, INPUT);  // definir le pinmode du capteur de mouvement en INPUT
  pinMode(led, OUTPUT);
  myservo.attach(4);  //definition de ma broche pour le servo-moteur.
  monServo.attach(5);
  dht.begin(); 
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  wifiConnect(); //la connexion au WIFI
  MQTTConnect(); // La connexion au MQTT
}

void loop() 
{
  mqqtClient.loop();
  hum = dht.readHumidity();
  temp= dht.readTemperature();
  mouv = analogRead(A0);
  if(tir == "LLED")
  {
    if(ONOFF =="true")
    {
      myservo.write(100);
      //Serial.println("HIGH");
    }
    else
    {
      myservo.write(0);
      //Serial.println("LOW");
    }
  }
  
  // Préparation de la detection du mouvement le capteur e
  if(digitalRead(cap)==HIGH) {
    Serial.println("Movement detecté");
    digitalWrite(led,HIGH);
  } else {
    Serial.println("Mouvement non détecté.");
    digitalWrite(led,LOW);
  }
  delay(1000);
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Sélectionnez l'une des cartes
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Afficher l'UID sur le moniteur série
  String UID = "";
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  String chaine = content;
  chaine.toUpperCase();
  Serial.println(chaine);
  delay(1000);
  
  //Condition si le module detecte la carte 
  if (chaine == "89 C8 44 A2" or "") 
  {
    Serial.print("etat_init");
    if(etat_init%2 == 1)
    {
    digitalWrite(led, HIGH);
    delay(2000);
    myservo.write(100);
    monServo.write(100);
    etat_init++;
    delay(1000);
    }
    else
    {
     digitalWrite(led, LOW);
     delay(2000);
     myservo.write(0); 
     monServo.write(0);
     etat_init++;
     delay(1000); 
    }
  }
  else {
   
    digitalWrite(led, HIGH);
    //tone(buzzer, 200, 750);
    delay(1000);
    //digitalWrite(led_rouge, LOW);
  }
  KeyValue[0] = temp;
  KeyValue[1] = hum;
  
  // Envoyer les données a Thingsboard
  SendPayload(TelemetryKey, KeyValue);
/*
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  KeyValue[0]= 1;
  Serial.println("High");
  SendPayload(TelemetryKey,KeyValue);
  delay(2000);                       // wait for a second
  
  
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  KeyValue[0]= 0;
  Serial.println("Low");
  SendPayload(TelemetryKey,KeyValue);
  delay(2000);
  
*/
}
