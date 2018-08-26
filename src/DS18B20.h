#ifndef _DS18B20_H_
#define _DS18B20_H_


#define DS18X_OK 0
#define DS18X_ERR_NO_MORE_ADDR 1
#define DS18X_ERR_CRC_ERROR 2
#define DS18X_ERR_NOT_THE_EXPECTED_DEVICE 3
#define DS18X_ERR_INVALID_RESOLUTION 4

#define DS18X_INVALID_READ 0xFE4D

extern int ds18b20_init(byte pin);
extern int16_t ds18b20SafeAndFastRead(byte pin);

// Input:  ds18b20 adc value
// Output: temperature (C, F, C*10 or F*10)
extern int ADC2C(int16_t t);
extern int ADC2F(int16_t t);
extern int ADC2C10(int16_t t);
extern int ADC2F10(int16_t t);
extern float ADC2Cfp(int16_t t);
extern float ADC2Ffp(int16_t t);

#ifdef _INLINE_

#define ds18b20_Read_C(pin)   (ADC2C(ds18b20SafeAndFastRead(pin)))
#define ds18b20_Read_C10(pin) (ADC2C10(ds18b20SafeAndFastRead(pin)))
#define ds18b20_Read_F(pin)   (ADC2F(ds18b20SafeAndFastRead(pin)))
#define ds18b20_Read_F10(pin) (ADC2F10(ds18b20SafeAndFastRead(pin)))
#define ds18b20_Read_Cfp(pin) (ADC2Cfp(ds18b20SafeAndFastRead(pin)))
#define ds18b20_Read_Ffp(pin) (ADC2Ffp(ds18b20SafeAndFastRead(pin)))

#else //_INLINE_

inline int   ds18b20_Read_C(byte pin)  { ADC2C(ds18b20SafeAndFastRead(pin)); }   // reads temperature in Celsius
inline int   ds18b20_Read_C10(byte pin){ ADC2C10(ds18b20SafeAndFastRead(pin)); } // reads temperature in Celsius*10
inline int   ds18b20_Read_F(byte pin)  { ADC2F(ds18b20SafeAndFastRead(pin)); }   // reads temperature in Fahrenheit
inline int   ds18b20_Read_F10(byte pin){ ADC2F10(ds18b20SafeAndFastRead(pin)); } // reads temperature in Fahrenheit*10
inline float ds18b20_Read_Cfp(byte pin){ ADC2Cfp(ds18b20SafeAndFastRead(pin)); } // reads temperature in Celsius
inline float ds18b20_Read_Ffp(byte pin){ ADC2Ffp(ds18b20SafeAndFastRead(pin)); } // reads temperature in Fahrenheit

#endif //_INLINE_

/*
// the following functions return an integer containing the temperature,
// there are no decimals:
// Example: a temperature of 12.41 is reported as 12
int16_t ds18b20_Read_C (int pin);   // reads temperature in Celsius
int16_t ds18b20_Read_F (int pin);   // reads temperature in Farenheit

// the following functions return an integer with the temperature multiplied by 10
// this allows an integer to store the first decimal digit too.
// Example: a temperature of 12.41 is reported as 124
int16_t ds18b20_Read_C10 (int pin); // reads temperature in Celsius * 10
int16_t ds18b20_Read_F10 (int pin); // reads temperature in Farenheit * 10

Temperature values are rounded-up using the method 'away from zero':
add      0.5 and truncate decimals if positive, 
aubtract 0.5 and truncate decimals if negative.
Examples:
a temperature of  is returned as
12.9              13
12.51             13
12.49             12
-0.4              0
-0.5              -1
-1.9              -2


If for any reason temperature could not be read, functions return DS18X_INVALID_READ.
Reasons for not reading are communication error, device disconnected or other errors.

Please note:
(0) normal DS18B20 reading requires ~ 800 ms, but using a trick,
    the above functions only require ~ 10 ms.
(1) a time interval of 1000 ms must be observed between 2 subsequent calls
    to any of these functions, this is the down-side of the trick.
    There is no check about this, you must take care of it.
    By not observing this precaution may lead to invalid temperature values.
(2) libraries only work with DS18B20 and not with the other variants (DS1820, ...).
*/
#endif //_DS18B20_H_
