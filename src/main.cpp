//Part 1 Libraries
#include <Arduino.h> //Commented code = Georges part
#include <SPI.h>
#include <MFRC522.h>

//Part 2 Libraries
//#include <Wire.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_BMP280.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

//Part 6 Libraries
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

//Part 1 global Variables
#define RFID_SS 5
#define RFID_RST 22

MFRC522 rfid(RFID_SS,RFID_RST);

String authorizedUIDs[] = {"C0 CB A4 5F"}; //Authourized UID
int numAuthorized = 1; //Number of authourized UIDs

#define GREEN_LED 25
#define RED_LED 26

unsigned long indicatorStart = 0;
unsigned long indicatorTime = 0;
bool indicatorOn = false;

String lastUID = "";
unsigned long lastScanTime = 0;

//Part 2 global variables
//Adafruit_BMP280;
//float temperature;
//#define SCREEN_WIDTH 128
//#define SCREEN_HEIGHT 64

//#define OLED_MOSI 11 //These pins are set directly from lab 6, it may be changed.
//#define OLED_CLK 13
//#define OLED_DC 9
//#define OLED_CS 10
//#define OLED_RESET 8

//Part 6 global variables
WebServer server(80);

//Part 1 Functions
String getUID() //Needed to convert binary value into a UID; does not need to be changed
{
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    if (rfid.uid.uidByte[i] < 0x10)
    {
      uid = uid + "0";
    }
    uid = uid + String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1)
    {
      uid = uid + " ";
    }
  }
  uid.toUpperCase();
  return uid;
}

bool isAuthorized(String uid) //Needed
{
  for (int i = 0; i < numAuthorized; i++)
  {
    if (uid == authorizedUIDs[i])
    {
      return true;
    }
  }
  return false;
}

void startIndicatorTimer(unsigned long duration)
{
  indicatorStart = millis();
  indicatorTime = duration;
  indicatorOn = true;
}

void allIndicatorsOff() //Needed
{
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void onAccessGranted() //Needed for authorized cards & to inidcated room is occupied
{
  Serial.println("ACCESS GRANTED:AUTHORIZED CARD");
  Serial.println("ROOM IS OCCUPIED");
  allIndicatorsOff();
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  //startIndicatorTimer(2000);
}

void onAccessDenied() //Needed for unathourized cards
{
  Serial.println("ACCESS DENIED:UNAUTHORIZED CARD");
  //Serial.println("ROOM IS FREE");
  allIndicatorsOff();
  int count = 1;
  for(int i = 0; i < count; i++)
  {
    count++;
    digitalWrite(RED_LED, HIGH);
    delay(1000);
    digitalWrite(RED_LED, LOW);
    delay(1000);
    digitalWrite(GREEN_LED,LOW);
    //startIndicatorTimer(1000);
  }
} 

void onTimerExpired()
{
  allIndicatorsOff();
  indicatorOn = false;
  //Serial.println("ROOM IS FREE");
}

void onCardScanned(String uid)
{
  if (uid == lastUID && millis() - lastScanTime < 2000)
  {
    //Serial.println("Repeat ignored");
    return;
  }
  lastUID = uid;
  lastScanTime = millis();

 // Serial.print("Card UID: ");
  //Serial.println(uid);

  if (isAuthorized(uid) == true)
  {
    onAccessGranted();
  }
  else
  {
    onAccessDenied();
  }
}



void setup(){

//Part 1 Setup() code
  Serial.begin(115200);
  Serial.println("Program Started");

  // RFID code
  SPI.begin();
  rfid.PCD_Init();
  delay(10);
  rfid.PCD_DumpVersionToSerial();
  Serial.println("Room is free");

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  //Part 2 Setup() code
  //Wire.begin();
  //Serial.println("I2C Started");
  //Serial.println("Program Started");

 // if (bmp.begin(0x76)) {
   // Serial.println("BMP280 Found at 0x76");
 // }
  //else if (bmp.begin(0x77))
 // {
  //  Serial.println("BMP280 Found at 0x77");
 // }
 // else {
  //  Serial.println("BMP280 Not Found");
  //}

  //Part 6 Setup() code
  server.begin()
  Serial.println("Web Server Started");
}

void loop(){
  //Part 1 loop() code
  //Card Detection

if (rfid.PICC_IsNewCardPresent() == true && rfid.PICC_ReadCardSerial() == true)
{
  String uid = getUID();
  onCardScanned(uid);
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

//Timer
if (indicatorOn == true && millis() - indicatorStart >= indicatorTime)
{
  onTimerExpired();
}

  //Part 2 loop() code
 // temperature = bmp.readTemperature();
 // Serial.print("Temperature: ");
 // Serial.print(temperature);
 // Serial.println(" C");

}
