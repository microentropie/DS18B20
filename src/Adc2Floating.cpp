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
// returns: a single precision floating point containing temperature in Celsius
float ADC2Cfp(int16_t t)
{
  if (t == DS18X_INVALID_READ) return (float)DS18X_INVALID_READ;
  return t / 16.0;
}

// t:       adc temperature register value
// returns: a single precision floating point containing temperature in Farenheit
float ADC2Ffp(int16_t t)
{
  if (t == DS18X_INVALID_READ) return (float)DS18X_INVALID_READ;
  return t * 1.8 / 16 + 32.0;
} 
