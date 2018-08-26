/*
Author:  Stefano Di Paolo
License: MIT, https://en.wikipedia.org/wiki/MIT_License
Date:  2018-07-12

Library: DS18B20 temperature read example.

Sources repository: https://github.com/microentropie/
*/
#include <DS18B20.h>

/*
Required Libraries:
* OneWire
* https://github.com/microentropie/DS18B20
*/

// define the Arduino pins where the 2 DS18B20 are connected:
#define ONEWIRE1_PIN 2      // pin2   in Arduino Uno (PD2)
#define ONEWIRE2_PIN 3      // pin3   in Arduino Uno (PD3)

void setup()
{
  initializations(115200); //<== Serial baud rate

  int ret = ds18b20_init(ONEWIRE1_PIN);
  if(ret != DS18X_OK);
  {
    Serial.print("Error ");
    Serial.print(ret);
    Serial.println(" initializing 1st DS18B20 chip !");
  }

  ret = ds18b20_init(ONEWIRE2_PIN);
  if(ret != DS18X_OK);
  {
    Serial.print("Error ");
    Serial.print(ret);
    Serial.println(" initializing 2nd DS18B20 chip !");
  }
}

void ReadAndPrintTemp(byte pin)
{
  // print temperature in Celsius
  int t = ds18b20_Read_C10(pin);
  //int t = ds18b20_Read_F10(pin);

  Serial.print("chip on pin ");
  Serial.print(pin);
  Serial.print(", temperature: ");
  if(t == DS18X_INVALID_READ)
  {
    Serial.println("***error***");
  }
  else
  {
    Serial.print(t / 10);
    Serial.print(".");
    Serial.print(t % 10);
    Serial.println(" C");
    //Serial.println(" F");
  }
  
  // to get the temperature in Farhenheit call ds18b20_Read_F10()
  // IMPORTANT:
  // to print temperature in multiple formats (both C and F) please
  // have a look at "C_and_F" sketch.
}

void loop()
{
  ReadAndPrintTemp(ONEWIRE1_PIN);
  ReadAndPrintTemp(ONEWIRE2_PIN);

  delay(1000); // the delay is absolutely required ! 1000 or more ms
}

void initializations(int baudRate) // <== serial baud rate
{
#ifdef ARDUINO_ARCH_STM32F1
  // trick to give user the time to open the Serial Monitor
  //
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off
  // the USB port is not available immediately after reset or programming;
  // approximatively 5 seconds are required
  int qtSecondsDelay = 6;
  for(int b = 0; b < (qtSecondsDelay << 1); ++b)
  {
    digitalWrite(LED_BUILTIN, (b & 1));
    delay(500);
  }
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off
#endif //ARDUINO_ARCH_STM32F1

  Serial.begin(baudRate);
  Serial.println();
  Serial.println();
  Serial.println(__FILE__);
  Serial.println("https://github.com/microentropie/DS18B20");
}
