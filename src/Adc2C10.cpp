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
#include <stdint.h>
#ifndef byte
typedef uint8_t byte;
#endif //byte
#include "DS18B20.h"



// t:       adc temperature register value
// returns: an integer containing temperature in Celsius * 10
//          example 123 means 12.3 Celsius
int ADC2C10(int16_t t)
{
  if (t == DS18X_INVALID_READ) return DS18X_INVALID_READ;
  int32_t raf0 = (t < 0) ? -8 : 8; // 'AwayFromZero' round-up
  return (int)(((int32_t)t * 100 + raf0) / 160);
}
