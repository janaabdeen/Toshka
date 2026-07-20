#include "sensors_imu.h"

// ── Object definition ─────────────────────────────────────────
MPU6050 mpu;

// ── Init — retries until MPU6050 responds ─────────────────────
void initMPU6050() {
  Serial.print(F("[INIT] MPU6050"));
  mpu.initialize();
  while (!mpu.testConnection()) {
    Serial.print(F(" ."));
    delay(SENSOR_RETRY_MS);
    mpu.initialize();
  }
  Serial.println(F(" OK"));
}

// ── Read — returns scaled integers, re-inits on bad data ──────
IMUData readMPU6050() {
  IMUData d = {0, 0, 0, 0, 0, 0};

  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float accelX = ax / 16384.0f;
  float accelY = ay / 16384.0f;
  float accelZ = az / 16384.0f;
  float gyroX  = gx / 131.0f;
  float gyroY  = gy / 131.0f;
  float gyroZ  = gz / 131.0f;

  bool accelBad = (accelX < -4.0f || accelX > 4.0f ||
                   accelY < -4.0f || accelY > 4.0f ||
                   accelZ < -4.0f || accelZ > 4.0f);
  bool gyroBad  = (gyroX < -300.0f || gyroX > 300.0f ||
                   gyroY < -300.0f || gyroY > 300.0f ||
                   gyroZ < -300.0f || gyroZ > 300.0f);

  if (accelBad || gyroBad) {
    Serial.println(F("[WARN] MPU6050 bad read — reinitializing"));
    initMPU6050();
    return d;
  }

  d.accelXx1000 = (int16_t)(accelX * SCALE_ACCEL);
  d.accelYx1000 = (int16_t)(accelY * SCALE_ACCEL);
  d.accelZx1000 = (int16_t)(accelZ * SCALE_ACCEL);
  d.gyroXx10    = (int16_t)(gyroX  * SCALE_GYRO);
  d.gyroYx10    = (int16_t)(gyroY  * SCALE_GYRO);
  d.gyroZx10    = (int16_t)(gyroZ  * SCALE_GYRO);
  return d;
}
