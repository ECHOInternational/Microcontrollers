# Wireless, Radio-Based Air Temperature and Humidity Monitor: with ATTINY85

## What these sketches do

This project was developed to wirelessly monitor air temperature and relative humidity in situations that require long battery life. The ATTINY85 is a small microcontroller chip with no power-consuming peripherals such as voltage regulators (see photo below). More information about the ATTINY85 is here: https://www.utmel.com/components/attiny85-microcontroller-pinout-datasheet-and-programming?id=157. Note that there are 8 pins on the ATTINY85 with pins 1 to 4 on one side and pins 5 to 8 on the other side. A small indented circle on the top left corner of the ATTINY85 marks Pin 1.


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
As mentioned earlier, the transmitter device consists of an ATTINY85, a LoRa transceiver, and an SHT31D temperature/humidity sensor. Note that the LoRa transceiver is not "breadboard friendly"; the hole spacing does not match that of standard printed circuit boards used in Arduino projects. I used an approach shown here: https://linux.livorno.it/2976/modulo-rfm95w-lora/. Another option is here: https://www.tindie.com/products/lps/rfm95-breakout-board/?pt=ac_prod_search

https://randomnerdtutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/

### Wiring

#### LoRa Transciever

| LoRa RFM95     |       | ATTINY85   |
| :---:          | :---: | :---:          |
| GND            |  ---> | GND (Pin 4)    |
| VCC/VIN        |  ---> | VCC (Pin 8)    |
| SCK            |  ---> | SCK (Pin 7)    |
| MISO           |  ---> | MOSI (Pin 5)   |
| MOSI           |  ---> | MISO (Pin 6)   |
| CS/NSS         |  ---> | PB4 (Pin 3)    |


Temperature/humidity sensor
|SHT31 sensor|   	  |ATTINY85    |
|:-----------|:----:|:---------: |
|GND	       |--->	|GND (Pin 4) |
|VCC	       |--->	|VCC (Pin 8) |
|SCL	       |--->	|SCL (Pin 7) |
|SDA	       |--->	|SDA (Pin 5) |

Battery holder: if using two AA batteries, they can be housed in a battery holder that holds two batteries, or in two single AA battery holders. Either way, on one end of the device, connect the positive end of one battery to the negative end of the other battery. This will mean that your batteries are connected in series (as opposed to parallel). When connected in series, the voltage of each battery is summed. Since the voltage of a new/fully charged AA battery is about 1.3 (for recharegable batteries; even though they are labeled as having 1.2 v) or 1.6 (for non-rechargeable batteries; even though they are labeled as having 1.5 v) volts, the power supplied to the ATTINY85 will be 2.6 volts (1.3 + 1.3) with new/fully charged rechareable batteries or 3.2 v (1.6 + 1.6) with non-recharceable batteries. On the other end of the battery holder (opposite from the end where you connected the positive end of one battery to the negative end of the other battery), connect the positive terminal of the battery holder to the power pin (VCC; Pin 8) of the ATTINY85 and the negetavie terminal of the battery holder to the ground pin (GND; Pin 4) of the ATTINY85.

