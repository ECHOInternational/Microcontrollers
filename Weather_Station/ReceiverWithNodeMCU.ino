/*********
  Modified from the examples of the Arduino LoRa library
  More resources: https://randomnerdtutorials.com
*********/
#include <ESP8266WiFi.h>
#include <LoRa.h>
#include <Adafruit_Sensor.h>
#include <ThingSpeak.h>


String apiKey = "VYTZPAQ1TKPN809W";     //  Enter your Write API key from ThingSpeak
const char * myWriteAPIKey = "VYTZPAQ1TKPN809W";

//const char *ssid =  "Hera";     // replace with your wifi ssid and wpa2 key
//const char *pass =  "MangoMadness@239";
//const char* server = "api.thingspeak.com";

const char *ssid =  "ECHO - Staff";     // replace with your wifi ssid and wpa2 key
const char *pass =  "Durrance17391RD";
const char* server = "api.thingspeak.com";

//define the pins used by the transceiver module
#define ss D8
#define dio0 D1

unsigned long myChannelNumber = 596676;

String myStatus = "";

int LoRaData;
 
WiFiClient client;

void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(905E6)) {
    Serial.println(".");
    delay(500);
  }
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  WiFi.begin(ssid, pass);
   
  while (WiFi.status() != WL_CONNECTED) 
  {
        delay(500);
        Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  ThingSpeak.begin(client);
}


void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
 
      // print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
                
      // Connect or reconnect to WiFi
      if(WiFi.status() != WL_CONNECTED){
        Serial.print("Attempting to connect to SSID: ");
        Serial.println("ECHO - Staff");
        while(WiFi.status() != WL_CONNECTED){
          WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
          Serial.print(".");
          delay(5000);     
        } 
        Serial.println("\nConnected.");
      }

      // set the fields with the values

      Serial.println("LoRaData String:");
      Serial.println(LoRaData);
      const int key = 9876;
      
      String strKey = getValue(LoRaData, ':', 0);
      String strTips = getValue(LoRaData, ':', 1);
      String strRainCm = getValue(LoRaData, ':', 2);
      String strTemp = getValue(LoRaData, ':', 3);
      String strHum = getValue(LoRaData, ':', 4);
      
      //Serial.println("Extracted String");
      //Serial.println(strPctShade);
      //Serial.println(strLum1);
      //Serial.println(strLum2);
      //Serial.println(strLum3);
      //Serial.println(strLum4);
      
      int intKey = strKey.toInt();
      //int intLum1 = strLum1.toInt();
      //int intLum2 = strLum2.toInt();
      //int intLum3 = strLum3.toInt();
      //int intLum4 = strLum4.toInt();

      //Serial.println("Converted to ints");
      //Serial.println(intPctShade);
      //Serial.println(intLum1);
      //Serial.println(intLum2);
      //Serial.println(intLum3);
      //Serial.println(intLum4);

      //float floatPctShade = (float)intPctShade / 100;
      //float floatLum1 = (float)intLum1 / 100;
      //float floatLum2 = (float)intLum2 / 100;
      //float floatLum3 = (float)intLum3 / 100;
      //float floatLum4 = (float)intLum4 / 100;

      //Serial.println("converted to floats");
      //Serial.println(floatPctShade);
      //Serial.println(floatLum1);
      //Serial.println(floatLum2);
      //Serial.println(floatLum3);
      //Serial.println(floatLum4);
      if (intKey == key) { 
      ThingSpeak.setField(1, strTips);
      ThingSpeak.setField(2, strRainCm);
      ThingSpeak.setField(3, strTemp);
      ThingSpeak.setField(4, strHum);
     

      //write to the ThingSpeak channel
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      if(x == 200){
        Serial.println("Channel update successful.");
      }
      else{
        Serial.println("Problem updating channel. HTTP error code " + String(x));
      }
     
      // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
      delay(30000);
      }
      else {};
    }
  }
}

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
