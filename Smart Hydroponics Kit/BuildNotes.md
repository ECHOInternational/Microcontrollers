<!-- PROJECT TITLE -->
<br />
<h1 align="center">NFT Hydroponics Smartfarm ☘️</h1>
<br />

<!-- Hardware -->
# Hardware

### Here are the primary devices used in the kit:

> Arduino Nano

> DHT22 Temperature/Humidity Sensor

> MQ135 Gas Sensor

> LiquidCrystal_I2C

> HG7881 Dual Channel Motor Driver (Motor A: 5V Fan | Motor B: Pump)

<br>

<!-- Installation -->
# Installation

## Library
* DHT22 Temp/Hum sensor
```c++
#include <DHT.h> 
DHT dht(TEMPHUM, DHT22);
```
* LiquidCrystal_I2C
```c++
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2);
```
* MQ135 Gas Sensor for CO2
```c++
#include <MQ135.h>
MQ135 mq135(CO2);
```

## Wiring (Breadboard 400 pin)

* ### DHT22 Temp/Hum sensor

|DHT22||Nano|
|:--:|:--:|:--:|
|VCC| --> |5V(+)|
|DATA| --> |D4|
|GND| --> |GND(-)|

* ### LiquidCrystal_I2C

|LCD||Nano|
|:--:|:--:|:--:|
|GND| --> |GND(-)|
|VCC| --> |5V(+)|
|SDA| --> |A4|
|SCL| --> |A5|

* ### MQ135 Gas Sensor for CO2

|MQ135||Nano|
|:--:|:--:|:--:|
|GND| --> |GND(-)|
|VCC| --> |5V(+)|
|AO| --> |A0|

* ### HG7881 Dual Channel Motor Driver

|Driver||Nano|
|:--:|:--:|:--:|
|GND| --> |GND(-)|
|VCC| --> |5V(+)|
|B-1A| --> |D10|
|B-2A| --> |D11|
|A-1A| --> |D6|
|A-2A| --> |D7|

<img src="https://github.com/achoi2025/Microcontrollers/blob/1dc1f8b9e78ce76bc70550bfee27a0d4197fbe63/Smart%20Hydroponics%20Kit/img/4.jpg" width="300">
