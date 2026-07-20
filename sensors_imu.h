#pragma once
#include <MPU6050.h>
#include "config.h"

// ── Struct ────────────────────────────────────────────────────
struct IMUData {
  int16_t accelXx1000;
  int16_t accelYx1000;
  int16_t accelZx1000;
  int16_t gyroXx10;
  int16_t gyroYx10;
  int16_t gyroZx10;
};

// ── Object (defined in sensors_imu.cpp) ───────────────────────
extern MPU6050 mpu;

// ── Function declarations ──────────────────────────────────────
void    initMPU6050();
IMUData readMPU6050();
