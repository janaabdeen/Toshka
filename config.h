#pragma once

#define DEBUG

// -- NRF24L01 pins ---------------------------------------------
#define NRF_CE_PIN          9
#define NRF_CSN_PIN         10

// -- SD card CS pin (shares SPI bus D11/D12/D13 with NRF) ------
#define SD_CS_PIN           8

// -- GPS SoftwareSerial pins -----------------------------------
#define GPS_RX_PIN          4
#define GPS_TX_PIN          3
#define GPS_BAUD            9600

// -- BPW34 photodiode analog pin -------------------------------
//#define BPW34_PIN           A0

// -- I2C addresses ---------------------------------------------
#define HMC5883L_ADDR       0x1E
#define BMP280_ADDR         0x76

// -- NRF pipe addresses ----------------------------------------
const byte ADDRESS_A[6] = "KAMAL";   
const byte ADDRESS_B[6] = "KEMAL";   

// -- Timing (milliseconds) -------------------------------------
#define RX_WINDOW_MS        50UL      
#define GPS_WINDOW_MS       50UL     
#define PKT_GAP_MS          10UL      
#define MODE_SETTLE_MS      10UL      
#define SENSOR_RETRY_MS     500UL     
#define SD_RETRY_MS         1000UL    
#define SD_MAX_RETRIES      10        
#define SD_FLUSH_EVERY      10       

// -- Sun simulation --------------------------------------------

#define SUN_AZIMUTH         180.0f

// -- Scale factors for integer compression ---------------------
#define SCALE_ACCEL         1000     
#define SCALE_GYRO          10       
#define SCALE_DHT           10        
#define SCALE_PRESSURE      10       
#define SCALE_BMP_TEMP      100       
#define SCALE_SPEED         10        
#define SCALE_HEADING       10        
#define SCALE_SUN_ANGLE     10        

