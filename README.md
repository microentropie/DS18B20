DS18B20
=======

* Author: Stefano Di Paolo (<http://github.com/microentropie>)
* Copyright &copy; 2018 Stefano Di Paolo.
* Released under the MIT license.

Arduino library.<br>
Read temperature from DS18B20 in 10 milliseconds<br>
(the standard library requires 800 milliseconds).<br>
Please note that it will only work under the following conditions:
* the chip is exactly DS18B20 (no DB1820 or other variants)
* the chip has the standard connection:
  pin 3 (Vcc) connected to Vcc
  pin 2 (output) connected to micro input
  pin 1 (GND) connected to GND
* A 4.7 K resistor is connected between pin 3 and pin 2
* Only one chip connected, if you need more use more Arduino pins
* the function that reads temperature is NEVER called more frequently than 1000 ms:
  at least one second must elapse between 2 calls.
  This time is not checked inside the function, you must take care by yourself.
    
The trick for this big time save is that while the chip is doing its work
(adc conversion), Arduino is free to do other tasks.
So at the moment the function is called, the temperature is ready to be collected
from the internal DS18B20 'scratchpad' (the internal buffer).
Just after having read it, the chip receives the order to start a new conversion
(i.e. read again the temperature). So the function returns temperature,
Arduino runs the sketch while the chip is working in the background.
Temperature is read as integer so no time and memory 'waste' for floating-point
conversions.

The code is derived from Peter Scargill blog (https://www.scargill.net/reading-dallas-ds18b20-chips-quickly/)
He shows a function where he reads temperature in just 5 milliseconds:
he skips integrity (CRC) checks !<cr>
This library performs CRC check while reading so invalid values can be discarded.
Library is Arduino-portable so should work in any Arduino compatible platform.
Tested on Arduino Uno (AtMega328), AtTiny85, ESP8266, ESP32 and STM32F1 (BluePill).

Installation
------------
Download the ZIP archive (<https://github.com/microentropie/Arduino-Libraries> click on DS18B20.zip then click on the Download button)<br>
Open the Arduino IDE and choose Sketch -> Include Library -> Add .ZIP Library... and select the downloaded file.

Code Examples
-------------
Library contains this example:
* [Simple](./examples/Simple/Simple.ino):
  trivial example that reads-in temperature and prints it to Serial.
* [C_and_F](./examples/C_and_F/C_and_F.ino):
  read temperature and print in different formats (Celsius and Fahrenheit, integer and floating-point).
* [TwoSensors](./examples/TwoSensors/TwoSensors.ino):
  read temperature from 2 different DS18B20 and print both.

Documentation
-------------
**int ds18b20_init(byte pin);**<br>
initialize DS18B20 and report any error.<br>
The full list of error codes is in DS18B20.h, DS18X_OK means no error.<br>

Example:<br>
```C++
#include <DS18B20.h>
//...
  int ret = ds18b20_init(12); // initialize DS18B20 connected to pin 12
  if(ret != DS18X_OK);
  {
    Serial.print("Error ");
    Serial.print(ret);
    Serial.println(" initializing DS18B20 chip !");
  }
```

**int ds18b20_Read_C(byte pin);**<br>
read temperature and return it in Celsius using an integer.<br>
In case of read error DS18X_INVALID_READ is returned.<br>

Example:<br>
```C++
#include <DS18B20.h>
// ... initialize
int t = ds18b20_Read_C(12); // read from DS18B20 connected to pin 12
Serial.print(rE); 
```


License
-------
Copyright &copy; 2018 Stefano Di Paolo
The MIT License (MIT).
A copy of the license is enclosed (see license file), or you can read it here:
<https://en.wikipedia.org/wiki/MIT_License>
