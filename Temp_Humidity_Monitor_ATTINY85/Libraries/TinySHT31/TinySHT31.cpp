//This is a modification of the Grove_SHT31_Temp_Humi_Sensor from Seeed Studio, as explained by Wolfgang Ewald (2022) 
//in TinyWireM vs Wire for ATtinys: https://wolles-elektronikkiste.de/en/tinywirem-vs-wire-for-attinys

//This is the cpp file


#include "TinySHT31.h"

SHT31::SHT31() {
}

boolean SHT31::begin(uint8_t i2caddr) {
  //Wire.begin();
  //TinyWireM.begin();  
  _i2caddr = i2caddr;
  reset();
  //return (readStatus() == 0x40);
  return true;
}

float SHT31::getTemperature(void) {
  if (! getTempHum()) return NAN;
  return temp;
}


float SHT31::getHumidity(void) {
  if (! getTempHum()) return NAN;
  return humidity;
}

uint16_t SHT31::readStatus(void) {  
return 42; ///Without this it compiles with ard nano but not attiny85
}

void SHT31::reset(void) {
  writeCommand(SHT31_SOFTRESET);
  delay(10);
}

void SHT31::heater(boolean h) {
  if (h)
    writeCommand(SHT31_HEATEREN);
  else
    writeCommand(SHT31_HEATERDIS);
}

uint8_t SHT31::crc8(const uint8_t *data, int len) {
  const uint8_t POLYNOMIAL(0x31);
  uint8_t crc(0xFF);
  
  for ( int j = len; j; --j ) {
      crc ^= *data++;

      for ( int i = 8; i; --i ) {
	crc = ( crc & 0x80 )
	  ? (crc << 1) ^ POLYNOMIAL
	  : (crc << 1);
      }
  }
  return crc; 
}


boolean SHT31::getTempHum(void) {
  uint8_t readbuffer[6];

  writeCommand(SHT31_MEAS_HIGHREP);
  
  delay(50);
 // Wire.requestFrom(_i2caddr, (uint8_t)6);
 TinyWireM.requestFrom(_i2caddr,6); //////// Add!!!!  
//if (Wire.available() != 6) 
  if (TinyWireM.available() != 6)
    return false;
  for (uint8_t i=0; i<6; i++) {
   // readbuffer[i] = Wire.read();
   readbuffer[i] = TinyWireM.receive(); /////// Add!!!!
  }
  uint16_t ST, SRH;
  ST = readbuffer[0];
  ST <<= 8;
  ST |= readbuffer[1];

  if (readbuffer[2] != crc8(readbuffer, 2)) return false;

  SRH = readbuffer[3];
  SRH <<= 8;
  SRH |= readbuffer[4];

  if (readbuffer[5] != crc8(readbuffer+3, 2)) return false;
 
  double stemp = ST;
  stemp *= 175;
  stemp /= 0xffff;
  stemp = -45 + stemp;
  temp = stemp;
  
  double shum = SRH;
  shum *= 100;
  shum /= 0xFFFF;
  
  humidity = shum;
  
  return true;
}

void SHT31::writeCommand(uint16_t cmd) {
  /*Wire.beginTransmission(_i2caddr);
  Wire.write(cmd >> 8);
  Wire.write(cmd & 0xFF);
  Wire.endTransmission(); */
  TinyWireM.beginTransmission(_i2caddr); /////// Add the following lines!!!!
  TinyWireM.send((uint8_t)(cmd >> 8));
  TinyWireM.send((uint8_t)(cmd & 0xFF));
  TinyWireM.endTransmission();
      
}
