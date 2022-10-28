//Moji grafi so dosegljivi na: https://thingspeak.com/channels/1912085
#include <Ethernet.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

byte mac[] = SECRET_MAC;

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(10, 8, 128, 120);
IPAddress myDns(212, 235, 181, 41);

EthernetClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Ethernet.init(10);  // Most Arduino Ethernet hardware
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
      
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}
void loop() {
  float temp = (((analogRead(A1)-18.0)*(5000.0/1023))-400)/19.5;
  float svet = (analogRead(A0));
  // set the fields with the values
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, svet);

  
  // write to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  delay(20000); // Wait 20 seconds to update the channel again
}
