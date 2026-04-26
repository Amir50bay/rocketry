/*
 * sd_logger.ino
 * WSO Galacticos — Onboard Data Logger
 *
 * Reads X/Y/Z acceleration from MPU-6050 via I2C and logs
 * raw values to data.txt on a micro SD card (SPI, CS = pin 10).
 * Data is also mirrored to the Serial monitor at 9600 baud.
 *
 * Hardware:
 *   - Arduino Nano
 *   - MPU-6050 (I2C: SDA = A4, SCL = A5)
 *   - Micro SD card module (SPI: CS = pin 10)
 *
 * Known limitation:
 *   The file is opened and closed on every loop iteration to
 *   prevent data loss on unexpected power-off. This reduces
 *   write throughput. A production version would buffer writes
 *   and close on apogee detection or hardware interrupt.
 */

#include <Wire.h>
#include <MPU6050.h>
#include <SD.h>
#include <SPI.h>

MPU6050 mpu;
File dataFile;

const int CS_PIN = 10;
const int SAMPLE_INTERVAL_MS = 1000;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed. Halting.");
    while(1);
  }
  Serial.println("MPU6050 connected.");

  if (!SD.begin(CS_PIN)) {
    Serial.println("SD initialization failed. Halting.");
    while(1);
  }
  Serial.println("SD card ready.");
}

void loop() {
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("X: "); dataFile.print(ax);
    dataFile.print(", Y: "); dataFile.print(ay);
    dataFile.print(", Z: "); dataFile.println(az);
    dataFile.close();
  } else {
    Serial.println("Error opening data.txt");
  }

  Serial.print("X: "); Serial.print(ax);
  Serial.print(", Y: "); Serial.print(ay);
  Serial.print(", Z: "); Serial.println(az);

  delay(SAMPLE_INTERVAL_MS);
}
