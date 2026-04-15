#include "imu.h"

void imu_init(){
    uint8_t pwrbuf[2] = {PWR_MGMT_1, 0x00};
    uint8_t accelbuf[2] = {ACCEL_CONFIG, 0x00}; // set value to 0 to select 2g mode
    uint8_t gyrobuf[2] = {GYRO_CONFIG, 0x03}; // set value to 3 to select 2000 dps mode
    i2c_write_blocking(i2c_default, IMU_ADDR, pwrbuf, 1, true);
    i2c_write_blocking(i2c_default, IMU_ADDR, accelbuf, 1, true);
    i2c_write_blocking(i2c_default, IMU_ADDR, gyrobuf, 1, true);
}

void read_data(){
    uint8_t buf[14]; 
    i2c_write_blocking(i2c_default, IMU_ADDR, &ACCEL_XOUT_H, 1, true);  // true to keep host control of bus
    i2c_read_blocking(i2c_default, IMU_ADDR, &buf, 1, false);  // false - finis
}

// uint8_t readPin(int pinnum){
//     uint8_t buf = 0; 
//     i2c_write_blocking(i2c_default, CHIPADDR, &GPIO, 1, true);  // true to keep host control of bus
//     i2c_read_blocking(i2c_default, CHIPADDR, &buf, 1, false);  // false - finished with bus
//     return (buf>>pinnum)&1;
// }