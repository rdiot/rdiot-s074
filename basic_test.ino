/* Barometric Pressure Altitude BMP180 (BMP180) [S074] : http://rdiot.tistory.com/176 [RDIoT Demo] */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
LiquidCrystal_I2C lcd(0x27,20,4);  // LCD2004
 
// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"
 
// I2Cdev and BMP085 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "BMP085.h"
 
// class default I2C address is 0x77
// specific I2C addresses may be passed as a parameter here
// (though the BMP085 supports only one address)
BMP085 barometer;
 
float temperature;
float pressure;
float altitude;
int32_t lastMicros;
 
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;
 
void setup()
{
  lcd.init();  // initialize the lcd 
  lcd.backlight();
  lcd.print("start LCD2004");
 
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  barometer.initialize();
 
  pinMode(LED_PIN, OUTPUT);
  delay(1000);
 
  lcd.clear();
}
 
void loop()
{
 
  lcd.setCursor(0,0);
  lcd.print("S074:BMP180");
 
    // request temperature
    barometer.setControl(BMP085_MODE_TEMPERATURE);
    
    // wait appropriate time for conversion (4.5ms delay)
    lastMicros = micros();
    while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());
 
    // read calibrated temperature value in degrees Celsius
    temperature = barometer.getTemperatureC();
 
    // request pressure (3x oversampling mode, high detail, 23.5ms delay)
    barometer.setControl(BMP085_MODE_PRESSURE_3);
    while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());
 
    // read calibrated pressure value in Pascals (Pa)
    pressure = barometer.getPressure();
 
    // calculate absolute altitude in meters based on known pressure
    // (may pass a second "sea level pressure" parameter here,
    // otherwise uses the standard value of 101325 Pa)
    altitude = barometer.getAltitude(pressure);
 
    // display measured values if appropriate
    lcd.setCursor(0,1);
    lcd.print("Temperature=" + (String)temperature + "  ");
 
    lcd.setCursor(0,2);
    lcd.print("Pressure=" + (String)pressure + "  ");
 
    lcd.setCursor(0,3);
    lcd.print("Altitude=" + (String)altitude + "  ");
 
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
    // delay 100 msec to allow visually parsing blink and any serial output
    delay(100); 
}
