#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial nodemcu(5, 6);
#include <EEPROM.h>
#include "GravityTDS.h"
#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows
#include <OneWire.h> 
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
#define TdsSensorPin A1
#define SensorpH A0 
OneWire oneWire(ONE_WIRE_BUS); 
GravityTDS gravityTds;
DallasTemperature sensors(&oneWire);
float tdsValue = 0;
float teganganPh7 = 2.3;   //Nilai kalibrasi
float teganganPh4 = 3.1;    //Nilai kalibrasi
float Po;

void setup()
{
    nodemcu.begin(9600);
    Serial.begin(9600);
    lcd.begin();
    sensors.begin();
    gravityTds.setPin(TdsSensorPin);
    gravityTds.setAref(5.0);
    gravityTds.setAdcRange(1024);
    gravityTds.begin();
}
void loop(){
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();

  delay(1000);
  data["Tds"] = tdsValue;
  data["temperature"] = sensors.getTempCByIndex(0); 
  data["Ph Air"] = Po; 
  //Send data to NodeMCU
  data.printTo(nodemcu);
  jsonBuffer.clear();
  delay(1000);
  
    sensors.requestTemperatures();
    gravityTds.setTemperature(sensors.getTempCByIndex(0));
    gravityTds.update();
    tdsValue = gravityTds.getTdsValue();
    int sensorValue = analogRead(SensorpH);        
  float voltage = sensorValue * (5.0 / 1023.0);
 Po = 7.00 + ((teganganPh7 - voltage) / ((teganganPh4 - teganganPh7)/3));
     
    lcd.setCursor(0, 0);
    lcd.print("TDS    : ");
    lcd.print(tdsValue,0);
    lcd.print(" PPM");
 
    lcd.setCursor(0, 1);
    lcd.print("Temp   : ");
    lcd.print(sensors.getTempCByIndex(0));
    lcd.print(" C");
    delay(1500);
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print("TDS    : ");
    lcd.print(tdsValue,0);
    lcd.print(" PPM");
    lcd.setCursor(0, 1);
    lcd.print("pH Air : ");
    lcd.print(Po,1);
    delay(1500);
    lcd.clear();

}
