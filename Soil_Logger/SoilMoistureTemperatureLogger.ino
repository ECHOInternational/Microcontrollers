
/*
Receiver sketch for receiving sensor values from the field (transmitter) and publishing
data to a web platform (Adafruit IO and/or Thingspeak) for online viewing.

LoRa code modified from the examples of the Arduino LoRa library
More resources: https://randomnerdtutorials.com

Adafruit code modified from the examples of the Arduino Adafruit IO library
and Adafruit tutorial: https://learn.adafruit.com/welcome-to-adafruit-io

Thingspeak portion adapted from code by:
  The Mathworks, Inc.: https://github.com/mathworks/thingspeak-arduino/blob/master/examples/ESP8266/program%20board%20directly/WriteMultipleFields/WriteMultipleFields.ino
  benjineer.io: https://www.instructables.com/Tutorial-2-Remote-Relay-Node/
*/

// LIBRARIES
#include "configAI_Thingspeak.h" //insert WiFi and Adafruit/Thingspeak account
                                 //info in the configAI_Thingspeak.h tab
#include <ESP8266WiFi.h> // Do not confuse this with WiFi.h, the library you 
                         // would need if using an ESP32 microcontroller
#include <LoRa.h>
#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>
#include <ThingSpeak.h>

// DEFINITIONS OF PINS AND VARIABLES
// Definitions for the LoRa transceiver module
#define ss D8
#define dio0 D0
int LoRaData;

// Variable for WiFi
WiFiClient client;

// Set up the 'temperature' and 'humidity' feeds for Adafruit
AdafruitIO_Feed *soiltemp = io.feed("Soil temperature");
AdafruitIO_Feed *soilmoistmv = io.feed("Soil moisture value"); 
                             //millivots read by the soil moisture sensor
AdafruitIO_Feed *soilmoistpct = io.feed("Soil moisture percent");

String myStatus = "";

void setup() {
 // Start the serial connection
  Serial.begin(115200);

  // Wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // Connect to io.adafruit.com
  io.connect();

  // Wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // We are connected
  Serial.println();
  Serial.println(io.statusText());

  //Initialize LoRa transceiver 
  LoRa.setPins(ss, dio0);
  
  //Replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
   // The sync word (0xF3) here (the receiver/gateway) and in code for the transmitter/node
   // should match. The sync word ranges from 0-0xFF. Its purpose is to prevent receiving 
   // data from other LoRa transceivers.
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  WiFi.begin(WIFI_SSID, WIFI_PASS);
   
  while (WiFi.status() != WL_CONNECTED) 
  {
        delay(500);
        Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  ThingSpeak.begin(client); //Could delete or comment out (enter two slash marks (//) before
                            //"Thingspeak...") if not using ThingSpeak

 }


void loop() {
  // Try to parse incoming data packet sent from the field (transmitter)
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Indicate reception of a packet
    Serial.print("Received packet '");

    // Read an incoming data packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
 
      // Print RSSI identifier of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
                
      // Connect or reconnect to WiFi
      if(WiFi.status() != WL_CONNECTED){
        Serial.print("Attempting to connect to SSID: ");
        Serial.println("Your WiFi network"); //Enter the name of your WiFi network (optional)
        while(WiFi.status() != WL_CONNECTED){
          WiFi.begin(WIFI_SSID, WIFI_PASS); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
          Serial.print(".");
          delay(5000);     
        } 
        Serial.println("\nConnected.");
      }

      // Set field names based on incoming LoRa data values
      Serial.println("LoRaData String:");
      Serial.println(LoRaData);
      const int Key = 4252; //modify the number key as needed to match the 
                            //transmitter key
      
      String strKey = getValue(LoRaData, ':', 0);
      String strSoilTemp = getValue(LoRaData, ':', 1);
      String strSoilMoistureValue = getValue(LoRaData, ':', 2);
      String strSoilMoisturePct = getValue(LoRaData, ':', 3);

      //Convert string to integer number  
      int intKey = strKey.toInt();
      int intSoilTemp = strSoilTemp.toInt();
      int intSoilMoistureValue = strSoilMoistureValue.toInt();
      int intSoilMoisturePct = strSoilMoisturePct.toInt();

      // Prevent online publishing unlesss the LoRa data packet
      // contains the unique number key; prevents publishing data
      // from any other nearby transmitters; it is possible that 
      // "if (strKey == Key)" will also work--if so, the preceding block 
      // of code to convert string numbers to integer numbers may not
      // be necessary
      if (intKey == Key) { 
        Serial.println("Extracted String");
        Serial.println(strKey);
        Serial.println(strSoilTemp);
        Serial.println(strSoilMoistureValue);
        Serial.println(strSoilMoisturePct);
                  
        io.run();
      
        Serial.print("Soil temperature: ");
        Serial.print(strSoilTemp);
        Serial.println(" C");
      
        Serial.print("Soil moisture value: ");
        Serial.print(strSoilMoistureValue);
        Serial.println(" mV");
      
        Serial.print("Soil moisture percent: ");
        Serial.print(strSoilMoisturePct);
        Serial.println(" %");
        
        // Save data to Adafruit IO (delete or comment out if only using Thingspeak)
        // Save soil temperature value to Adafruit IO
        soiltemp->save(strSoilTemp);
      
        // Save soil moisture millivolt value to Adafruit IO
        soilmoistmv->save(strSoilMoistureValue);
      
        // Save soil moisture percentage value to Adafruit IO
        soilmoistpct->save(strSoilMoisturePct);
      
        // Wait 5 seconds (5000 milliseconds == 5 seconds)
        delay(5000);

        // Publish data to Thingspeak (delete or comment out if only using Adafruit IO)
        ThingSpeak.setField(1, strSoilTemp);
        ThingSpeak.setField(2, strSoilMoistureValue);
        ThingSpeak.setField(3, strSoilMoisturePct);
        
        // Write to the ThingSpeak channel
        int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
        if(x == 200){
          Serial.println("Channel update successful.");
          }
        else{
          Serial.println("Problem updating channel. HTTP error code " + String(x));
          }
       
        // Thingspeak needs minimum 15 sec delay between updates. Here it is set to 30 seconds.
        delay(30000);
            
        }
  else {};
  }
  }
}

// Break string values into separate fields to post on Thingspeak;
// Delete or comment out the following code if not using Thingspeak.
String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
