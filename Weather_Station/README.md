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
  * A 10 Kohm resistor
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
* Schmitt trigger and related parts  


