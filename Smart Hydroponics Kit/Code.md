```c++
// by Dongmin Choi

// fan uses motor driver A
#define FAN1 6 // connect digital pin 6 to A-1A
#define FAN2 7 // connect digital pin 7 to A-2A

// water pump uses motor driver B
#define WATER1 10 // connect digital pin 10 to B-1A
#define WATER2 11 // connect digital pin 11 to B-2A

#define TEMPHUM 4

#define CO2 A0

#include <DHT.h> // DHT22 Temp/Hum Sensor
DHT dht(TEMPHUM, DHT22);

#include <LiquidCrystal_I2C.h> // LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

#include <MQ135.h> // MQ135 Gas Sensor for CO2
MQ135 mq135(CO2);

void setup() {
  Serial.begin(9600);

  pinMode(WATER1, OUTPUT);
  pinMode(WATER2, OUTPUT);

  pinMode(FAN1, OUTPUT);
  pinMode(FAN2, OUTPUT);

  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("--- SmartFarm ---");
}

void loop() {
  digitalWrite(WATER1, HIGH); // pump activates when one returns HIGH and another returns LOW
  digitalWrite(WATER2, LOW);
  delay(70000); // water circulates for 70 seconds

  digitalWrite(WATER1, HIGH); // pump stops when both returns HIGH
  digitalWrite(WATER2, HIGH);
  delay(30000); //pause for 30 seconds

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  float co2 = mq135.getPPM();

  // if co2 concentration is high, activiate fan for 10 seconds
  if(co2 > 600) { 
    digitalWrite(FAN1, HIGH);
    digitalWrite(FAN2, LOW);
    delay(10000);
    digitalWrite(FAN1, HIGH);
    digitalWrite(FAN2, HIGH);
  }

  Serial.println(temp);
  Serial.println(hum);
  Serial.println(co2);
  
  //display variables on lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TEMP: " + (String)temp + (char)223 + "C");
  lcd.setCursor(0, 1);
  lcd.print("CO2: " + (String)co2 + "ppm");
  delay(4000);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("HUM: " + (String)hum + "%");
  delay(4000);
}
```
