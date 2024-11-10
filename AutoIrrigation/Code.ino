/* Code assembled from various open sources by Tim Motis

Much of the code was drawn from: https://www.makerguides.com/capacitive-soil-moisture-sensor-with-arduino/
Another source was: https://lastminuteengineers.com/soil-moisture-sensor-arduino-tutorial/#:~:text=Arduino%20Code,-The%20sketch%20below&text=%3C%20500%20is%20too%20wet,dry%20enough%20to%20be%20watered
*/

//.....LIBRARIES AND GLOBAL VARIABLES............

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Arduino.h>
#include <LowPower.h>  //for sleep functionality

const uint16_t sleepSeconds = 7200; //seconds of sleep time betweeen readings; there are 3600 seconds per hour so this is set for two hours between moisture checks

//Initialization of soil moisture sensor
  const int soilPin = A1; //analog output pin of soil moisture sensor connected to pin A1
  const int soilPowerPin = 3;  //connect VCC of moisture sensor to this pin (D3) on Arduino Nano, which will be turned on (HIGH) or off (LOW) to conserve power
  const int dryValue = 825;   //millivolts in dry soil (replace with value for your soil)
  const int wetValue = 425;  //millivolots in wet/saturated soil (replace with value for your soil)
  int moistureValue = 0; //initial value needs to be zero; do not change
  int moisturePercent=0; //initial value needs to be zero; do not change
  const int wetSoilPct = 80; //wet soil moisture percentage set to achieve the best level of soil moisture for your scenario
  const int drySoilPct = 65; //dry soil moisture percentage set to achieve the best level of soil moisture for your scenario
  

//Solenoid pin and on time
const int solenoidPin = 5;  //this pin (D5) should be connected to the gate of the mosfet
unsigned long previousMillis = 0UL;
unsigned long interval = 5000UL;  //1000 UL per sec for each second of on time


// Define OLED display parameters for a 128 x 64 pixel screen
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int PushButtonPin = 3;

//const int ledPin = 5;
volatile bool ButtonPushDetected = false;

void setup() {
 //Start serial monitor 
 Serial.begin(115200);
 while(! Serial); // Wait for serial monitor to open
 
 //initialize soil power pin, solenoid power pin, and analog reference (AREF) pin
 pinMode(soilPowerPin, OUTPUT);
 digitalWrite (soilPowerPin, LOW);
 pinMode(solenoidPin, OUTPUT);
 analogReference(EXTERNAL); //to use AREF, for more stable moisture readings,connect AREF to 3.3v on the Arduino Nano
 
 //Send and display soil moisture reading to the OLED screen when the reset button is pressed on the Arduino Nano
 screenSend(); 
 delay(20);
 }

//....MAIN LOOP.............................

void loop() // this code runs repeatedly
 { 
   
 //check the soil moisture and turn on the solenoid to irrigate if needed
 moistureCheck();  

 //go to sleep!
 Serial.println("Going to Sleep...");
 delay(20);
 longSleep(sleepSeconds); //sleepSeconds, defined earlier, sets the amount of sleep time betwen moisture checks


}


//....FUNCTIONS...........................  
void WaterSoil()
{
 digitalWrite (solenoidPin, HIGH);
 Serial.println("Watering");
 delay(30000);
 digitalWrite (solenoidPin, LOW);
 Serial.println("Done watering");
 	 
}

unsigned int moistureCheck()
{
 
 int sum;
 int num_readings =  0;
 int reading1 = 0;
 int reading2 = 0;
 int reading3 = 0;
 int averageReading = 0;
 int totalValue = 0;
 const int minAcceptableReading = 5;  //set what you think might be an anomoly for a low reading
 const int maxAcceptableReading = 1020; //set what you think might be an anoly for a high reading 
 
 digitalWrite(soilPowerPin, HIGH);
 delay(3000); 
 reading1 = analogRead(soilPin);
 if (reading1 > minAcceptableReading && reading1 <= maxAcceptableReading) {
  Serial.print("reading 1: ");
  Serial.println(reading1);
  num_readings++ ;
  } else if (reading1 < minAcceptableReading && reading1 >= maxAcceptableReading) {return 0;}
 
 
 delay(500);
 reading2 = analogRead(soilPin);
 if (reading2 > minAcceptableReading && reading2 <= maxAcceptableReading) {
  Serial.print("reading 2: "); delay(20);
  Serial.println(reading2); 
  num_readings++;
  }else if (reading2 < minAcceptableReading && reading2 >= maxAcceptableReading) {return 0;}
 

 delay(500);
 reading3 = analogRead(soilPin);
 if (reading3 > minAcceptableReading && reading3<= maxAcceptableReading) {
  Serial.print("reading 3: "); delay (20);
  Serial.println(reading3);
  num_readings++;
  } else if (reading3 < minAcceptableReading && reading3 >= maxAcceptableReading) {return 0;}
 
 digitalWrite(soilPowerPin, LOW);
 Serial.print("number of readings = "); delay(20);
 Serial.println(num_readings);
 
 totalValue = reading1 + reading2 + reading3;
 averageReading = totalValue/num_readings;
 num_readings = 0;
 delay(1000);
 
 Serial.print("Total value: ");
 delay(20);
 Serial.println(totalValue);
 delay(20);
 
 Serial.print("Soil moisture: ");
 delay(20);
 Serial.println(averageReading);
 delay(20);
 float moisturePercent = map(averageReading, dryValue, wetValue, 0, 100); //converts millivolts to percent moisture
  Serial.print("Soil moisture percent: ");
  delay(20);
  Serial.println(moisturePercent);
  delay(20);
  
  // Determine status of our soil
  if (moisturePercent > wetSoilPct) {
    Serial.println("Status: Soil is too wet");  //sends a message to the serial port for debugging but does not turn on the solenoid
    delay (20);
  } else if (moisturePercent <= wetSoilPct && moisturePercent > drySoilPct) { //sends a message to the serial port but does not turn on the solenoid
    Serial.println("Status: Soil moisture is perfect");
    delay(20);
  } else {  //if neither of the above conditions (too wet or just right) are true then the soil is dry and needs to be watered
    Serial.println("Status: Soil is too dry - time to water!");
    delay(20);
    WaterSoil(); //function that turns on the solenoid valve

  }
}
  

void longSleep( uint16_t sleepInSeconds )
{
  if ( sleepInSeconds & 0x01 ) LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  if ( sleepInSeconds & 0x02 ) LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
  if ( sleepInSeconds & 0x04 ) LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);

  while ( sleepInSeconds & 0xFFFFFFF8 ) {
    sleepInSeconds = sleepInSeconds - 8;
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}


void screenSend()
{
  // Initialize the OLED display
  digitalWrite(soilPowerPin, HIGH);
  delay(3000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //use something other than 0x3C if address differs
  display.clearDisplay(); //clear the buffer
  display.setTextColor(WHITE);
  
  display.clearDisplay(); // clear display

  // Get  moisture readings from the sensor. 
  
  //delay(2000);
  moistureValue = analogRead(soilPin);  //put Sensor insert into soil
  
  Serial.println(moistureValue);
  delay(20);
  moisturePercent = map(moistureValue, dryValue, wetValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);
  //Serial.print("Moisture check: ");
  //delay(20);
  //Serial.println(moisturePercent);
  //delay(20);  
  
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
  digitalWrite(soilPowerPin, LOW);
}
