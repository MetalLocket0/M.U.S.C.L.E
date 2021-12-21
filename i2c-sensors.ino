// MPU-6050 + TCA9548A
// Modifications by Francisco Sanchez
// November, 2019
// Public Domain
#include<Wire.h>
#include <SD.h>
#include <SPI.h>
#define TCAADDR 0x70

// Temperature
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Celcius = 0;
float Fahrenheit = 0;

//clock
#include <ds3231.h>
struct ts t;

//multiplexer
const byte chipSelect = 10;
String dataString;
void tcaselect(uint8_t i)
{
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}
void writeToCard(int16_t AcX, int16_t AcY, int16_t AcZ, int16_t Tmp, int16_t GyX, int16_t GyY, int16_t GyZ, byte bus) {
  dataString = "";
  dataString += String(bus);
  dataString += "," + String(AcX) + "," + String(AcY) + "," + String(AcZ);
  dataString += "," + String(GyX) + "," + String(GyY) + "," + String(GyZ);


  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

}

//Example for Port 0 out of 8 available ports (0 to 7)
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  //temperature sensor
  sensors.begin();

  //Clock Module
  DS3231_init(0);
  t.hour = 8;
  t.min = 52;
  t.sec = 20;
  t.mday = 16;
  t.mon = 12;
  t.year = 2021;
  DS3231_set(t);

  /*Serial.print("Initializing SD card...");
    pinMode(10, OUTPUT); // change this to 53 on a mega  // don't follow this!!
    digitalWrite(10, HIGH);
    if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    while (1);
    }
    Serial.println("SD Card initialization done.\n");
    Serial.println(SD.exists("datalog.txt"));
    if (!SD.exists("datalog.txt")) {
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.print(dataString);
      dataFile.close();
      Serial.println("successfully printed");
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
    } else {
    Serial.println("file exists\n");
    }
    dataString.reserve(50);*/

  /*tcaselect(6);
    Serial.println("Initalizing MPU6050");
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);*/

}

void loop() {
  /*tcaselect(6);
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
    AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    Serial.print("AcX = ");
    Serial.print(AcX);
    Serial.print(" | AcY = ");
    Serial.print(AcY);
    Serial.print(" | AcZ = ");
    Serial.print(AcZ);
    Serial.print(" | Tmp = ");
    Serial.print(Tmp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
    Serial.print(" | GyX = ");
    Serial.print(GyX);
    Serial.print(" | GyY = ");
    Serial.print(GyY);
    Serial.print(" | GyZ = ");
    Serial.println(GyZ);
    writeToCard(AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ, 6);
    delay(500);*/

  //Temperature
  sensors.requestTemperatures();
  Celcius = sensors.getTempCByIndex(0);
  Fahrenheit = sensors.toFahrenheit(Celcius);
  Serial.print(" C  ");
  Serial.print(Celcius);
  Serial.print(" F  ");
  Serial.println(Fahrenheit);
  delay(1000);

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
