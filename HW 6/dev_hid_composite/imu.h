#ifndef IMU_H
#define IMU_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
// #include "ssd1306.h"
// #include "display.h"

#define IMU_ADDR 0b1101000

// config registers
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B

#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1  0x6B
#define PWR_MGMT_2 0x6C
// sensor data registers:
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H   0x41
#define TEMP_OUT_L   0x42
#define GYRO_XOUT_H  0x43
#define GYRO_XOUT_L  0x44
#define GYRO_YOUT_H  0x45
#define GYRO_YOUT_L  0x46
#define GYRO_ZOUT_H  0x47
#define GYRO_ZOUT_L  0x48


void imu_init();
void read_data(uint8_t addr, uint8_t* buf);
void recombination(uint8_t* buf, int16_t* data);
void num_to_line(int16_t accelx, int16_t accely);

#endif