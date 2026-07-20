#pragma once
#include <SdFat.h>
#include "config.h"
#include "structs.h"

SdFat   sd;
SdFile  logFile;
bool    sdAvailable  = false;
uint8_t flushCounter = 0;

// ── Init — up to SD_MAX_RETRIES attempts, then continues ──────
// Opens MISSION.CSV and leaves it open for the full mission.
void initSD() {
  Serial.print(F("[INIT] SD card"));
  uint8_t attempts = 0;

  while (!sd.begin(SD_CS_PIN, SPI_HALF_SPEED)) {
    attempts++;
    Serial.print(F(" ."));
    delay(SD_RETRY_MS);
    if (attempts >= SD_MAX_RETRIES) {
      Serial.println(F(" UNAVAILABLE — continuing without logging"));
      return;
    }
  }

  // Write CSV header only if file doesn't exist yet
  // (preserves data across reboots if SD card stays inserted)
  bool isNewFile = !sd.exists("MISSION.CSV");

  // Open file once — stays open for entire mission
  if (!logFile.open("MISSION.CSV", O_WRITE | O_CREAT | O_AT_END)) {
    Serial.println(F(" OPEN FAILED — continuing without logging"));
    return;
  }

  if (isNewFile) {
    logFile.println(F("ms,dhtT,dhtH,aX,aY,aZ,gX,gY,gZ,"
                      "lat,lon,gAlt,spd,pres,bmpT,bmpAlt,"
                      "sunI,sunA,mX,mY,mZ,hdg,mission"));
    logFile.sync();                   // flush header immediately
  }

  sdAvailable = true;
  Serial.println(F(" OK"));
}

// ── Log — appends one CSV row, file stays open between calls ──
void logData(unsigned long timestamp,
             Packet1 &p1, Packet2 &p2, Packet3 &p3) {

  if (!sdAvailable) return;

  // If file was somehow closed (e.g. SPI glitch), attempt recovery
  if (!logFile.isOpen()) {
    Serial.println(F("[WARN] SD file closed unexpectedly — reinitializing"));
    sdAvailable = false;
    initSD();
    return;
  }

  // ── Write CSV row ─────────────────────────────────────────

  // Timestamp
  logFile.print(timestamp);                                     logFile.print(',');

  // PKT1 — DHT11 + MPU6050 (decode scaled integers)
  logFile.print(p1.dhtTempX10  / 10.0f,   1);                  logFile.print(',');
  logFile.print(p1.dhtHumX10   / 10.0f,   1);                  logFile.print(',');
  logFile.print(p1.accelXx1000 / 1000.0f, 2);                  logFile.print(',');
  logFile.print(p1.accelYx1000 / 1000.0f, 2);                  logFile.print(',');
  logFile.print(p1.accelZx1000 / 1000.0f, 2);                  logFile.print(',');
  logFile.print(p1.gyroXx10    / 10.0f,   1);                  logFile.print(',');
  logFile.print(p1.gyroYx10    / 10.0f,   1);                  logFile.print(',');
  logFile.print(p1.gyroZx10    / 10.0f,   1);                  logFile.print(',');

  // PKT2 — GPS + BMP280 + Sun
  logFile.print(p2.lat,                    5);                  logFile.print(',');
  logFile.print(p2.lon,                    5);                  logFile.print(',');
  logFile.print(p2.gpsAlt);                                     logFile.print(',');
  logFile.print(p2.speedX10    / 10.0f,   1);                  logFile.print(',');
  logFile.print(p2.pressureX10 / 10.0f,   1);                  logFile.print(',');
  logFile.print(p2.bmpTempX100 / 100.0f,  2);                  logFile.print(',');
  logFile.print(p2.bmpAlt);                                     logFile.print(',');
  logFile.print(p2.sunIntensity);                               logFile.print(',');
  logFile.print(p2.sunAngleX10 / 10.0f,   1);                  logFile.print(',');

  // PKT3 — Magnetometer + Mission
  logFile.print(p3.magX);                                       logFile.print(',');
  logFile.print(p3.magY);                                       logFile.print(',');
  logFile.print(p3.magZ);                                       logFile.print(',');
  logFile.print(p3.headingX10  / 10.0f,   1);                  logFile.print(',');
  logFile.println(p3.mission);

  // ── Sync every SD_FLUSH_EVERY cycles ──────────────────────
  // Commits buffered data to card without closing the file.
  // FAT is only updated here — not on every write.
  flushCounter++;
  if (flushCounter >= SD_FLUSH_EVERY) {
    logFile.sync();
    flushCounter = 0;
  }
}
