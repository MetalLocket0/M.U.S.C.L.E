//pH
const int analogInPin = A0; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
// Temperature
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 5


OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

 float Celcius=0;
 float Fahrenheit=0;
 //Clock Module
 #include <Wire.h>
#include <ds3231.h>
 
struct ts t; 
void setup() {
   Serial.begin(9600);
  sensors.begin();
//Clock Module
  Wire.begin();
  
 // DS3231_init(DS3231_CONTROL_INTCN);
  DS3231_init(0);
  /*----------------------------------------------------------------------------
  In order to synchronise your clock module, insert timetable values below !
  ----------------------------------------------------------------------------*/
  t.hour=8; 
  t.min=52;
  t.sec=20;
  t.mday=16;
  t.mon=12;
  t.year=2021;
 
  DS3231_set(t); 

}
 
void loop() {
  //pH
 for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + 21.34;
 Serial.print("sensor = ");
 Serial.println(phValue);
 
 delay(20);
 {
  //Temperature
    sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Fahrenheit=sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  delay(1000);
 }
 {
  //Clock Module
 
   DS3231_get(&t);
  Serial.print("Date : ");
  Serial.print(t.mday);
  Serial.print("/");
  Serial.print(t.mon);
  Serial.print("/");
  Serial.print(t.year);
  Serial.print("\t Hour : ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(".");
  Serial.println(t.sec);
 
  delay(1000);
 }
}
