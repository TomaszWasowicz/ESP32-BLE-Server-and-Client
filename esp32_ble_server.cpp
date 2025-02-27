/*
https://randomnerdtutorials.com/esp32-ble-server-client/

BLE Server that advertises a service that contains two characteristics: 
one for temperature and another for humidity. 
Those characteristics have the Notify property 
to notify new values to the client.

We’ll advertise sensor readings from a BME280 sensor. 
So, you need to install the libraries to interface with the BME280 sensor

-Adafruit_BME280 library
-Adafruit_Sensor library

You can install the libraries using the Arduino Library Manager. 
Go to Sketch > Include Library > Manage Libraries and search for the library name

If you’re using VS Code with the PlatformIO extension, 
copy the following to the platformio.ini file to include the libraries:

lib_deps = adafruit/Adafruit Unified Sensor @ ^1.1.4
            adafruit/Adafruit BME280 Library @ ^2.1.2

With the circuit ready and the required libraries installed, 
copy the following code to the Arduino IDE, 
or to the main.cpp file if you’re using VS Code
*/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//Default Temperature is in Celsius
//Comment the next line for Temperature in Fahrenheit
#define temperatureCelsius

//BLE server name
#define bleServerName "BME280_ESP32"

Adafruit_BME280 bme; // I2C

float temp;
float tempF;
float hum;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

bool deviceConnected = false;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

// Temperature Characteristic and Descriptor
#ifdef temperatureCelsius
BLECharacteristic bmeTemperatureCelsiusCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bmeTemperatureCelsiusDescriptor(BLEUUID((uint16_t)0x2902));
#else
BLECharacteristic bmeTemperatureFahrenheitCharacteristics("f78ebbff-c8b7-4107-93de-889a6a06d408", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bmeTemperatureFahrenheitDescriptor(BLEUUID((uint16_t)0x2902));
#endif

// Humidity Characteristic and Descriptor
BLECharacteristic bmeHumidityCharacteristics("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bmeHumidityDescriptor(BLEUUID((uint16_t)0x2903));