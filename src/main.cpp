#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//global variables
Adafruit_BMP280;
float temperature;
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI 11 //These pins are set directly from lab 6, it may be changed.
#define OLED_CLK 13
#define OLED_DC 9
#define OLED_CS 10
#define OLED_RESET 8

void setup(){
  Serial.begin(9600);
  Wire.begin();
  Serial.println("I2C Started");
  Serial.println("Program Started");

  if (bmp.begin(0x76)) {
    Serial.println("BMP280 Found at 0x76");
  }
  else if (bmp.begin(0x77))
  {
    Serial.println("BMP280 Found at 0x77");
  }
  else {
    Serial.println("BMP280 Not Found");
  }
}

void loop(){
  temperature = bmp.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");
}