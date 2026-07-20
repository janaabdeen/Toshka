#pragma once
#include <Adafruit_BMP280.h>
#include "config.h"

// ═══════════════════════════════════════════════════════════════
Adafruit_BMP280 bmp;

struct EnvData {
  uint16_t pressureX10;     // hPa × SCALE_PRESSURE (10)
  int16_t  bmpTempX100;     // °C  × SCALE_BMP_TEMP (100)
  int16_t  bmpAlt;          // m, 1m resolution
};

// ── Init — retries until BMP280 is found on I2C ───────────────
void initBMP280() {
  Serial.print(F("[INIT] BMP280"));
  while (!bmp.begin(BMP180_ADDR)) {
    Serial.print(F(" ."));
    delay(SENSOR_RETRY_MS);
  }
  bmp.setSampling(
    Adafruit_BMP180::MODE_NORMAL,
    Adafruit_BMP180::SAMPLING_X2,
    Adafruit_BMP180::SAMPLING_X16,
    Adafruit_BMP180::FILTER_X16,
    Adafruit_BMP180::STANDBY_MS_500
  );
  Serial.println(F(" OK"));
}

// ── Read — returns scaled integers, re-inits on bad data ──────
EnvData readBMP280() {
  EnvData d = {0, 0, 0};

  float temp = bmp.readTemperature();
  float pres = bmp.readPressure() / 100.0F;   // Pa → hPa
  float alt  = bmp.readAltitude(1013.25F);

  // Health check — BMP280 operating range
  bool tempBad = (temp < -40.0f || temp > 85.0f);
  bool presBad = (pres < 300.0f || pres > 1100.0f);

  if (tempBad || presBad) {
    Serial.println(F("[WARN] BMP280 bad read — reinitializing"));
    initBMP280();
    return d;
  }

  d.pressureX10 = (uint16_t)(pres * SCALE_PRESSURE);
  d.bmpTempX100 = (int16_t)(temp  * SCALE_BMP_TEMP);
  d.bmpAlt      = (int16_t)alt;
  return d;
}
