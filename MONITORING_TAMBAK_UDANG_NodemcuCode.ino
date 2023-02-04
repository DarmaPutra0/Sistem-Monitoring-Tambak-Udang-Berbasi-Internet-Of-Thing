#include <ESP8266WiFi.h>              //Library ESP8266
#include <BlynkSimpleEsp8266.h>       //Library Blynk
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

char auth[] = "NOu7Gc1LGYvTxYbMjbb6dq5qDBG0KhMR"; //Token
char ssid[] = "BINABNI";                          //Nama wifi
char pass[] = "nasikuning";                         //Password wifi

BlynkTimer timer;

#define SensorpH A0                 //Sensor pH pada pin A0

float teganganPh7 = 2.73;   //Nilai kalibrasi
float teganganPh4 = 3.3;    //Nilai kalibrasi
float Po;

#include <OneWire.h>                //Library komunikasi one wire
#include <DallasTemperature.h>      //Library sensor
#define ONE_WIRE_BUS 0              //Deklarasi pin komunikasi one wire
#define sensor 0                    //Sensor pada pin D3
OneWire oneWire(ONE_WIRE_BUS);      //Kenalkan pin komunikasi one wire
DallasTemperature Suhu(&oneWire);   //Masukkan pada library

#include <Servo.h>        //Library Servo
Servo myservo;           //Nama Servo


float tAir;               //Variabel penampung nilai suhu
void setup() {
   lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Lombok");
  lcd.setCursor(0, 1);
  lcd.print("Robotika");
  delay(3000);
  lcd.clear();
  Blynk.begin(auth, ssid, pass);          //Inisialisasi token, wifi dan password
  timer.setInterval(1000L, sendSensor);   //Kirim data sensor
  Suhu.begin();           //Mulai sensor suhu
  myservo.attach(2);
}

void loop() {
  Blynk.run();                        //Jalankan pembacaan Blynk
  timer.run();  
  int sensorValue = analogRead(SensorpH);         //Baca Sensor
  float voltage = sensorValue * (5.0 / 1023.0);   //Konversi menjadi nilai tegangan
  //Rumus mencari pH
  Po = 7.00 + ((teganganPh7 - voltage) / ((teganganPh4 - teganganPh7)/3));

  Suhu.requestTemperatures();       //Baca suhu
  tAir =  Suhu.getTempCByIndex(0);  //Masukkan pada variabel
  
}
BLYNK_WRITE(V2){                      //Baca pin V1
  int tombolservo = param.asInt();
  if (tombolservo == 1){
    myservo.write(0);
    delay (2000);
    myservo.write(90);
  }
}


void sendSensor(){                  //Kirim data sensor
  Blynk.virtualWrite(V0,Po);
  Blynk.virtualWrite(V1,tAir);
}
