#include <Adafruit_BMP085.h>
#include <Ethernet.h>
#include <Wire.h> //Chama a biblioteca
#include "ThingSpeak.h"
#include <LiquidCrystal_I2C.h> //Chama a biblioteca
#include <dht.h>
#include <HttpClient.h>

#define mydht 7
#define I2C_ADDR  0x27
#define BACKLIGHT_PIN  3
#define En  2
#define Rw  1
#define Rs  0
#define D4  4
#define D5  5
#define D6  6
#define D7  7

byte mac[]{0x90, 0xA2, 0xDA, 0x00, 0x4A, 0xE0};
byte a[]= {B00110,B01001,B00110,B00000,B00000,B00000,B00000,B00000}; 
unsigned long myChannelNumber = 785414, timer = 0;
const char * myWriteAPIKey = "C2JXJ1J6KCSVWRFV";
//uint32_t timer = 0;
Adafruit_BMP085 bmp;
dht sensor;
IPAddress ip(192, 168, 0, 10);
IPAddress myDns(192, 168, 0, 1);
EthernetClient client;
EthernetServer server = EthernetServer(80); //port 80

LiquidCrystal_I2C lcd(I2C_ADDR,En,Rw,Rs,D4,D5,D6,D7);
float number1, number2, number3, number4;
void setup(){
  Ethernet.init(10);
  Ethernet.begin(mac);
  //Server.begin();
  Ethernet.localIP();
  
  lcd.begin (16,2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.createChar(1, a);
  lcd.setCursor(15, 1);
  //Serial.begin(9600);
  ThingSpeak.begin(client);
}

void loop(){
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  ThingSpeak.setField(3, number3);
  ThingSpeak.setField(4, number4);

  sensor.read22(mydht);
  //float t2 = bmp.readTemperature();
  //Serial.println(bmp.readTemperature());
  float a = bmp.readPressure();
  float p = bmp.readAltitude();
  float h = sensor.humidity; // Valor da umidade
  float t = sensor.temperature; // Valor da temperatura
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  //float tm = (t + t2)/2;

  if(millis() - timer < 1000){
    lcd.setBacklight(HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Umidade: ");
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.write(1);
    lcd.print("c");
  }
  
  if((millis() - timer >= 3000) && (millis() - timer < 4000)){
    lcd.setBacklight(HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Umidade: ");
    lcd.print(h);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(t);
    lcd.write(1);
    lcd.print("c");
  }

  if((millis() - timer >= 4000) && (millis() - timer < 5000)){
    lcd.clear();
    lcd.setBacklight(LOW);
  }
  
  if((millis() - timer >= 5000) && (millis() - timer < 6000)){
    lcd.setBacklight(HIGH);
    lcd.setCursor(0, 0);
    lcd.print("Altitude ");
    lcd.print(a);
    lcd.print("m");
    lcd.setCursor(0, 1);
    lcd.print("Pressao: ");
    lcd.print(p);
    lcd.print("Pa");
  }
  
  if((millis() - timer >= 6000) && (millis() - timer < 7000)){
    lcd.clear();
    lcd.setBacklight(LOW);
  }

  if(millis() - timer > 7000){
    timer = millis();
  }
  
  number1 = t;
  number2 = h;
  number3 = p;
  number4 = a;
}
