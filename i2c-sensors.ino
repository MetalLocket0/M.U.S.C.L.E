#include<Wire.h>
#include <SD.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3231.h>

#define TCAADDR 0x70

// Temperature
#define ONE_WIRE_BUS 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float Celcius = 0;
float Fahrenheit = 0;

//clock
DS3231 clock1;
byte year;
byte month;
byte date;
byte dOW;
byte hour;
byte minute;
byte second;
bool h12Flag = false;
bool century = false;

//SD Card
const byte chipSelect = 10;
String dataString;

//pH
const int analogInPin = A0;
int sensorValue = 0;
unsigned long int avgValue;
float b;
int buf[10], temp;

//Example for Port 0 out of 8 available ports (0 to 7) and mpu6050 variables
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

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


void setup()
{
  Wire.begin();
  Serial.begin(9600);
  //temperature sensor
  sensors.begin();

  //Clock Module
  clock1.setClockMode(h12Flag);  // set to 24h
  clock1.setYear(year);
  clock1.setMonth(month);
  clock1.setDate(date);
  clock1.setDoW(dOW);
  clock1.setHour(hour);
  clock1.setMinute(minute);
  clock1.setSecond(second);

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
  Serial.print(clock.getYear(), DEC);
  Serial.print(' ');

  // then the month
  Serial.print(clock.getMonth(century), DEC);
  Serial.print(" ");

  // then the date
  Serial.print(clock.getDate(), DEC);
  Serial.print(" ");
  // and the day of the week
  Serial.print(clock.getDoW(), DEC);
  Serial.print(" ");
  // Finally the hour, minute, and second
  Serial.print(clock.getHour(h12Flag, pmFlag), DEC);
  Serial.print(" ");
  Serial.print(clock.getMinute(), DEC);
  Serial.print(" ");
  Serial.print(clock.getSecond(), DEC);
}