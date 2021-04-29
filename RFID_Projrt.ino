/*
 * 
 * All the resources for this project: https://www.hackster.io/Aritro
 * Modified by Aritro Mukherjee
 * 
 * 
 */
  
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>//defintion de ma librairie
 
#define SS_PIN 11
#define RST_PIN 7
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
Servo myservo;//declaration de ma variabe de type (servo)
Servo monServo;

//declaration des variables
int val = 0;//declaration de ma variable d'entree pour le moniteur
int LED = 6;
int etat_init = 1;
 
void setup() 
{
  Serial.begin(9600); //demarrage de la fréquence du moniteur seris
  SPI.begin();      // demarrage de la communication SPI
  mfrc522.PCD_Init();   // Initialisation de la communication MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(LED, OUTPUT);          // set arduino pin to output mode
  myservo.attach(4);//definition de ma broche pour le servo-moteur.
  monServo.attach(5);

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
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
  delay(2000);

if (chaine == " 89 C8 44 A2" ) // si le module detecte le badge ou le port clé 
  {
    Serial.print("etat_init");
    if(etat_init%2 == 1)
    {
    digitalWrite(LED, HIGH);
    myservo.write(100);
    monServo.write(255);
    etat_init++;
    delay(1000);
    }
    else
    {
     digitalWrite(LED, LOW);
     myservo.write(0); 
     monServo.write(0);
     etat_init++;
     delay(1000); 
    }
    }
}
