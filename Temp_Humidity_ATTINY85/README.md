# Wireless, Radio-Based Air Temperature and Humidity Monitor: with ATTINY85

## What these sketches do

This project was developed to wirelessly monitor air temperature and relative humidity in situations that require long battery life. The ATTINY85 is a small microcontroller chip with no power-consuming peripherals such as voltage regulators (see photo below). More information about the ATTINY85 is here: https://www.utmel.com/components/attiny85-microcontroller-pinout-datasheet-and-programming?id=157


![ATTINY85](https://user-images.githubusercontent.com/69003593/225685170-52b94bbe-28e1-4673-9ed5-e76df751e9f8.jpg)

The small size of the ATTINY makes it a good option for building a device that is small enough to place in small containers, as would be the case in monitoring desiccant effectiveness in a jar of seeds. Wireless functionality is acheived with a LoRa (long range) transceiver that serves as a radio signal transmitter. The LoRa transmitting device, pictured below, transmits temperature and humidity data from a small sensor (SHT31) sensor. The ATTINY85 is programmed to send data every hour. In between transmissions it will be in sleep mode. Powered with two AA batteries (a commonly available size), the device consumes just a few microamps of current. 


![TempHumATTINY_TransmitterTop](https://user-images.githubusercontent.com/69003593/225691424-4bde27cd-b45f-4fc3-bdb3-dc2a22a991d0.jpg)  ![TempHumATTINY_Bottom](https://user-images.githubusercontent.com/69003593/225691467-717c4945-711c-447a-9483-ea8feb2ef507.jpg)


Another LoRa transceiver is used to receive the radio signal from the transmitting device. The receiving device, pictured below, includes a Wemos D1 Mini microctontroller that connects to the internet and posts the incoming data to a feely avalable, online platform called Thingspeak. 

![TempHumATTINY_Receiver](https://user-images.githubusercontent.com/69003593/225692993-ff026d28-7bd9-458d-92c6-1cf1e577c1b2.jpg)

## Hardware list
* An ATTINY85
* A Sensirion SHT31 sensor
* A WemosD1 Mini 
* Two LoRa transceiver radios (this programming works with RFM95 radios; select a version capable of transmitting at a licence-free frequency for your country
* An OLED display (128 X 64 pixels)
* Wires (e.g. 24 gauge thickness wire)
* Optional parts for soldering on a circuit board include protaboard, pin headers, and an 8-pin DIP socket (so that you can pull out/replace the ATTINY85 sensor as needed)
