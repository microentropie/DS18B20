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

// define the Arduino pin where DS18B20 is connected:
#define ONEWIRE_PIN 2      // pin2   in Arduino Uno (PD2)

void setup()
{
  initializations(115200);

  int ret = ds18b20_init(ONEWIRE_PIN);
  if(ret != DS18X_OK);
  {
    Serial.print("Error ");
    Serial.print(ret);
    Serial.println(" initializing DS18B20 chip !");
  }
}

void loop()
{
  // get temperature and print in Celsius
  int t = ds18b20_Read_C10(ONEWIRE_PIN);
  //int t = ds18b20_Read_F10(ONEWIRE_PIN);
  Serial.print("temperature: ");
  Serial.print(t / 10);
  Serial.print(".");
  Serial.print(t % 10);
  Serial.println(" C");
  //Serial.println(" F");
  
  // to get the temperature in Farhenheit call ds18b20_Read_F10()
  // IMPORTANT:
  // to print temperature in multiple formats (both C and F) please
  // have a look at "C_and_F" sketch.

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
