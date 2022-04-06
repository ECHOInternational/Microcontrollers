/*
Code assembled by Tim Motis from various sources:

  Adafruit code modified from the examples of the Arduino Adafruit IO library
  and Adafruit tutorial: https://learn.adafruit.com/welcome-to-adafruit-io
  
  LoRa transceiver code based on content by Sandeep Mistry (https://github.com/sandeepmistry/arduino-LoRa) and 
  Emmanuel Odunlade (https://www.electronics-lab.com/project/introduction-lora-send-data-two-arduino-using-lora/)

  OLED display: https://arduinogetstarted.com/tutorials/arduino-oled
  Some lines of code related to the OLED display may not be needed. The sketch, however, works well as is.
  
*/

// LIBRARIES
#include "configAdafruit_IO.h"
#include <ESP8266WiFi.h>
#include <LoRa.h>
#include <Adafruit_Sensor.h>
#include <ThingSpeak.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>

// DEFINITIONS OF PINS AND VARIABLES
//define the pins used by the transceiver module; wiring needs to match (e.g., connect SS pin on the LoRa transceiver to pin D8 on the ESP8266 [NodeMcu])
#define ss D8
#define dio0 D0

String myStatus = "";

int LoRaData;

WiFiClient client;

// Define OLED display parameters
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperature = io.feed("Air Temperature");
AdafruitIO_Feed *humidity = io.feed("Relative Humidity");

// #### SETUP ####################################################
void setup() {
 // Start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
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

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
 
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  
  display.println("LoRa Receiver");
  display.display(); 
  
  while (!Serial);
  
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, dio0);
  
  //replace the LoRa.begin(---E-) argument with your location's frequency 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(915E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0x25);
  Serial.println("LoRa Initializing OK!");

  // Setup the WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
   
  while (WiFi.status() != WL_CONNECTED) 
  {
        delay(500);
        Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  //Setup OLED 
  Wire.begin(); 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //use something other than 0x3C if address differs
  display.clearDisplay(); //clear the buffer
  display.setTextColor(WHITE);
  display.clearDisplay(); // clear display
}


void loop() {
  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // Read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
 
      // Print RSSI of packet
      Serial.print("' with RSSI ");
      Serial.println(LoRa.packetRssi());
                
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("Your_WiFi_SSID"); //replace, between quotation marks, with your WiFi SSID
      while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(WIFI_SSID, WIFI_PASS); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
      } 
      Serial.println("\nConnected.");
      }

    // set the fields with the values
    Serial.println("LoRaData String:");
    Serial.println(LoRaData);
    const int key = 0000; //This key should match that specified in the transmitter sketch
      
    String strKey = getValue(LoRaData, ':', 0);
    String strDate = getValue(LoRaData, ':', 1);
    String strTime = getValue(LoRaData, ':', 2);
    String strHumidity = getValue(LoRaData, ':', 3);
    String strTemperature = getValue(LoRaData, ':', 4);
  
    int intKey = strKey.toInt();
    int intDate = strDate.toInt();
    int intTime = strTime.toInt();
    int intHumidity = strHumidity.toInt();
    int intTemperature = strTemperature.toInt();
    
    if (intKey == key) { 
      Serial.println("Extracted String");
      Serial.println(strDate);
      Serial.println(strTime);
      Serial.println(strHumidity);
      Serial.println(strTemperature);
      
      display.clearDisplay(); //clear the buffer
      delay(2);
      display.setTextColor(WHITE);
      display.clearDisplay(); // clear display   
  
     // Display date/time
      display.setTextSize(1);
      display.setCursor(0,0);
      //display.print("Date: ")  
      display.print(strDate);
      display.print(": ");
      display.print(strTime);
      
     // Display temperature
      display.setTextSize(1);
      display.setCursor(0,16);
      display.print("Temp: ");
      display.setTextSize(2);
      display.print(strTemperature);
      display.print(" ");
      display.setTextSize(1);
      display.cp437(true);
      display.write(167);
      display.setTextSize(2);
      display.print("F");
    
    // Display humidity
      display.setTextSize(1);
      display.setCursor(0, 42);
      display.print("RH:   ");
      display.setTextSize(2);
      //display.setCursor(0, 45);
      display.print(strHumidity);
      display.print(" %"); 
    
      display.display();
      delay(2000);
      display.clearDisplay(); // clear display
      //display.ssd1306_command(SSD1306_DISPLAYOFF)//this is useful for low-power applications where you want the display to turn off between readings
      
      io.run();
    
      Serial.print("Temperature: ");
      Serial.print(strTemperature);
      Serial.println("C");
    
      Serial.print("Humidity: ");
      Serial.print(strHumidity);
      Serial.println("%");
    
      //save Temperature to Adafruit IO
      temperature->save(strTemperature);
    
      // save humidity to Adafruit IO
      humidity->save(strHumidity);
    
      // wait 5 seconds (5000 milliseconds == 5 seconds)
      delay(5000);
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
