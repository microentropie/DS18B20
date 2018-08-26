/*
  Fast routine (5 ms) to read temperature from DS18B20.
  It will only work under the following conditions:
  * the chip is exactly DS18B20
  * the chip has the standard connection:
    pin 3 (Vcc) connected to Vcc
    pin 2 (output) connected to micro input
    pin 1 (GND) connected to GND
    A 4.7 K resistor is connected between pin 3 and pin 2
  * the ds18b20FastRead() method is NEVER called more frequently than 1000 ms.
    At least one second must elapse between 2 calls.
    The time is not checked inside the function.
*/
#include <OneWire.h>
#include "DS18B20.h" 



// pin: arduino pin number
// return: DS18X_OK if ok or error code
int ds18b20_init(byte pin)
{
  byte data[12];
  byte addr[8];


  OneWire ds(pin);

  // Read ROM:
  if (!ds.search(addr))
  {
    ds.reset_search();
    delay(250);
    return DS18X_ERR_NO_MORE_ADDR; // no more addresses
  }
  if (OneWire::crc8(addr, 7) != addr[7]) return DS18X_ERR_CRC_ERROR; // read ERROR: invalid data readin

  // first byte is the device type
  if (addr[0] != 0x28) return DS18X_ERR_NOT_THE_EXPECTED_DEVICE; // this is not DS18B20

  // now start a new conversion
  ds.reset();
  //ds.select(addr);
  ds.write(0xCC); // skip ROM command
  ds.write(0x44, 1); // start conversion, assuming 5V connected to Vcc pin
  delay(1000);

  ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad
  for (byte i = 0; i < 9; i++) // we need 9 bytes
    data[i] = ds.read();

  if ((data[4] & 0x60) != 0x60) return DS18X_ERR_INVALID_RESOLUTION; // resolution is not 12 bit as expected

  return DS18X_OK;
}

// Returns a 16 bit number with the Celsius temperature * 16,
// this is the value returned by internal ADC.
// The read-in data is error checked: DS18X_INVALID_READ is returned if error.
// This function takes ~ 9 ms.
// The internal chip temperature read process takes ~ 800 ms,
// but while converting, Arduino is free to do other tasks.
// For this reason you MUST leave 1000 ms or more between 2 subsequent calls
// to this method.
// The trick is that after the temperature has been collected,
// a new temperature readin is asked to the chip. In the subsequent call
// to this function that value (stored in the internal 'scratchpad') is collected.
//
// pin:     arduino pin number
// returns: Celsius temperature *16 or DS18X_INVALID_READ if error
int16_t ds18b20SafeAndFastRead(byte pin)
{
  byte data[9];
  int16_t result;

  OneWire ds(pin);
  ds.reset();
  ds.write(0xCC); // skip ROM command
  ds.write(0xBE); // read scratchpad command
  for (byte i = 0; i < 9; ++i) data[i] = ds.read(); // read
  if (OneWire::crc8(data, 8) != data[8]) return DS18X_INVALID_READ; // read ERROR: invalid data readin
  // device is DS18B20, already checked during initialization that resolution is 12 bit
  result = (data[1] << 8) | data[0];

  // now start a new conversion
  ds.reset();
  ds.write(0xCC); // skip ROM command
  ds.write(0x44, 1); // start conversion, assuming 5V connected to Vcc pin

  return result;
}

#ifdef UNIT_TESTS
struct utStorage
{
  int16_t adc;
  int     c;
  int     c10;
  int     f;
  int     f10;
};

bool test1Temperature(utStorage *pUt)
{
  Serial.println("\n");
  Serial.print(pUt->adc);
  if (ADC2C(pUt->adc) != pUt->c) return false;
  Serial.print("ADC2C ok");
  if (ADC2C10(pUt->adc) != pUt->c10) return false;
  Serial.print(", ADC2C10 ok");
  if (ADC2F(pUt->adc) != pUt->f) return false;
  Serial.print(", ADC2F ok");
  if (ADC2F10(pUt->adc) != pUt->f10) return false;
  Serial.print(", ADC2F10 ok");

  return true;
}

bool UnitTests()
{
  utStorage testComparer[] =
  {
    //   adc     C   C*10     F   F*10
    { 0x0000,    0,     0,   32,   320 }, // 0 C = 32 F
    { 0x0640,    0,     0,   32,   320 }, // 100 C = 212 F
    { 0xFE6F,  -25,  -250,  -13,  -131 }, // -25.0625 C = -13.11 F
    { DS18X_INVALID_READ, DS18X_INVALID_READ, DS18X_INVALID_READ, DS18X_INVALID_READ, DS18X_INVALID_READ }, // invalid read, sensor off-line or communication error
  };

  for (int i = 0; i < sizeof(testComparer) / sizeof(testComparer[0]))
    if (!test1Temperature(&testComparer[i])) return false;
  return true;
}
#endif //UNIT_TESTS
