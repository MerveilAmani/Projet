
// Connection using WPA2 Enterpriser WIFI network


#include <WiFiNINA.h>
#include "wifiStatus.h"

//please enter your sensitive data for log on

//char ssid[] = "LeCamping";  // your WPA2 enterprise network SSID (name)
//char user[] = "ba-ido-01";  // your WPA2 enterprise username
//char pass[] = "Summer2019";  // your WPA2 enterprise password

char ssid[] = "BELL982";  // your WPA2 enterprise network SSID (name)
char pass[] = "17A4AF9F532F";  // your WPA2 enterprise password
/*
char ssid[] = "Linksys19681";  // your WPA2 enterprise network SSID (name)
char pass[] = "1234567890";  // your WPA2 enterprise password
*/
/*
char ssid[] = "BELL030";  // your WPA2 enterprise network SSID (name)
char pass[] = "6DC25222292E";  // your WPA2 enterprise password
*/
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiClient wifiClient; 

void wifiConnect()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA2 enterprise network:
    // - You can optionally provide additional identity and CA cert (string) parameters:
    //      WiFi.beginEnterprise(ssid, user, pass, identity, caCert)
    // . if your network requires them.
    //status = WiFi.begin(ssid, pass);
    status = WiFi.begin(ssid, pass);
    //status = WiFi.beginEnterprise(ssid, user, pass);
    Serial.println(status);
    // wait 10 seconds for connection:
    delay(10000);
  }
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

}
