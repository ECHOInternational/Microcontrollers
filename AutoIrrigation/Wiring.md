# Wiring  
## Connections between components and the Arduino Nano microcontroller

### Soil moisture sensor
| Soil sensor    |      |  Arduino Nano   |
| :---:          |:---: |  :---:          |
| GND            | ---> |  GND            |
| VCC            | ---> |  D3             |
| Data           | ---> |  A1             |

###Solenoid and diode
Solder a 1N4007 diode between the the two terminals on the solenoid. The diode will have a white stripe on one end. It doesn't matter which terminal of the solenoid is closest to the white strip, but once you solder the diode to the solenoid terminals, or wires connected to the terminals, the terminal closest to the white stripe becomes the positive terminal of the solenoid. 

See this instructible for photos and more details on how to connect the diode to the solenoid, as well as other connections: https://www.instructables.com/Controling-a-solenoid-valve-with-an-Arduino/

### Mosfet, solenoid valve, and battery
1. Connect the gate pin of the mosfet to pin D5 of the Arduino Nano. To make sure the solenoid stays off when it is supposed to, you can connect one leg (wire) of a 10K (K = Kiloohom) resistor to the Gate pin of the mosfet and the other leg (wire) of the 10K resistor to ground (GND of the Arduino Nano)
2. Connect the Drain pin of the mosfet to the negative terminal of the battery.
3. Connect the Source pin of the mosfet to ground (GND of the Arduino Nano).
4. Connect the negative (black; ground) terminal of the battery to the ground (GND) pin of the Arduino Nano.
5. Connect the positive (red; power) terminal of the battery to the positive side of the solenoid.

See the photo below to identify the Gate, Drain, and Source pins on the mosfet. Note that the photo identifies the pin with the black portion (with writing) of the mosfet facing you.

![MosfetLabeled](https://github.com/user-attachments/assets/84e01369-fc0b-42bd-acd3-579d18a86e7e)




| Mosfet   |      |Solenoid    |  Arduino Nano   |
| :---:           | :---:  |  :---:       |       :---:         
| Gate     | ---> |        |      D5      |
| Drain    | ---> |  5v    |              |
| SDA      | ---> |  A4    |              | 
| SCL      | ---> |  A5    |              |  


