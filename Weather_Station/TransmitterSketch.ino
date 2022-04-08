
/*
Code assembled by Tim Motis, much of which is adapted from that by Quintans (2015):

  Quintans, DJ 2015. Sneesl: An Arduino-based data-logger for rain collectors. Microprocessor software. 
  https://github.com/DesiQuintans/sneesl-rain-logger. doi:10.5281/zenodo.1204674 

Code related to debouncing of the reed swtich was sourced from: https://www.youtube.com/watch?v=KHrTqdmYoAk&t=872s 

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

// DEFINITIIONS OF PINS AND VARIABLES
// SD card 
#define MOSIpin 11
#define MISOpin 12
const int chipSelect = 4; //the CS/SS pin for the microSD card module
File logFile;
File backupFile;

// Rain sensor 
#define Tip 0.033058  //replace this number (cm of rain per tip) based on the area of your rain collector;
                      // Cm of rain per tip = tipping bucket volume in cubic centimeters divided by the 
                      // catchment area, in square cm, of your rain collector 
const int reedPin = 2;
const int ledPin = 5;
int TipCounter = 0;
volatile bool tipEventDetected = false;

//LoRa transceiver
#define ss 10
//#define rst 6  //have found that the transceiver works without the rst pin connected to the microcontroller
#define dio0 9

//External clock
RTC_DS1307 rtc; //this seems to work fine for me even though I'm using the DS3231
DateTime now;
int lastHour = 0;
int lastDay = 0;
static unsigned long timeOfLastTip = 0; //probably not needed since this sketch relies on hardware instead of software debouncing of the reed switch

//Temperature sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// #### Setup ####################################################

/*
    Error codes (Flashing of the LED connected to the ledPin [pin 5 as designated earlier]):
    Fast-fast    RTC couldn't be initialised.
    Fast-slow    SD card couldn't be initialised.
    2 fast 5 Slow All OK, starting to log data.
*/

void setup ()
{
    pinMode(ledPin, OUTPUT);
    pinMode(reedPin, INPUT);
    digitalWrite(reedPin, HIGH); // When the switch closes, this pin gets pulled low.
    
    // For the Arduino SD library:
    pinMode(chipSelect, OUTPUT);digitalWrite (chipSelect,HIGH);
    pinMode(MOSIpin, OUTPUT); digitalWrite(MOSIpin, HIGH);   //pullup the MOSI pin
    pinMode(MISOpin, INPUT); digitalWrite(MISOpin, HIGH);    //pullup the MISO pin
    delay(1000);
        
    // ---- RTC ------------------------------

    Wire.begin();
    rtc.begin();
    

    // Set the time by running the Set_RTC_Time sketch.

    if (! rtc.isrunning()) {
        while (true)
        {
            // Either the time is not set, or the RTC's oscillator is no good.
            // Stop running and just blink the LED like crazy.
            ledBlink(ledPin, 10, 100, 100, 100, 100);
        }
    }
    
    // ---- Timekeeping ----------------------

    DateTime now = rtc.now();
    lastHour = now.hour();
    lastDay = now.day();
    timeOfLastTip = millis();
        
    // ---- SD card --------------------------

    if (!SD.begin(chipSelect))
    {
        while (true)
        {
            // Stop running and do short-long-short-long blinks.
            ledBlink(ledPin, 10, 100, 100, 500, 100);
        }
    }
    
     // ---- Temperature sensor -------------------
    sht31.begin(0x44);
    float Temp = sht31.readTemperature();
    float Hum = sht31.readHumidity();
    delay (1000);
         
   // ---- LoRa transceiver -------------------
    Serial.begin(9600);    // Open serial communications and wait for port to open:
    LoRa.setPins(ss, dio0);
    
    //replace the LoRa.begin(---E-) argument with your location's frequency 
    //433E6 for Asia
    //866E6 for Europe
    //915E6 for North America
    while (!LoRa.begin(915E6)) {
      Serial.println(".");
      delay(200);
    }
    // Change sync word (0xF3) to match the receiver to avoid receiving messages from other LoRa transceivers
    // ranges from 0-0xFF
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");
    LoRaSend();
    delay (250); 
    
    // ---- Test write to SD card-----------------------

    bool logExists = false;

    if (SD.exists("logfile.csv"))
        logExists = true;

    logFile = SD.open("logfile.csv", FILE_WRITE);

    if (logFile)
    {
        if (logExists == false)
            logFile << "Time,Tips,RainCm,TempC,Hum%,Event,Volt" << endl; // Write a header row into a new file.
        
        logFile  << datestamp(now) << "T" << timestamp(now) << "," << TipCounter << "," << TipCounter * Tip << "," << Temp << "," << Hum << "," << "Rebooted" << "," << voltMeasure() << endl;
        // Need to be completely sure that writing is finished, so let's blink a little to pass time.
        ledBlink(ledPin, 2, 100, 50, 100, 50);
        logFile.close(); 
    }
    else
    {
        while (true)
        {
            // Stop running and do short-long-short-long blinks.
            ledBlink(ledPin, 10, 100, 100, 750, 100);
        }
    }
    
    // ---- Self-tests complete --------------

    // Give us some visual indication that everything is okay. If the LED light keeps blinking, check wiring (more often than not, the problem relates to 
    // incorrect wiring of the SD card module, a non-formatted SD card, or no SD card in the SD card module
    ledBlink(ledPin, 5, 500, 100, 500, 100);
}

// #### Main loop ################################################

void loop ()
{  
    attachInterrupt(0, recordTip, RISING); //Note that this works with hardware debouncing via a Schmitt trigger; if you are using software/code
                                           //to debounce the reed switch, you would type "LOW" instead of "RISING".
    
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); //wakes up the microcontroller every 8 seconds
    
    // I was woken by either a bucket tip or the watchdog timer. Either way, I'm awake already,
    // so I may as well check the time.
    
    detachInterrupt(0);
    
    if (tipEventDetected == true)
    {
        ledBlink(ledPin, 3, 50, 50, 50, 50);
        // Oh, I got tipped while I was sleeping! Better write that down.
        TipCounter++;
        tipEventDetected = false;
    }
    
    now = rtc.now();
    
    if (now.hour() != lastHour)
    {
        // ledBlink(ledPin, 2, 50, 50, 50, 50);
        // It's a new hour! I can write data! 
        logWrite();
        TipCounter = 0;
        lastHour = now.hour();
    }
    
    if (now.day() != lastDay)
    {
        // It's a new day! I can back-up the log file!
        logBackup();
        lastDay = now.day();
        
    }
}

// FUNCTIONS CALLED IN SETUP OR THE LOOP

void recordTip()
{
    // I heard something. Probably a tip event!
    // This function would contain software debouncing code would go if you are not debouncing
    // via hardware (in this case, a Schmitt trigger)
    tipEventDetected = true;  

    /* if you do not have a Schmitt trigger, replace the preceding line with the following software debouncing code:
       
       int timeDifference = int(abs(millis() - timeOfLastTip));
       if (timeDifference >= 2000)
       {
        tipEventDetected = true;
        timeOfLastTip = millis();
       }
    */
}

void logWrite()
{
    //gather data to log to the SD card and send to the receiver via LoRa 
    float Tips = TipCounter;
    float RainCm = TipCounter*Tip;
    float Temp = sht31.readTemperature();
    float Hum = sht31.readHumidity();
    float finalVolt = voltMeasure();
    const int Key = 9876; //this four-digit (nothing special about 4 digits) key should match that in the receiver sketch;
                          //the receiver will only process data that contains this key; I find that, in addition to the 
                          //sync word, this is needed to prevent the receiver from processing unwanted data that may be 
                          //coming from other transceivers; assign any combination of numbers as your key
    
    //save the data on an SD card    
    logFile = SD.open("logfile.csv", FILE_WRITE);

    if (logFile)
    {
        logFile << datestamp(now) << "T" << timestamp(now) << "," << TipCounter << "," << TipCounter * Tip << "," << Temp << "," << Hum << "," << "Normal" << "," << voltMeasure() << endl;
        // Need to be completely sure that writing is finished. Should take 0.25ms,
        // so here's me being 500% sure.
        delay(125);
        logFile.close();
    }
    else
    {
        // Couldn't open.
    }

   //transmit the data to the receiver
   LoRa.beginPacket();
   LoRa.print (Key); //enables inclusion of the key as part of data transmitted to the receiver
   LoRa.print (":");
   LoRa.print (Tips);
   LoRa.print (":");
   LoRa.print (RainCm);
   LoRa.print (":");
   LoRa.print (Temp);
   LoRa.print (":");
   LoRa.print (Hum);
   LoRa.endPacket(); 
   delay(250);
}

void logBackup()
{
    float Temp = sht31.readTemperature();
    float Hum = sht31.readHumidity();
    
    String shortyear;
    shortyear += now.year();
    shortyear.remove(0,2); // Keep only the decade so that I have room for the hour.
    
    String backupFilename;
    backupFilename += shortyear;
    backupFilename += withLeadingZero(now.month());
    backupFilename += withLeadingZero(now.day());
    backupFilename += withLeadingZero(now.hour());
    backupFilename += ".csv";
    
    logFile = SD.open("logfile.csv", FILE_READ);
    backupFile = SD.open(backupFilename.c_str(), FILE_WRITE); // c_str() because SD.open wants a char array.
   
    if (logFile && backupFile)
    {
        while (logFile.available()) {
          backupFile.write(logFile.read());
        }
        // Stick a delay in for good measure. No idea if this is long enough; each log line
        // is supposed to be ~25 bytes, so at a write rate of 4 bytes per ms, that's only 4kb
        // of data. But really, you'd expect the SD library to handle writing timing.
        delay(1000);
        logFile.close();
        backupFile.close();
    }
    else
    {
        // Couldn't open.
    }
    
    logFile = SD.open("logfile.csv", FILE_WRITE);

    if (logFile)
    {
        
        logFile << datestamp(now) << "T" << timestamp(now) << "," << TipCounter << "," << TipCounter * Tip << "," << Temp << "," << Hum << "," << "Daily backup" << "," << voltMeasure() << endl;
        // Need to be completely sure that writing is finished. Should take 0.25ms,
        // so here's me being 500% sure.
        // if you are not wiring your micrcontroller with a voltage divider for checking voltage, you could omit the "voltMeasure()" function
        delay(125);
        logFile.close();
    }
    else
    {
        // Couldn't open.
    }
}

void LoRaSend() //this function is called in Setup so that, for troublehooting purposes, you can make sure the LoRa transceiver
                //transmissions are being received after pressing the reset button on the microcontroller
{
   float Tips = TipCounter;
   float RainCm = TipCounter*Tip;
   float Temp = sht31.readTemperature();
   float Hum = sht31.readHumidity();
   const int Key = 9876;
   
   LoRa.beginPacket();
   LoRa.print (Key);
   LoRa.print (":");
   LoRa.print (Tips);
   LoRa.print (":");
   LoRa.print (RainCm);
   LoRa.print (":");
   LoRa.print (Temp);
   LoRa.print (":");
   LoRa.print (Hum);
   LoRa.endPacket(); 
   delay(250);

}
String datestamp(DateTime now)  //this and the next two blocks of code enable time-stamping of data on SD card
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
    output += ":";
    output += withLeadingZero(now.minute());
    output += ":";
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

void ledBlink(int ledPin, int flashes, int evenOn, int evenOff, int oddOn, int oddOff) 
{
    // Utility function for blinking an LED.
    // It intentionally blocks with delay() to buy time for processes.
    bool even = false;
    for(int i=0; i<flashes; i++)
    {
        digitalWrite(ledPin, HIGH);
        
        if (even == false)
            delay(evenOn);
        else
            delay(oddOn);
            
        digitalWrite(ledPin, LOW);
        
        if (even == false)
            delay(evenOff);
        else
            delay(oddOff);
            
        even = !even;
    }
}

float voltMeasure()
{
    // Voltage measurement code from W.A. Smith, http://startingelectronics.org
    
    int sum = 0;                    // sum of samples taken
    unsigned char sample_count = 0; // current sample number
    float voltage = 0.0;            // calculated voltage
    
    while (sample_count < 10)
    {
        sum += analogRead(A2);
        sample_count++;
        delay(10);
    }
    
    // calculate the voltage
    // use 5.0 for a 5.0V ADC reference voltage
    // 5.015V is the calibrated reference voltage
    voltage = ((float)sum / (float)10 * 5.015) / 1024.0;
    // send voltage for display on Serial Monitor
    // voltage multiplied by 11 when using voltage divider that
    // divides by 11. 11.132 is the calibrated voltage divide
    // value
    
    return voltage;
}
