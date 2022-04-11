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

## Transmitter (node) sketch
### What this sketch does
The Wemos Lolin ESP32 microcontroller stays in sleep mode unless woken up to collect soil temperature and moisture readings. It wakes up by 1) a push button or 2) at a specified interval of time tracked by the internal real time clock during sleep. As mentioned in the introduction, pressing the push button displays the data on an OLED screen; otherwise data is saved to an SD card or transmitted to a receiver for online viewing.
### Power supply
The Wemos Lolin32 has a battery connector and an internal voltage regulator designed for power supplied by 3.7 volt batteries. I am using  two 3.7 volt 18650 lithium ion batteries connected in parallel (to keep the supply voltage at 3.7 volt; if they were connected in series, voltage would double). Replace the batteries when they run out (after about two weeks) or charge them with a TP4056 charger connected to a 6-volt solar panel.
### Wiring connections:
The diagram below shows breadboard connections between the ESP32, the two sensors, and the push button. This is shown to give you an idea of how to place items on a breadboard for testing. A full-size breadboard will accommodate other components as well. The tables below the diagram indicate pin connections for all parts of the circuit. 

![SoilLoggerSensors](https://user-images.githubusercontent.com/69003593/161762021-66341a91-7dee-48f1-8450-97eaf2a11270.jpg)
#### Capacitive soil moisture sensor
| Soil sensor|       | Lolin ESP32 |
| :---:      | :---: | :---:       |
| GND        |  ---> | GND         |
| VCC/VIN    |  ---> | 3v3         |
| Analog out |  ---> | 35          |

#### Temperature sensor (DS18B20)
| DS18B20    |       | Lolin ESP32 |
| :---:      | :---: | :---:       |
| GND        |  ---> | GND         |
| VCC/VIN    |  ---> | 3v3         |
| DQ (Data)  |  ---> | 32          |

#### Push button
| Push button|       | Lolin ESP32 |
| :---:      | :---: | :---:       |
| Right pin  |  ---> | 15          |
| Left pin   |  ---> | 3v3         |

#### Micro SD card module
| SD card module  |       | Lolin ESP32 |
| :---:      | :---: | :---:       |
| GND        |  ---> | GND         |
| VCC/VIN    |  ---> | 3v3         |
| CS (SS)*   |  ---> | 5           |
| MOSI (DI)* |  ---> | 23          |
| MISO (DO)* |  ---> | 19          |
| CLK (SCK)* |  ---> | 18          |
*Letters in parentheses indicate alternate pin labels, dpending on the supplier.

#### External clock (DS3231 Real Time Clock [RTC]
| DS3231 RTC |       | Lolin ESP32 |
| :---:      | :---: | :---:       |
| GND        |  ---> | GND         |
| VCC/VIN    |  ---> | 3v3         |
| SDA        |  ---> | 21          |
| SCL        |  ---> | 22          |

#### OLED display (128 x 64 pixel size)  
| OLED       |       | Lolin ESP32 |
| :---:      | :---: | :---:       |
| GND        |  ---> | GND         |
| VCC/VIN    |  ---> | 3v3         |
| SDA        |  ---> | 21          |
| SCL        |  ---> | 22          |

#### LoRa transceiver
| LoRa       |       | Lolin ESP32 |
| :---:      | :---: | :---:       |
| GND        |  ---> | GND         |
| VCC/VIN    |  ---> | 3v3         |
| GO/D1      |  ---> | 2           |
| SCK        |  ---> | 18          |
| MISO       |  ---> | 19          |
| MOSI       |  ---> | 23          |
| CS/NSS     |  ---> | 16          |

## Receiver (gateway) sketch
### What this sketch does
This code equips an ESP8266 (NodeMcu 12-E) microcontroller to receive, via a LoRa RFM95 transceiver, LoRa transmitted soil temperature and moisture data from an ESP32 microcontroller in the field. Readings received by the transmitter are uploaded to Adafruit IO and/or Thingspeak web platforms for online viewing on a computer or mobile phone. Coding is included for both web platforms, since a free account can be set up with either. Readings are published online at the time interval specified in code for the transmitter in the field (ESP32 with connected sensors).

### Power supply
Supply power to the ESP8266 at all times, since the microcontroller is always awake to receive readings sent from the field. Place the ESP8266, with the LoRa transceiver connected, in a place where it can be connected to power and protected from rain or extreme temperatures. Power the the ESP8266 by connecting one end of a USB cable to the USB connector on the microcontroller and the other end of the cable to a 5-volt USB wall plug. 

### Wiring connections
Pin numbers defined in the sketch need to match the pins used in connecting the LoRa transceiver to the ESP8266. As with the transmitter, not all of the pins on the LoRa transceiver are used.

#### LoRa transceiver
| LoRa       |       | Lolin ESP32 |
| :---:      | :---: | :---:       |
| GND        |  ---> | GND         |
| VCC/VIN    |  ---> | 3v3         |
| GO/D1      |  ---> | DO          |
| SCK        |  ---> | D5          |
| MISO       |  ---> | D6          |
| MOSI       |  ---> | D7          |
| CS/NSS     |  ---> | D8          |

## Weatherproofing the soil moisture sensor
There is a waterproof version of a capacitive soil moisture: https://www.dfrobot.com/product-2054.html
The electronics of most of the soil moisture sensors being sold, however, will need to be protected in some way. Exposed electronics at the top of the sensor are shown in the photo below:
![SoilMoistureSensor](https://user-images.githubusercontent.com/69003593/162763179-ca2a520e-4cd4-4c60-ba04-f5078f8abecd.jpg)

Some have used epoxy for weatherproofing. An approach I have succeeded with is to 1) pass wires through drip irrigation supply tubing (which serves as a conduit) and 2) place the sensor inside PVC. 

An analog capacitive moisture sensor will have three wires to connect: ground (negative); power (positive); and analog out. So, cut three wires at a length that you will need to go from the ground, where the sensor will be burried, to your microcontroller enclosure. I suggest mounting the enclosure (box of some sort that contains your microcontroller, SD card module, etc.) on a fence post, at a distance above the ground that does not require more than 1 to 2 m of wire. Then pass your three wires through your conduit, as shown in the photos below:

Solder your three wires, at one end, to the end of a wire thick and stiff and long enough to push all the way through the conduit. Take the other end of your push wire (thick/non-stranded piece) and push it through the conduit until that end emerges out the other end of the conduit. Now grasp the emerged end of the push wire and pull it through the conduit. In doing so, you will be able to pull your sensor wires through the conduit. Below is a photo showing the push wire soldered to two sensor wires (this build will require a third wire), and the sensor wires pulled through the conduit via the push wire.
![DripSupplyTubing](https://user-images.githubusercontent.com/69003593/162767086-0098be0e-2c32-454d-a0a9-ddbd19f79837.jpg)

Next, assemble the PVC enclosure. You will a few inches of pipe, just long enough to house the wires, and two end caps with the same inner diameter of the short piece of pipe. Cut one of the end caps with a blade close to the same thickness as that of the moisture sensor. I find that a wood saw or hack saw works well. Cut a slit in one of the PVC end caps that you can push the sensor through. You may need to sand the slit if the opening is too narrow to push the sensor through. The idea is to end up with a tight fit, so that the sensor is held firmly in place, reducing reliance on glue to do that. Below is a photo of an end cap being cut with a hack saw, in a vice, and what the end result should look like:
![CutPVCCap](https://user-images.githubusercontent.com/69003593/162768978-8a934038-ef3d-4fcc-84be-b8d9041e9523.jpg)
![CutPVCCapB](https://user-images.githubusercontent.com/69003593/162769011-1e6be23b-c880-4b29-892f-a3f84bfc0afa.jpg)

Then, on the other of the two end caps, drill a hole through which you can push the conduit (with sensor wires inside) through. Aim for a tight fit; avoid ending up with a hole larger than the diameter of the conduit. With the hole drilled and conduit pushed through it, connect the wires to the sensor as shown below:
![ConnectWires](https://user-images.githubusercontent.com/69003593/162769584-e033a8cf-4eb9-4f56-9db0-0bd990888670.jpg)

The last step, then, is to push the soil moisture sensor through the slit in the end cap that you had cut for that purpose. Then push both end caps onto the PVC pipe and seal everything with silicone glue or putty. I suggest avoiding the use of PVC cement, since you may need to replace the sensor from time to time. Below is a photo showing all the wires inside the PVC, with just the sensor protruding out from one end.
![WaterproofedSensor](https://user-images.githubusercontent.com/69003593/162770475-d96c9eda-3a68-48ce-8a0b-af42ed3bf690.jpg)








