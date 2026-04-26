/*
 * speed_calculator.ino
 * WSO Galacticos — In-Flight Velocity Estimator
 *
 * Estimates vertical velocity by numerically integrating the
 * calibrated Z-axis acceleration from the MPU-6050 IMU.
 * Uses Euler forward integration with millis()-based delta time.
 *
 * Hardware:
 *   - Arduino Nano
 *   - MPU-6050 (I2C: SDA = A4, SCL = A5)
 *
 * Known limitation:
 *   Euler integration accumulates drift over time. Without
 *   barometer fusion or a complementary/Kalman filter, velocity
 *   estimates degrade past approximately 5-8 seconds of flight.
 */

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

float speedZ             = 0.0f;
unsigned long lastTime   = 0;
float accelerationOffset = 0.0f;

const int   CALIBRATION_SAMPLES  = 100;
const int   CALIBRATION_DELAY_MS = 10;
const float LSB_PER_G            = 16384.0f;
const float G_TO_MS2             = 9.81f;
const int   SAMPLE_INTERVAL_MS   = 100;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 not connected. Halting.");
    while(true);
  }
  Serial.println("MPU6050 connected. Calibrating...");

  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    accelerationOffset += mpu.getAccelerationZ();
    delay(CALIBRATION_DELAY_MS);
  }
  accelerationOffset /= CALIBRATION_SAMPLES;

  lastTime = millis();
  Serial.println("Ready.");
}

void loop() {
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0f;
  lastTime = currentTime;

  float rawZ          = mpu.getAccelerationZ() - accelerationOffset;
  float accelerationZ = (rawZ / LSB_PER_G) * G_TO_MS2;

  speedZ += accelerationZ * deltaTime;

  Serial.print("Accel Z (m/s^2): ");
  Serial.print(accelerationZ, 3);
  Serial.print(" | Speed Z (m/s): ");
  Serial.println(speedZ, 3);

  delay(SAMPLE_INTERVAL_MS);
}
