/*
Code assembled by Tim Motis, gathered from the following sources:
  Libraries for the LoRa RFM95 transceiver and SHT31D sensor had to be adapted to work with the ATTINY85. Sources for those adaptations are:
   
    T Zindove (2022) who explained how to adapt the header (h) and cpp files of the Sandeep LoRa library for the ATTINY85: https://www.youtube.com/watch?v=amkT5NtOgWc 
    Wolfgang Ewald (2022) who explained how to adapt the header (h) and cpp files of the Grove_SHT31_Temp_Humi_Sensor from Seeed Studio: https://wolles-elektronikkiste.de/en/tinywirem-vs-wire-for-attinys    

Code related to sleep functions: https://www.mischianti.org/2021/12/11/lineameteostazione-technical-guide-master-device-sending-and-collecting-data-1/ 
Code related to using the watchdog timer to set the sleep interval: https://learn.sparkfun.com/tutorials/h2ohno/all

*/


// LIBRARIES

#include <TinyLoRa.h>
#include <TinyWireM.h>
#include <TinySHT31.h>
#include <avr/sleep.h>
#include "PinChangeInterrupt.h"header (h) and cpp files of

// DEFINITIIONS OF PINS AND VARIABLES

//Watchdog counter
volatile int watchdog_counter;

//This runs each time the watch dog wakes us up from sleep
ISR(WDT_vect) {
  watchdog_counter++;
}

//Temperature sensor
SHT31 sht31 = SHT31();

// #### SETUP ####################################################

void setup ()
{
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
    
    //sleepNow();
    watchdog_counter = 0;
   
    //Serial.begin(9600);
   
    TinyWireM.begin();
         
    // ---- Temperature sensor -------------------
    sht31.begin();
    sht31.heater(false); // heater off, with "true" it's on -> can remove condensated water, 
                       // but gives false temperature values (about 3 degrees too high);
         
   // ---- LoRa transceiver -------------------
    if (!LoRa.begin(915E6)) {
      while (1);
    }
    LoRa.setSyncWord(0xF3);
    //replace the LoRa.begin(---E-) argument with your location's frequency 
    //433E6 for Asia
    //866E6 for Europe
    //915E6 for North America
    //Serial.println(".");
      delay(300);
    
    // Change sync word (0xF3) to match the receiver to avoid receiving messages from other LoRa transceivers
    // ranges from 0-0xFF
    //LoRa.setSyncWord(0xF3);
    //Serial.println("LoRa Initializing OK!");
    LoRaSend();
    delay (600); 

    // ---- Self-tests complete --------------
    // Give us some visual indication that everything is okay. If the LED light keeps blinking, check wiring (more often than not, the problem relates to 
    // incorrect wiring of the SD card module, a non-formatted SD card, or no SD card in the SD card module
    //ledBlink(ledPin, 5, 500, 100, 500, 100);
    setup_watchdog(9); //Wake up after 4 seconds
}

// #### Main loop ################################################

void loop ()
{  
    LoRa.sleep();
    sleepNow();  
    // I was woken by either a bucket tip or the watchdog timer. Either way, I'm awake.;
   
  
   if(watchdog_counter > 400)
   {
    watchdog_counter = 0;
    LoRaSend();
    
  } 
   
}

// FUNCTIONS CALLED IN SETUP OR THE LOOP

void LoRaSend() {

   float Temp = sht31.getTemperature();
   float Hum = sht31.getHumidity();
   delay(1000);
   //float finalVolt = voltMeasure();
   const int Key = 9874;
   
   LoRa.beginPacket();
   LoRa.print (Key);
   LoRa.print (":");
   LoRa.print (Temp);
   LoRa.print (":");
   LoRa.print (Hum);
   LoRa.endPacket(); 
   delay(600);

}


void sleepNow() 
{
  /* Now is the time to set the sleep mode. In the Atmega8 datasheet
     http://www.atmel.com/dyn/resources/prod_documents/doc2486.pdf on page 35
     there is a list of sleep modes which explains which clocks and
     wake up sources are available in which sleep modus.
     In the avr/sleep.h file, the call names of these sleep modus are to be found:
     The 5 different modes are:
         SLEEP_MODE_IDLE         -the least power savings
         SLEEP_MODE_ADC
         SLEEP_MODE_PWR_SAVE
         SLEEP_MODE_STANDBY
         SLEEP_MODE_PWR_DOWN     -the most power savings
     For now, we want as much power savings as possible, so we choose the according
     sleep modus: SLEEP_MODE_PWR_DOWN. 
  */
  
  // disable ADC
  ADCSRA = 0;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

  sleep_enable();          // enables the sleep bit in the mcucr register
  // so sleep is possible. just a safety pin

  sleep_mode();            // here the device is actually put to sleep!!
  // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP
  sleep_disable();         // first thing after waking from sleep:
  // disable sleep...
}

void setup_watchdog(int timerPrescaler) {

  if (timerPrescaler > 9 ) timerPrescaler = 9; //Correct incoming amount if need be

  byte bb = timerPrescaler & 7; 
  if (timerPrescaler > 7) bb |= (1<<5); //Set the special 5th bit if necessary

  //This order of commands is important and cannot be combined
  MCUSR &= ~(1<<WDRF); //Clear the watch dog reset
  WDTCR |= (1<<WDCE) | (1<<WDE); //Set WD_change enable, set WD enable
  WDTCR = bb; //Set new watchdog timeout value
  WDTCR |= _BV(WDIE); //Set the interrupt enable, this will keep unit from resetting after each int
}
