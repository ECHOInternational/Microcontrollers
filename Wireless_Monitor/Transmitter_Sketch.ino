
/* 
Code assembled by Tim Motis from various sources:

  Code enabling hourly data transmission: https://github.com/DesiQuintans/sneesl-rain-logger
  Code related to the temperature sensor (SHT31): Arduino IDE examples
  
 */

// LIBRARIES

#include <Wire.h>
#include <RTClib.h>
#include <Arduino.h>
#include "LowPower.h"
#include "Adafruit_SHT31.h"
#include <SPI.h>
#include <SD.h>
#include <Streaming.h>
#include <LoRa.h>

// DEFINITIONS OF PINS AND VARIABLES
#define MOSIpin 11
#define MISOpin 12
const int ledPin = 9;

//define the pins used by the transceiver module
#define ss 8
#define rst 6  //I've found that LoRa works without connecting the RST LoRa pin to the microcontroller
#define dio0 3

// define variables 
RTC_DS1307 rtc;
File logFile;
File backupFile;
DateTime now;
int lastHour = 0;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// #### SETUP ####################################################

/*
    Error codes (Flashing built-in LED (usually pin 13):
    Fast-fast    RTC couldn't be initialised.
    Fast-slow    SD card couldn't be initialised.
    Slow-slow    Couldn't access DHT11.
    2 fast 5 Slow All OK, starting to log data.
*/

void setup ()
{
    // For the Arduino SD library:
    //pinMode(10, OUTPUT);digitalWrite (10,HIGH);
    //pinMode(MOSIpin, OUTPUT); digitalWrite(MOSIpin, HIGH);   //pullup the MOSI pin
    //pinMode(MISOpin, INPUT); digitalWrite(MISOpin, HIGH);    //pullup the MISO pin
    //delay(1000);
    Serial.begin(9600);
        
    // ---- RTC ------------------------------

    Wire.begin();
    rtc.begin();
    

    // Set the time by running the Set_RTC_Time sketch.

    if (! rtc.isrunning()) {
        while (true)
        {
            // Either the time is not set, or the RTC's oscillator is no good.
            // Stop running and just blink the LED like crazy.
        }
    }
    
    // ---- Timekeeping ----------------------

    DateTime now = rtc.now();
    lastHour = now.hour() ;
      
    // ---- SHT 31 package -------------------
    
    
    sht31.begin(0x44);
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
    
    delay (1000);

    // ---- LoRa transceiver -------------------
    Serial.begin(9600);    // Open serial communications and wait for port to open:
    //analogReference(EXTERNAL); // 
    LoRa.setPins(ss, dio0);  //if you want to include the RST pin, write "ss, rst, dio" inside the parenthesis
    
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
       
    // Function built out further down in the sketch that sends data to the receiver
    DataSend();
}

// #### MAIN LOOP ################################################

void loop ()
{  
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
    
    // I was woken by the watchdog timer so will check the time
    
    now = rtc.now();
    
    if (now.hour()!= lastHour)
    {
        // It's a new hour! I can write data! 
        DataSend();
        lastHour = now.hour() ;
    }
    
 }

// #### FUNCTIONS ####################################

void DataSend ()
{ 
  
  float Humidity = sht31.readHumidity();
  float TemperatureC = sht31.readTemperature(); //by default the sensor reads in degrees Celsius
  float Temperature = TemperatureC * 1.8 + 32; //converts temperature in degrees Celsius to degrees Fahrenheit
  String Date = datestamp(now);
  String Time = timestamp(now); 
  const int Key = 0000;  //this four-digit (nothing special about 4 digits) key should match that in the receiver sketch;
                         //the receiver will only process data that contains this key; I find that, in addition to the 
                         //sync word, this is needed to prevent the receiver from processing unwanted data that may be 
                         //coming from other transceivers; assign any combination of numbers as your key
  
   LoRa.beginPacket();
   LoRa.print (Key); //enables inclusion of the key as part of data transmitted to the receiver
   LoRa.print (":");
   LoRa.print (Date);
   LoRa.print (":");
   LoRa.print (Time);
   LoRa.print (":");
   LoRa.print (Humidity);
   LoRa.print (":");
   LoRa.print (Temperature);
   LoRa.endPacket(); 
   delay(200);

   Serial.println(Key);
   Serial.println(Time);
   Serial.println(Humidity);
   Serial.println(TemperatureC);
   Serial.println(Temperature);
}

String datestamp(DateTime now) //this and the next two blocks of code enable time-stamping of data sent to the OLED display
{
    String output;
    output += now.year();
    output += "-";
    output += withLeadingZero(now.month());
    output += "-";
    output += withLeadingZero(now.day());
    return output;
}

String timestamp(DateTime now)
{
    String output;
    output += withLeadingZero(now.hour());
    output += "-";
    output += withLeadingZero(now.minute());
    output += "-";
    output += withLeadingZero(now.second());
    return output;
}

String withLeadingZero(int digits) {
    // Utility function for printing leading zeroes.
    String output;
    
    if (digits < 10)
        output += "0";
    
    output += digits;
    return output;
}


    
