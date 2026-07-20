#pragma once
#include <stdint.h>

// ═══════════════════════════════════════════════════════════════
//  Byte budgets (NRF max = 32 bytes per packet):
//  TX_Packet : 24 bytes
//  Packet1   : 17 bytes 
//  Packet2   : 22 bytes 
//  Packet3   : 29 bytes 
// ═══════════════════════════════════════════════════════════════

// ── Incoming from Station A ───────────────────────────────────
// 2 + 2 + 20 = 24 bytes
struct TX_Packet {
  int16_t tempX10;          
  int16_t humX10;           
  char    mission[20];      
};

// ── PKT1: DHT11 + MPU6050 ─────────────────────────────────────
// 1 + 2+2 + 2+2+2 + 2+2+2 = 17 bytes
struct Packet1 {
  uint8_t id;               
  int16_t dhtTempX10;      
  int16_t dhtHumX10;        
  int16_t accelXx1000;      
  int16_t accelYx1000;
  int16_t accelZx1000;
  int16_t gyroXx10;         
  int16_t gyroYx10;
  int16_t gyroZx10;
};

// ── PKT2: GPS + BMP280 + Sun ──────────────────────────────────
// 1 + 4+4 + 2+2 + 2+2+2 + 1+2 = 22 bytes
struct Packet2 {
  uint8_t  id;             
  float    lat;            
  float    lon;            
  int16_t  gpsAlt;          
  int16_t  speedX10;       
  uint16_t pressureX10;     
  int16_t  bmpTempX100;     
  int16_t  bmpAlt;          
  uint8_t  sunIntensity;    
  int16_t  sunAngleX10;     
};

// ── PKT3: Magnetometer + Mission flag ─────────────────────────
// 1 + 2+2+2 + 2 + 20 = 29 bytes
struct Packet3 {
  uint8_t  id;              
  int16_t  magX;            
  int16_t  magY;            
  int16_t  magZ;           
  uint16_t headingX10;     
  char     mission[20];     
};
