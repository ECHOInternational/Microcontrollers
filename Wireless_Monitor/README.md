# Wireless Air Temperature and Humidity Monitor

## What these sketches do
This project was developed to monitor conditions in a seed storage room that was out of range of WiFi (internet) but close to the main seed banking area that has WiFi. LoRa RFM95 transceivers are therefore used to be able to send data from an Arduino Nano in the seed storage room to an ESP8266/NodeMcu in the main building. Each microcontroller is paired with a LoRa transceiver. If you are monitoring conditions in a building where there is WiFi, you would only need a WiFi-capable microcontroller (e.g., NodeMCE or ESP32). 

The Arduino Nano, placed in the seed storage room, collects hourly temperature and humidity readings via an SHT31 sensor. The main purpose of the monitoring project is to publish data online to be able to receive an email alert if, for example, temperature increases above a threshold that would indicate the cooling system is not working. Since we were not interested in tracking long-term trends, data are not logged to a microSD card.

LoRa radio signals can travel further with no obstructions. In this case, the signal has to pass through the metal walls of the shipping container in which the seeds are stored and through a window of an adjacent building where the receiver is located. Despite the obstructions, readings are rarely missed. 

Incoming data is processed by the receiver to 1) display the data on an OLED screen and 2) post the data to a web platform called Adafruit IO. With an Adafruit IO account you can set up a visual dashboard to display data in real time in (e.g., graphs and gauges, dependding on preference). The Adafruit dashboard can be viewed on a smartphone or computer. Using an online service called If This Then That (IFTTT), a "trigger" was set up to email an alert if the temperature exceeds 60 degrees Fahrenheeit (since the CoolBot device used to cool the room normally keeps the room at 50 degrees F when working propoerly). Instructions for setting up an IFTTT trigger in conjunction with Adafruit IO are here: https://learn.adafruit.com/using-ifttt-with-adafruit-io/ifttt-to-adafruit-io-setup

## Transmitter
