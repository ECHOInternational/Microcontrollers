
/*
Transmitter sketch for reading sensors and transmitting data, via a LoRa transceiver, to a receiver.

Code sources:
  Push button code by David Bird: https://www.youtube.com/watch?v=dZ1ohpCmkdA;
  https://github.com/G6EJD/ESP32-Push-button-or-Pulse-Wake-up/blob/master/ESP32_Switrch_WakeUp_Example.ino
  
  Wakeup switch code by Pranav Cherukupalli: https://randomnerdtutorials.com/esp32-external-wake-up-deep-sleep/

  LoRa transceiver code based on content by Sandeep Mistry (https://github.com/sandeepmistry/arduino-LoRa) and 
  Emmanuel Odunlade (https://www.electronics-lab.com/project/introduction-lora-send-data-two-arduino-using-lora/)

  DS3231 coding, using just the Wire.h library, adapted from code by John Boxall:
  https://tronixstuff.com/2014/12/01/tutorial-using-ds1307-and-ds3231-real-time-clock-modules-with-arduino/

  Soil moisture and OLED display code adapted from a how2electronics project example: 
  https://how2electronics.com/capacitive-soil-moisture-sensor-esp8266-esp32-oled-display/

  Micro SD card module code adapted from coding by Rui Santos:
  https://randomnerdtutorials.com/esp32-data-logging-temperature-to-microsd-card/

  Code for sensing soil temperature with the DS18B20 One-Wire sensor from a Last Minute Engineers tutorial:
  https://lastminuteengineers.com/ds18b20-arduino-tutorial/

Code assembled by Tim Motis 
*/

//LIBRARIES
#include <driver/rtc_io.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <Streaming.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LoRa.h>

//DEFINITIONS OF PINS AND VARIABLES
//Define LoRa transceiver pins
#define ss 16
#define dio0 2

// Define sleep time that the internal clock tracks before waking up to log/transmit readings
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
uint64_t TIME_TO_SLEEP = 1800; //Sleep for 10 minutes = 600 seconds; 1800 (600 X 3) codes for 30 min

//External clock (DS3231 RTC) clock settings
#define DS3231_I2C_ADDRESS 0x68
byte zero = 0x00; //workaround for issue #527
  
// One-Wire Dallas temperature sensor (DS18B290) sensor settings 
#define ONE_WIRE_BUS 32 //Data wire is connected to ESP32 GPIO 32
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with a OneWire device
DallasTemperature sensors(&oneWire); //// Pass our oneWire reference to Dallas Temperature sensor

// Initialize a variable for saving the reading number in internal memory
RTC_DATA_ATTR int readingID = 0; //"RTC_DATA_ATTR" enables memory of readings in sleep mode

String dataMessage;

//Initialization of soil moisture sensor
  const int dryValue = 2900;   //millivolts in dry soil (replace with value for your soil)
  const int wetValue = 1455;  //millivolots in wet/saturated soil (replace with value for your soil)
  const int soilPin = 35; //analog output pin of soil moisture sensor connected to pin 35
  int moistureValue = 0; //initial value needs to be zero; do not change
  int moisturePercent=0; //initial value needs to be zero; do not change
  
// SD Card setup
#define SD_CS 5  //CS pin on SD card module connected to pin 5 on ESP32
File logFile;

// Define OLED display parameters
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Start serial communication for debugging purposes
  Serial.begin(115200); 

  //Initialize SD card
  SD.begin(SD_CS); 
  if(!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return;    // init failed
  }

  // If the data.txt file doesn't exist, create a file on the SD card and write the data labels.
  // The data labels constitute column headings when opening the data.txt file in Excel.
  File file = SD.open("/data.txt");
  if(!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Reading ID, Date, Time, Temperature, Moisture_mV, MoisturePct \r\n");
  }
  else {
    Serial.println("File already exists");  
  }
  file.close();
  
  Wire.begin(); 
  
  //set the initial time 
  //setDateTime(); //when setting the DS3231 clock time, uncomment (by deleting the two slash marks
                   //before "setDateTime()"), set the date/time (by adjusting values in the "voidSetDateTime"
                   //function of this sketch), and upload the sketch to the ESP32 with the 
                   //DS3231 connected to the ESP32; then comment out (by re-typing the two 
                   //slash marks before "setDateTime()")and re-upload this sketch to the ESP32; 
                   //if you do not do this, the clock time will reset to the values in the "voidSetDateTime"
                   //function every time the ESP32 wakes up or is reprogrammed---since, in either case,
                   //the ESP32 implements Setup commands, causing the clock to lose the current time. 

  //Set the ESP32 to wake up after the time specified in the Definition of Pins and Variable section 
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  
  //Set the ESP32 to wake up with an external trigger, the push button connected to pin 15.
  rtc_gpio_pulldown_en((gpio_num_t)GPIO_NUM_15); //an internal resistor pulls pin 15 low 
  esp_sleep_enable_ext0_wakeup((gpio_num_t)GPIO_NUM_15, RISING); //enables pin 15 to recognize an
    //an incoming pulse (rise) of electricity when the push button is pressed; the pulse occurs becasue 
    //one side of the push button is connected to power (3.3 volts) on the ESP32.
  
 // ---- Setup LoRa transceiver -------------------
    LoRa.setPins(ss, dio0);
    
    //replace the LoRa.begin(---E-) argument with your location's frequency 
    //433E6 for Asia
    //866E6 for Europe
    //915E6 for North America
    while (!LoRa.begin(915E6)) {
      Serial.println(".");
      delay(200);
    }
    // The sync word (0xF3) here (the transmitter/node) and in code for the reciver/gateway 
    // should match. The sync word ranges from 0-0xFF. Its purpose is to prevent receiving data from 
    // other LoRa transceivers.
    LoRa.setSyncWord(0xF3);
    Serial.println("LoRa Initializing OK!");
    delay (250); 

  // code that tells the ESP32 what to do, depending on how it is woken up
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); screenSend(); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); logData(); break;
  }
    
  // Increment readingID on every new reading
  readingID++;
  
  // Start deep sleep
  Serial.println("DONE! Going to sleep now.");
  esp_deep_sleep_start();
   
}

void loop() {
  // The ESP32 will be in deep sleep, so it never reaches the loop. Though empty (no code between brackets), 
  // the loop function is still needed for the sketch to compile.
  }


//---FUNCTIONS CALLED ON IN SETUP-----

//Function that sets the external clock (DS3231) time
void setDateTime(){
  Wire.begin(); //change the values below to match current time
  byte second =      1; //0-59
  byte minute =      46; //0-59
  byte hour =        9; //0-23
  byte weekDay =     4; //1-7
  byte monthDay =    9; //1-31
  byte month =       2; //1-12
  byte year  =       22; //0-99

  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(zero); //stop Oscillator

  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(weekDay));
  Wire.write(decToBcd(monthDay));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));

  Wire.write(zero); //start 

  Wire.endTransmission();

   }
  // Convert normal decimal numbers to binary coded decimal
  byte decToBcd(byte val){
    return ( (val/10*16) + (val%10) );
  }

  // Convert binary coded decimal to normal decimal numbers
  byte bcdToDec(byte val)  {
    return ( (val/16*10) + (val%16) );
  }

// Function that saves sensor values on the micro SD card and then, for online viewing, transmits
// them to a receiving unit via the LoRa transceiver.
void logData() {
  //Start by getting the time from the DS3231 clock for time-stamping of data
  //saved to the SD card.
  sensors.begin();
  Wire.begin();
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(zero);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);

  int second = bcdToDec(Wire.read());
  int minute = bcdToDec(Wire.read());
  int hour = bcdToDec(Wire.read() & 0b111111); //24 hour time
  int weekDay = bcdToDec(Wire.read()); //0-6 -> sunday - Saturday
  int monthDay = bcdToDec(Wire.read());
  int month = bcdToDec(Wire.read());
  int year = bcdToDec(Wire.read());

  //print the date and time (e.g., 3/1/11 23:59:59)
  Serial.print(month);
  Serial.print("/");
  Serial.print(monthDay);
  Serial.print("/");
  Serial.print(year);
  Serial.print(" ");
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.println(second);

  //Get readings from the moisture and temperature sensors
  float moistureValue = analogRead(soilPin); //reads soil moisture in millivots
  delay(10);
  Serial.print("Soil moisture: ");
  Serial.println(moistureValue);
  moisturePercent = map(moistureValue, dryValue, wetValue, 0, 100); //converts millivolts to percent moisture
  sensors.requestTemperatures();
  float SoilTemp = sensors.getTempCByIndex(0); //if you want temperature in degrees Fahrenheit, type as
                                               // "float SoilTemp = sensors.getTempFByIndex(0);"
  
  // Assemble content (the reading number, time/date, and readings) to save on SD card
  dataMessage = String(readingID) + "," + String(monthDay) + "/" + String(month) + "/" + String(year) + "," + 
                String(hour) + ":" + String(minute) + "," + String(SoilTemp) + "," + String(moistureValue) + "," +
                String(moisturePercent)+ "\r\n"; 

  // Save (append) data to the data.text file previously written to the SD card.
  // This is done without erasing previously saved data appended to the data.txt file.
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str()); //

  // Transmit data to a receiving microcontroller for online viewing
  const int Key = 4252;  //insert a unique combination of numbers to serve as a key that,in addition
                         //to the sync word, I have found is needed to prevent the receiver from 
                         //processing data from other signals in the area broadcasting on the same 
                         //frequency as this transmitting unit (node); the receiver, then, is programmed
                         //to only publish data packets containing this number key. 
   
  LoRa.beginPacket();
  LoRa.print (Key);
  LoRa.print (":");
  LoRa.print (SoilTemp);
  LoRa.print (":");
  LoRa.print (moistureValue);
  LoRa.print (":");
  LoRa.print (moisturePercent);
  LoRa.endPacket(); 
  delay(250);
 
}

// Function that displays data on the OLED screen
void screenSend()
{
  // Initialize the OLED display
  sensors.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //use something other than 0x3C if address differs
  display.clearDisplay(); //clear the buffer
  display.setTextColor(WHITE);
  
  display.clearDisplay(); // clear display

  // Get temperature and moisture readings from the sensors. 
  sensors.requestTemperatures(); 
  float SoilTemp = sensors.getTempCByIndex(0); // if you want temperature in degrees Fahrenheit, type as
                                               // "float SoilTemp = sensors.getTempFByIndex(0);"
  moistureValue = analogRead(soilPin);  //put Sensor insert into soil
  Serial.println(moistureValue);
  moisturePercent = map(moistureValue, dryValue, wetValue, 0, 100);
    
  // Display temperature in degrees Celsius
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Soil temperature: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(SoilTemp);
  display.print(" ");
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  display.print("C"); //Replace "C" with "F" if you want to display Fahrenheit 
  
  // display moisture
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Soil moisture: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(moisturePercent);
  display.print(" %"); 
  
  display.display();
  delay(2000); //display for 2 seconds; if, for example, you want to be able to see the 
               //data on the screen for 3 seconds, replace "2000" with "3000"
  display.clearDisplay(); // clear display
  display.ssd1306_command(SSD1306_DISPLAYOFF); //so the OLED does not stay on, which would 
                                               //consume too much battery power
}

// Function that writes the data.txt file to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Function that appends data to the data.txt file on the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
