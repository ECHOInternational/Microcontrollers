# Soil Moisture and Temperature Logger

## What these sketches do

With this code, an ESP32 stays in deep sleep mode unless woken up to take soil temperature and moisture readings. This is done to conserve battery power. The microcontroller wakes up with a push button or at a specified interval of time via the internal real time clock (rtc).  When the push button is pressed, the ESP32 wakes up to display soil moisture and temperature values on an OLED screen for in-field viewing at any time. At the specified time interval, which you can change in the sketch, the ESP32 wakes up to save the moisture and temperature values on a microSD card and send the data to an ESP8266 gateway to be published on the Adafruit IO web platform for viewing on a computer or mobile phone with internet access. 

To accomplish the above, we need a transmitter (the ESP32) and a receiver (the ESP8266). If your field or garden is within range of WiFi, there would be no need for two microcontrollers, since the ESP32 has WiFi capability and could publish sensor data to the internet. This soil logger, however, is designed to work in situations where the field or garden is up to 2 km away from a WiFi signal; trees or buildings between the transmitter and receiver will reduce the distance over which the data will be received. The transmitter (also called a node), placed in the field, consists of an ESP32 connected to sensors, a microSD card module, an OLED display screen, and a LoRa RFM95W transceiver programmed to transmit data. The receiver consists of an ESP8266 connected to a LoRa RFM95W transceiver programmed to receive data. 

The sketch can be modified depending on how you want to access data. For instance, if online viewing is not a priority, or is simply not available, you could omit the ESP8266 (receiver) and LoRa transceiver portions of the build and remove aspects of the transmitter code that relate to LoRa.

## Hardware list
*	Two microcontrollers: ESP32 (Wemos Lolin 32) and ESP8266 (NodeMCU 12-E)
*	Micro USB cable for connecting the microcontrollers to your computer to program them
*	2- pin battery connector wire for the Lolin ESP32 (JST PH-2, 2.0 mm connector)
*	Two LoRa RFM95W radio transceivers
* Two 3.7 volt 18850 lithium-ion batteries
* Two single cell, 18650 battery holders with wire leads
*	A waterproof DSB1820 Dallas temperature sensor (these require a 4.7K resistor, but you can purchase the sensor with a module that already has the resister built in)
*	A capacitive soil moisture sensor
*	An OLED display screen 128 X 68 pixels in size
*	Fabrication items: 
    -	 Wires for making connections (22 to 24 awg [gauge; standard unit for wire thickness])
    -	Solderless breadboard for testing 
     -	Pin headers and metal circuit board, such as solderable breadboard, for making wiring connections permanent
     -	Wood and/or plastic enclosure for weather proofing each of the two microcontrollers

## Information on getting started and project components
Below are links to information for getting started with microcontrollers, as well as to information regarding components for this project:
### Getting started
Arduino IDE software: https://www.arduino.cc/en/software

Getting started with Arduino products: https://www.arduino.cc/en/Guide

Installing the ESP32 Board in Arduino IDE: https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

Installing the ESP8266 Board on Arduino IDE: https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/

### Components
ESP32 with LoRa using Arduino IDE- Getting Started: https://randomnerdtutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/

ESP32 WeMos Lolin32 Pinout: https://www.mischianti.org/2021/02/21/esp32-wemos-lolin32-high-resolution-pinout-and-specs/

Insight into ESP8266 NodeMCU Features and Using It with Arduino IDE: https://lastminuteengineers.com/esp8266-nodemcu-arduino-tutorial/

Capacitive Soil Moisture Sensorr: https://arduino-tutorials.net/tutorial/capacitive-soil-moisture-sensor-arduino

Arduino Temperature Sensor: https://arduinogetstarted.com/tutorials/arduino-temperature-sensor

### Testing and fabrication
Breadboards for Beginners: https://learn.adafruit.com/breadboards-for-beginners/diy-jumper-wires?view=all

Beyond Prototyping: https://docs.particle.io/tutorials/learn-more/beyond-prototyping/ 

##Transmitter (node) sketch
### What this sketch does
The Wemos Lolin ESP32 microcontroller stays in sleep mode unless woken up to collect soil temperature and moisture readings. It wakes up by 1) a push button or 2) at a specified interval of time tracked by the internal real time clock during sleep. As mentioned in the introduction, pressing the push button displays the data on an OLED screen; otherwise data is saved to an SD card or transmitted to a receiver for online viewing.
### Power supply
The Wemos Lolin32 has a battery connector and an internal voltage regulator designed for power supplied by 3.7 volt batteries. I am using  two 3.7 volt 18650 lithium ion batteries connected in parallel (to keep the supply voltage at 3.7 volt; if they were connected in series, voltage would double). Replace the batteries when they run out (after about two weeks) or charge them with a TP4056 charger connected to a 6-volt solar panel.
### Wiring connections:
The diagram below shows breadboard connections between the ESP32, the two sensors, and the push button. This is shown to give you an idea of how to place items on a breadboard for testing. A full-size breadboard will accommodate other components as well. The tables below the diagram indicate pin connections for all parts of the circuit. 








