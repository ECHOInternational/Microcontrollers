# Wireless, Radio-Based Air Temperature and Humidity Monitor: with ATTINY85

## What these sketches do

This project was developed to wirelessly monitor air temperature and relative humidity in situations that require long battery life. The ATTINY85 is a small microcontroller chip with no power-consuming peripherals such as voltage regulators (see photo below). More information about the ATTINY85 is here: https://www.utmel.com/components/attiny85-microcontroller-pinout-datasheet-and-programming?id=157. Note that there are 8 pins on the ATTINY85 with pins 1 to 4 on one side and pins 5 to 8 on the other side. A small indented circle (see photo below) on the top left corner of the ATTINY85 marks Pin 1. Pin 4 (ground/GND) is the 4th pin down, on the same side of the ATTINY85 as Pin 1. Pin 8 (power/VCC) is the pin to the right of Pin 1.


![ATTINY85](https://user-images.githubusercontent.com/69003593/225685170-52b94bbe-28e1-4673-9ed5-e76df751e9f8.jpg)

The small size of the ATTINY makes it a good option for building a device that is small enough to place in containers, as would be the case in monitoring desiccant effectiveness in a jar of seeds. Wireless functionality is acheived with a LoRa (long range) transceiver that serves as a radio signal transmitter. The LoRa transmitting device, pictured below, transmits temperature and humidity data from a small sensor (SHT31) sensor. The ATTINY85 is programmed to send data every hour. In between transmissions it will be in sleep mode. Powered with two AA batteries (a commonly available size), the device consumes just a few microamps of current. 


![TempHumATTINY_TransmitterTop](https://user-images.githubusercontent.com/69003593/225691424-4bde27cd-b45f-4fc3-bdb3-dc2a22a991d0.jpg)  ![TempHumATTINY_Bottom](https://user-images.githubusercontent.com/69003593/225691467-717c4945-711c-447a-9483-ea8feb2ef507.jpg)


Another LoRa transceiver is used to receive the radio signal from the transmitting device. The receiving device, pictured below, includes a Wemos D1 Mini microctontroller that connects to the internet and posts the incoming data to a feely avalable, online platform called Thingspeak. The receiver can be located in another room or building where there is electricity. The Wemos D1 Mini used for the receiver is easily powered via a USB cable with a Micro-USB connector on one end (that plugs into the Wemos microcontroller) and a USB-A (standard) connector on the other end that plugs into a 5-volt USB plug adaptor (for plugging into 110 or 220v wall outlets). 

![TempHumATTINY_Receiver](https://user-images.githubusercontent.com/69003593/225692993-ff026d28-7bd9-458d-92c6-1cf1e577c1b2.jpg)

## Hardware list
* An ATTINY85
* An ATTINY85 programmer (I use an Arduino Nano)
* A Sensirion SHT31 sensor
* A WemosD1 Mini 
* Two LoRa transceiver radios (this programming works with RFM95 radios; select a version capable of transmitting at a licence-free frequency for your country
* An OLED display (128 X 64 pixels); I use the LoRa RFM95W model--select one that transmits at a license-free frequency for your country (915 Mhz commonly used for North America; 923 Mhz for Asia; 865 to 867 Mhz for India--see https://www.thethingsnetwork.org/docs/lorawan/frequencies-by-country/).
* Wires (e.g. 24 gauge thickness wire)
* A USB cable for the WeMos D1 Mini receiver: needs to have a Micro-USB connector on one end and a standard USB-A connector on the other
* A USB plug adapter that converts 110v or 220v to the 5v needed to power the WeMos D1 Mini
* Optional parts for soldering on a circuit board include protaboard, pin headers, and an 8-pin DIP socket (so that you can pull out/replace the ATTINY85 sensor as needed)

## Transmitter device
As mentioned earlier, the transmitter device consists of an ATTINY85, a LoRa transceiver, and an SHT31D temperature/humidity sensor. Note that the LoRa transceiver is not "breadboard friendly"; the hole spacing does not match that of standard printed circuit boards used in Arduino projects. I used an approach shown here: https://linux.livorno.it/2976/modulo-rfm95w-lora/. A cleaner option, with less soldering, is here: https://www.tindie.com/products/lps/rfm95-breakout-board/?pt=ac_prod_search.


### Wiring

#### LoRa Transciever
The simplest option for an antenna is a piece of sheathed copper wire, cut to the appropriate length for the license-free frequency band in your country. A length of 7.8 cm (3 in) is the correct length for a frequecy of 915 Mhz. For lenths for other frequencies, see the web page found here: https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts/assembly

| LoRa RFM95     |       | ATTINY85   |
| :---:          | :---: | :---:          |
| GND            |  ---> | GND (Pin 4)    |
| VCC/VIN        |  ---> | VCC (Pin 8)    |
| SCK            |  ---> | SCK (Pin 7)    |
| MISO           |  ---> | MOSI (Pin 5)   |
| MOSI           |  ---> | MISO (Pin 6)   |
| CS/NSS         |  ---> | PB4 (Pin 3)    |


#### Temperature/humidity sensor
|SHT31 sensor|   	  |ATTINY85    |
|:-----------|:----:|:---------: |
|GND	       |--->	|GND (Pin 4) |
|VCC	       |--->	|VCC (Pin 8) |
|SCL	       |--->	|SCL (Pin 7) |
|SDA	       |--->	|SDA (Pin 5) |

#### Battery holder
If using two AA batteries, they can be housed in a battery holder that holds two batteries, or in two single AA battery holders. Either way, on one end of the device, connect the positive end of one battery to the negative end of the other battery. This will mean that your batteries are connected in series (as opposed to parallel). When connected in series, the voltage of each battery is summed. Since the voltage of a new/fully charged AA battery is about 1.3 (for recharegable batteries; even though they are labeled as having 1.2 v) or 1.6 (for non-rechargeable batteries; even though they are labeled as having 1.5 v) volts, the power supplied to the ATTINY85 will be 2.6 volts (1.3 + 1.3) with new/fully charged rechareable batteries or 3.2 v (1.6 + 1.6) with non-recharceable batteries. On the other end of the battery holder (opposite from the end where you connected the positive end of one battery to the negative end of the other battery), connect the positive terminal of the battery holder to the power pin (VCC; Pin 8) of the ATTINY85 and the negetavie terminal of the battery holder to the ground pin (GND; Pin 4) of the ATTINY85. 

The ATTINY85 can be supplied with a voltage range of 1.8 to 5.5 volts, giving flexibility in terms of battery options. Instead of AA batteries, you could power it with a single 18650 battery, which would supply 3.7 v. The AAs are more widely available than 18650s. 

### Programming
You will need a way to program the ATTINY85. I use an Arduino Nano, but you can also use an Arduino Uno or a USB option by Digispark. There are many YouTube videos on how to do this. When using an Arduino to connect the ATTINY to your computer and program it, you may get a "programmer required" error when you try to upload the sketch. This happens with newer versions (e.g. 2.03) of Arduino IDE software. If this happens, navigate to the "Sketch" tab of your Arduino IDE screen and click on the option that says "Upload using programmer."

The LoRa functionality is based on the sandeepmistry library found here: https://github.com/sandeepmistry/arduino-LoRa. It requires some modification to work with the ATTINY85, as explained by T Zondove (2022) here: https://www.youtube.com/watch?v=amkT5NtOgWc. Add the sandeepmistry library to your Arduino library folder, rename it as "TinyLoRa," navigate to the "src" folder, and replace the ".h" and ".cpp" files with the two that are in the Temp_Humidity_Monitor_ATTINY85/Libraries/TinyLoRa folder in this github repository. Those files reflecct all of the modifications presented by T Zondove. Make sure the files are named "TinyLoRa.cpp" and "TinyLoRa.h"; otherwise, Arduino IDE will not be able to find the TinyLoRa library.

Functionality related to the SHT31D temperature sensor is based on the library called Grove_SHT31_Temp_Humi_Sensor by Seeed-Studio, found here: https://github.com/Seeed-Studio/Grove_SHT31_Temp_Humi_Sensor. It requires modification of the ".h" and ".cpp" files, as explained by Wolfgang Ewald (2022) 
in TinyWireM vs Wire for ATtinys found here: https://wolles-elektronikkiste.de/en/tinywirem-vs-wire-for-attinys. Add the Grove SHT31 library to your Arduino library folder, open the resulting Grove SHT31 library folder, and replace the ".cpp" and ".h" files with the two that are in the Temp_Humidity_Monitor_ATTINY85/Libraries/TinySHT31 folder in this github repository. Those files reflecct all of the modifications presented by W. Ewald.

Pay attention to the LoRa frequency in the transmitter sketch. Use a licence-free frequency band for your country. 

## Receiver Device
The receiver makes use of a LoRa RFM95 transceiver to receive temperature and humidity data from the transmitter. The LoRa module is wired to a Wemos D1 Mini microcontroller, which then posts the data to an online platform called Thingspeak. You will need to set up a free Thingspeak channel, following instructions given here: https://thingspeak.com/. Once there, click on the "Get Started for Free" tab. This will give you a "Write API Key" needed in the code. For screenshots of how to set up ThingSpeak see the "Getting Thingspeak API Ready" section of the website found here:     https://microcontrollerslab.com/esp8266-nodemcu-send-sensor-readings-thingspeak-arduino/ 

### Wiring 
The LoRa RFM95 radio/transceiver is designed for a 3.3 volt power supply. Be sure to connect the power (VCC) of the radio to the 3.3 volt pin of the Wemos; do NOT connect it to the 5 volt pin on the WeMos, as this will likely damage the radio. Solder an antenna as explained for the transmitting device. The transmitter and receiver both need to operate at the same frequency.

| LoRa           |       | WeMos D1 Mini  |
| :---:          | :---: | :---:          |
| GND            |  ---> | GND            |
| VCC/VIN        |  ---> | 3v3            |
| GO/Dio0        |  ---> | D1             |
| SCK            |  ---> | D5             |
| MISO           |  ---> | D6             |
| MOSI           |  ---> | D7             |
| CS/NSS         |  ---> | D8             |

### Programming
You will need to set up a free Thingspeak channel, following instructions given here: https://thingspeak.com/. Once there, click on the "Get Started for Free" tab. This will give you a "Write API Key" needed in the code. For screenshots of how to set up ThingSpeak see the "Getting Thingspeak API Ready" section of the website found here:     https://microcontrollerslab.com/esp8266-nodemcu-send-sensor-readings-thingspeak-arduino/.

You will also need your WiFi credentials in order for the WeMos to connect to WiFi. Before uploading the Receiver sketch to the WeMos, look at the comments towards the top of the sketch to find where you need to input your WiFi credentials. 

Note in the LoRa parts of each sketch that both the Transmitter and Receiver sketch ask for a 4-digit key. That key needs to be exactly the same. Its purpose is to only post online the data that come from your transmitter, avoiding unwanted data from stray radio signals.

Pay attention to the LoRa frequency in the receiver sketch. It needs to match the frequency selected in the transmitter sketch.






