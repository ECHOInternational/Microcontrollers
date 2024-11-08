# Wiring  
## Connections between components and the Arduino Nano microcontroller

### Soil moisture sensor
| Soil sensor    |      |  Arduino Nano   |
| :---:          |:---: |  :---:          |
| GND            | ---> |  GND            |
| VCC            | ---> |  D3             |
| Data           | ---> |  A1             |

### Mosfet and solenoid alve
| Mosfet   |      |Solenoid    |  Arduino Nano   |
| :---:           | :---:  |  :---:       |       :---:         
| Gate     | ---> |        |      D5      |
| Drain    | ---> |  5v    |              |
| SDA      | ---> |  A4    |              | 
| SCL      | ---> |  A5    |              |  
