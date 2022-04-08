# Weather Station
## What these sketches do
Coding for this weather station enables the monitoring of rainfall, air temperature, and relative humidity. Hourly readings are saved to a MicroSD card and transmitted to a receiving unit that publishes the data to a web platform called Thingspeak. Thingspeak enables real-time monitoring of incoming data in the form of graphs and/or gauges. 

The weather station itself is out of range of WiFi (internet). Thus the data are sent via long range radio (LoRa) to a receiving device located in a building with internet access.

## Hardware list
* An Arduino Nano microcontroller that collects and transmits data
* An ESP8266/NodeMCU that receives incoming data and posts it to Thingspeak
* Appropriate USB cables for each of the micronctollers
* A DS3231 real time clock 
* A 5-volt compatible MicroSD card module and card
* Two LoRa RFM95x transceivers (look for one that transmits using a licence-free frequency [these vary by country: search LoRa frequencies by country for more info])
* A tipping bucket rain sensor with a reed switch; the station at ECHO uses an old RainWise sensor, but you could also purchase an inexpensive Misol sensor sold as a replacement part for weather stations; the long length of wire it comes with is already connected to the reed switch inside, and you can cut the connector off at the end to access the two wires.
* Hardware debouncing parts (there is some code for software debouncing, making these parts optional but a potentially significant improvement)
  * A Schmitt trigger (SN74LS14N) that operates at 5 volts: https://www.aliexpress.com/item/1005003483691039.html?spm=a2g0o.productlist.0.0.3cb92cecWMwbcT&algo_pvid=50ee4675-446e-48ef-a2df-707a0f9ed6e6&algo_exp_id=50ee4675-446e-48ef-a2df-707a0f9ed6e6-5&pdp_ext_f=%7B%22sku_id%22%3A%2212000025991879251%22%7D&pdp_pi=-1%3B0.76%3B-1%3B-1%40salePrice%3BUSD%3Bsearch-mainSearch
  * A 10 Kohm resistor  (resister kits are available with an assortment of resisters you might need for this or other projects)
  * A 100 ohm resistor
  * A 10 uf capacitor
  * See this video for information on the benefits of and how to wire a Schmitt trigger into your rain gauge circuit: https://www.youtube.com/watch?v=KHrTqdmYoAk&t=872s
* Power supply
  * Two 6-volt solar panels that each produce 800 to 1000 milliamps
  * A solar charger: I have had good success with a version of the TP4056 charger that steps up voltage from 3.7 volts to 5 volts (which the Arduino Nano needs); it is 
    found here: https://www.amazon.com/gp/product/B08JGDVMM6/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&th=1  and
                https://www.aliexpress.com/item/4000087101515.html?spm=a2g0o.productlist.0.0.70975ffdFQOVok&algo_pvid=c92fc316-1fc7-4482-a170-65c9f3ff0f4c&algo_exp_id=c92fc316-1fc7-4482-a170-65c9f3ff0f4c-1&pdp_ext_f=%7B%22sku_id%22%3A%2210000000232313748%22%7D&pdp_pi=-1%3B0.54%3B-1%3B-1%40salePrice%3BUSD%3Bsearch-mainSearch
  * Four 18650 3.7 volt lithium ion batteries
  * A battery holder for the above batteries that allows them to be connected in parrallel (so that the voltage coming out of the battery holder is close to 3.7 volts)
* Fabrication items: wires, solder, materials you may have on hand to build an enclosure for the Arduino Nano  

## Transmitter
This consists of an Arduino Nano with sensors (a rain and temperature/humidity sensor) and other components (RTC clock, MicroSD card, and LoRa transceiver) connected to it. 
### Wiring
#### Temperature/humidity sensor
| SHT31 sensor   |      |  Arduino Nano   |
| :---:          |:---: |  :---:          |
| GND            | ---> |  GND            |
| VCC            | ---> |  5v             |
| SDA            | ---> |  A4             |
| SCL            | ---> |  A5             |

### External real-time clock (RTC)
| DS3231 RTC     |      |  Arduino Nano   |
| :---:          |:---: |  :---:          |
| GND            | ---> |  GND            |
| VCC            | ---> |  5v             |
| SDA            | ---> |  A4             |
| SCL            | ---> |  A5             |

#### LoRa transceiver
| LoRa           |       | Arduino Nano   |
| :---:          | :---: | :---:          |
| GND            |  ---> | GND            |
| VCC/VIN        |  ---> | 3v3            |
| GO/D1          |  ---> | 9              |
| SCK            |  ---> | 13             |
| MOSI           |  ---> | 11             |
| MISO           |  ---> | 12             |
| CS/NSS         |  ---> | 10             |

#### MicroSD card module
| Card module    |       | Arduino Nano   |
| :---:          | :---: | :---:          |
| GND            |  ---> | GND            |
| VCC/VIN        |  ---> | 3v3            |
| CS/SS          |  ---> | 4              |
| MOSI/DI        |  ---> | 11             |
| MISO/DO        |  ---> | 12             |
| CLK/SCK        |  ---> | 13             |

#### Reed switch 
A reed sitch consists of a mechanical swtich enclosed in glass, with a short leg of wire coming out either side of the glass. Some switches come with the glass enclosed in plastic to make it less fragile. With mechanical swtiches, electrical pulses "bounce" for a time resulting in electrical "noise" as the contacts open and close. Debouncing enables the microntroller to reliably count pulses, preventing it from detecting more tips of the tipping buckets than there really are. Software debouncing code tells the microcontroller to wait for a fraction of a second after a tip (of the rain gauge tipping bucket) to count the pulse; this way a tip count is only registered until after the "noice" of the resulting pulse has passed. Hardware debouncing uses a Schmitt trigger and capacitor to filter the noise, creating a well-defined pulse that the microcontroller can easily recognize and count. With hardware debouncing, the code is simplified because the microntoller does not have to track the time after a tip is detected. The transmitter sketch is written for hardware debouncing, but alternate code is included in case you want to simplify your build by omitting the Schmitt trigger. In my opinion, the Schmitt trigger adds a level of complexity to the build, but also adds a degree of reliabiliity and reduces demand on the microcontroller to track time. 

For software debounding, connect the wire on one side of the glass to ground,  and the other to a pin on the arduino designated in the sketch as "reedPin." It does not matter which side of the swtich is connected to ground. See this website for a diagram of how you would connect a reed switch without going through a Schmitt trigger: https://learn.sparkfun.com/tutorials/reed-switch-hookup-guide/all#:~:text=Just%20as%20your%20magnet%20may,determining%20how%20the%20switch%20activates. To apply what is seen in the Sparkfun tutorial to the transmitter sketch, connect the reed switch as follows:

| Reed swtich    |      |  Arduino Nano   |
| :---:          |:---: |  :---:          |  
| Left side      | ---> |  GND            |     
| Right side     | ---> |  2              |      
| Left side      | ---> |  GND            |   


For hardware debouncing with a Schmitt trigger, connect one side of the reed swtich to a ground pin of the Arduino, as shown in the above table, or two the ground rail of the breadboard. The other end of the reed switch will be routed between the two resistors of the Schmitt trigger set up shown in this video: https://www.youtube.com/watch?v=KHrTqdmYoAk&t=872s  (the latter third of the video discusses Schmitt triggers and contains a useful diagram of the circuit). The trigger signal wire, shown in the diagram in the video and coming off the Schmitt trigger, will connect to pin 2 of the arduino.

#### Light Emitting Diode (LED) pin
The transsmitter sketch makes use of a small LED light that blinks as the tipping buckets rock back and forth. This can be helpful in determining the effectiveness of your debouncing approach. You should see the light blink one time for each tip. The light also indicates that everything is functioning normally after pressing the reset button of the arduino. If the LED light blinks continuously, something is wrong. If you get several long blinks prior to the LED light shutting off, that is an inidcation that the SD card module and other aspects of the setup code have initialized proplerly.

Look at the LED to determine which side is the negative ("-"; cathode) and positive ("+"; anode) side. The side of the LED with the longest wire lead is the anode (+) side. Looking inside the LED, the wire connected to the biggest metal plate is the cathode (-). Here is a website that explains, with pictures, how to tell which side of the LED is negative or positive: https://startingelectronics.org/beginners/components/LED/.

Connect the cathode leg of the LED to ground. Connect the anode leg of the LED to a 300 ohm resistor, with the other end of the resister connected to pin 5 on the arduino.

| LED pin        |      |  Arduino Nano             |
| :---:          |:---: |  :---:                    |  
| Anode (+)      | ---> |pin 5 and 300 ohm resister |     
| Cathode (-)    | ---> |  GND                      |      
 
## Receiver
The receiver consists of an ESP8266 (NodeMcu 12-E) microcontroller connected to a LoRa transceiver. The receiver sketch configures the LoRa transceiver to receive rather than transmit radio signals. There are two items in the receiver sketch that must match that of the transmitter sketch:

* The LoRa frequency (the sketch specifies 915 megahertz, which is a license-free band in the United States; change this depending on what frequency does not require a radio transmitting license in your country)
*  A key consisting of a four-digit number. The receiver only processes incoming packets of data that contain this key. Without this, I found that my receiver was processing data from other LoRa transceivers in the area, resulting in unwanted data.

The sketch tells you where to insert the following information:
* Your Write API key from Thingspeak
* Your WiFI SSID and password
* Your Thingspeak channel number

Locate the receiver within range of a WiFi/internet signal. It needs to be powered on all the time in order to not miss readings. I power the ESP8266 through the USB port, connecting the other end of the USB cable to a 5-volt USB wall plug used for charging mobile phones. The USB port on the microcontroller is connected to a power regulator that converts the incoming voltage (5 volts) to the operating voltage of 3.3 volts. It is ideal to place the receiver within line of sight of the weather station (transmitter). 

Incoming data are sent to a web platform called Thingspeak. You will need to go to Thingspeak.com and click on "Get Started" to open an account. You can configure Thingspeak to display data in graphs and/or gauges. An mobile phone app called Thingverse works with iOS (am not sure if it also works with Android); after inputting your Thingspeak channel number you can view graphs without having to be logged into Thingspeak. Adafruit IO is another web platform (see the Soil Logger and Wireless Monitor projects in this repository for coding related to Adafruit IO). 


### Wiring
#### LoRa transceiver
| LoRa           |       | Arduino Nano   |
| :---:          | :---: | :---:          |
| GND            |  ---> | GND            |
| VCC/VIN        |  ---> | 3v3            |
| GO/D1          |  ---> | D1             |
| SCK            |  ---> | D5             |
| MOSI           |  ---> | D7             |
| MISO           |  ---> | D6             |
| CS/NSS         |  ---> | D8             |

## Build photos
The weather station as a whole:
![RainGaugeAT](https://user-images.githubusercontent.com/69003593/162528707-3d216195-2819-41f8-b4a7-e936541656bd.jpg)

The Wood enclosure with a wooden cap, all surrounded with thin metal sheeting
![IMG-0652](https://user-images.githubusercontent.com/69003593/162528847-5f0ea7ba-f3cb-4fc4-8700-e5d1bb569d29.jpg)

Wood enclosure with cap/lid removed:
![IMG-0656](https://user-images.githubusercontent.com/69003593/162528898-4ab8b972-6482-4aea-a2bd-d50c5bd58289.jpg)

Bottom of enclosure with wires from rain and temperature/humidity sensor coming in and red antenanna (for LoRa transmitter) protruding out:
![IMG-0658](https://user-images.githubusercontent.com/69003593/162529184-65d28188-9ff5-469d-8960-adce13a4ab8e.jpg)

Microcontroller parts (Schmitt trigger assembly [left]; screw terminals for hooking up sensor wires; sD card module; LED light; DS3231 clock; charge controller):
![IMG-0663](https://user-images.githubusercontent.com/69003593/162529421-9f5d9962-4a7c-44af-98b4-52ca30d1c06f.jpg)

Receiver with ESP8266 and LoRa (note the matching red antenna [but to 9 cm for 915 megahertz frequency]  Note the much cleaner look with the receiver---because wires were soldered directly onto solderable breadboard as opposed to relying more on pre-made jumper wires attached to male pinheaders.
![WeatherStationReceiver](https://user-images.githubusercontent.com/69003593/162530191-18ad3179-fc02-41ad-ab5c-bfbfa55fd26c.jpg)





