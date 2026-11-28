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
unsigned long lastTempTime = 0;
const unsigned long TEMP_INTERVAL = 2000; // Interval for temperature readings in milliseconds
int sampleIndex = 0;

#define OLED_MOSI 23 //These pins are set directly from lab 6, it may be changed.
#define OLED_CLK 18
#define OLED_DC 17
#define OLED_CS 16
#define OLED_RESET 4

const int NUM_SAMPLES = 10; // Number of samples to average for temperature readings
float samples[NUM_SAMPLES];

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
  temperature = bmp.readTemperature();
  samples[sampleIndex] = temperature; //saves the read temp to the samples array at the sanple index slot which starts at 0 and goes up to 9.
  sampleIndex++; // writes to slot 0 to 1 to 2 to 3 ...
  if (sampleIndex >= NUM_SAMPLES) { //resets sample index to 0 if it goes over 9 and overwrites the oldest sample with the newest.
    sampleIndex = 0; //resets sample back to slot 0
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
  Serial.begin(115200);
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
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.display();
}

void loop(){

  unsigned long currentMillis = millis();
  if (currentMillis - lastTempTime >= TEMP_INTERVAL) { //does the current time - the lasttemptime which is 0 and says if its been 2 seconds since last read
    lastTempTime = currentMillis; //resets the lasttemptime to the current time so it can start counting again
    takesSamples(); // already have bmp.readTemperature() in takesSamples() so no need to do it again here
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");
  
    printSamples();

    Serial.print("Average Temperature: ");
    Serial.print(getAverageTemperature());
    Serial.println(" C");

    // display room status on OLED over here

    float averageTemperature = getAverageTemperature();
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Average Temp:");
    display.print(averageTemperature, 1);
    display.println("C");
    display.display();
  }
}