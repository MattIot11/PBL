#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//global variables
Adafruit_BMP280 bmp;
float temperature;
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI 11 //These pins are set directly from lab 6, it may be changed.
#define OLED_CLK 13
#define OLED_DC 9
#define OLED_CS 10
#define OLED_RESET 8

const int NUM_SAMPLES = 10; // Number of samples to average for temperature readings
int samples[NUM_SAMPLES];

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  OLED_MOSI,
  OLED_CLK,
  OLED_DC,
  OLED_CS,
  OLED_RESET
);

void takesSamples() {
  for (int i = 0; i < NUM_SAMPLES; i++) {
    samples[i] = bmp.readTemperature();
  }
}

void printSamples() {
  for (int i = 0; i < NUM_SAMPLES; i++) { // dont think i need this, but just in case, lab 11 arrays
  Serial.print("sample[");
  Serial.print(i);
  Serial.print("]= ");
  Serial.println(samples[i]);
  }
}

float getAverageTemperature() {
  float sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum = sum + samples[i];
  }
  return sum / NUM_SAMPLES;
}

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

  display.begin(SSD1306_SWITCHCAPVCC);
}

void loop(){
  temperature = bmp.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  takesSamples();
  printSamples();

  Serial.print("Average Temperature: ");
  Serial.print(getAverageTemperature());
  Serial.println(" C");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  // display room status on OLED over here

  float averageTemperature = getAverageTemperature();
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Average Temp:");
  display.print(averageTemperature, 1);
  display.println("C");
  display.display();

  delay(2000); // Delay for 2 seconds before the next reading
}