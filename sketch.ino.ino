#include <Wire.h>
#include "config.h"
#include "structs.h"
#include "sensors_imu.h"
#include "sensors_mag.h"
#include "sd_logger.h"

// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println(F("=== CubeSat Boot ==="));

  initMPU6050();
  initHMC5883L();

  Serial.println(F("=== All systems ready ==="));
}

// ─────────────────────────────────────────────────────────────
void loop() {

  // ══ Receive from Station A (dummy - no RF) ══════
  TX_Packet received;
  received.tempX10 = 250;   // 25.0°C dummy
  received.humX10  = 600;   // 60.0% dummy
  strncpy(received.mission, "TOUSHKA", sizeof(received.mission));
  received.mission[sizeof(received.mission) - 1] = '\0';

  // bool gotPacket = receiveTXPacket(received);  // RF معطّل

  // ══ Read onboard sensors ════════════════════════
  IMUData imu = readMPU6050();
  MagData mag = readHMC5883L();

  // ══ Dummy GPS data (no module) ══════════════════
  float dummyLat     = 22.7f;    // منطقة توشكا تقريباً
  float dummyLon     = 32.0f;
  int16_t dummySpeed = 0;

  // ══ Dummy ENV data (no BMP280) ══════════════════
  int16_t dummyPressure = 10132;  // ~1013.2 hPa x10

  // ══ Build outgoing packets ══════════════════════

  Packet1 p1;
  p1.id          = 1;
  p1.dhtTempX10  = received.tempX10;
  p1.dhtHumX10   = received.humX10;
  p1.accelXx1000 = imu.accelXx1000;
  p1.accelYx1000 = imu.accelYx1000;
  p1.accelZx1000 = imu.accelZx1000;
  p1.gyroXx10    = imu.gyroXx10;
  p1.gyroYx10    = imu.gyroYx10;
  p1.gyroZx10    = imu.gyroZx10;

  Packet2 p2;
  p2.id          = 2;
  p2.lat         = dummyLat;
  p2.lon         = dummyLon;
  p2.speedX10    = dummySpeed;
  p2.pressureX10 = dummyPressure;
  p2.sunAngleX10 = mag.sunAngleX10;

  Packet3 p3;
  p3.id         = 3;
  p3.magX       = mag.magX;
  p3.magY       = mag.magY;
  p3.magZ       = mag.magZ;
  p3.headingX10 = mag.headingX10;
  strncpy(p3.mission, received.mission, sizeof(p3.mission));
  p3.mission[sizeof(p3.mission) - 1] = '\0';

  // ══ Serial monitor output ════════════════════════
  Serial.print(F("[IMU] Ax="));  Serial.print(imu.accelXx1000 / 1000.0f, 2);
  Serial.print(F(" Ay="));       Serial.print(imu.accelYx1000 / 1000.0f, 2);
  Serial.print(F(" Az="));       Serial.println(imu.accelZx1000 / 1000.0f, 2);

  Serial.print(F("[MAG] H="));   Serial.print(mag.headingX10 / 10.0f, 1);
  Serial.println(F("°"));

  // sendPackets(p1, p2, p3);     // RF معطّل
  Serial.println(F("[SIM] PKT1+PKT2+PKT3 ready (no TX)"));

  // logData(millis(), p1, p2, p3);  // SD معطّل

  delay(1000);
}