#pragma once
#include "config.h"

struct MagData {
  int16_t  magX;
  int16_t  magY;
  int16_t  magZ;
  uint16_t headingX10;
  int16_t  sunAngleX10;
};

void initHMC5883L() {
  Serial.println(F("[INIT] HMC5883L (simulated)"));
}

MagData readHMC5883L() {
  MagData d;

  // Fake mag raw values
  d.magX = random(-300, 300);
  d.magY = random(-300, 300);
  d.magZ = random(-300, 300);

  // Heading من 0 لـ 360
  float heading = atan2((float)d.magY, (float)d.magX) * (180.0f / PI);
  if (heading < 0.0f) heading += 360.0f;

  // Sun angle offset
  float sunOffset = heading - SUN_AZIMUTH;
  if (sunOffset >  180.0f) sunOffset -= 360.0f;
  if (sunOffset < -180.0f) sunOffset += 360.0f;

  d.headingX10  = (uint16_t)(heading  * SCALE_HEADING);
  d.sunAngleX10 = (int16_t)(sunOffset * SCALE_SUN_ANGLE);

  return d;
}