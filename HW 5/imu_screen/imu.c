#include "imu.h"

void imu_init(){
    uint8_t pwrbuf[2] = {PWR_MGMT_1, 0x00};
    uint8_t accelbuf[2] = {ACCEL_CONFIG, 0x00}; // set value to 0 to select 2g mode
    uint8_t gyrobuf[2] = {GYRO_CONFIG, 0x03}; // set value to 3 to select 2000 dps mode
    i2c_write_blocking(i2c_default, IMU_ADDR, pwrbuf, 1, true);
    i2c_write_blocking(i2c_default, IMU_ADDR, accelbuf, 1, true);
    i2c_write_blocking(i2c_default, IMU_ADDR, gyrobuf, 1, true);
}
uint8_t read_data(uint8_t addr){
    uint8_t buf[14]; 
    i2c_write_blocking(i2c_default, IMU_ADDR, &addr, 1, true);  // true to keep host control of bus
    for (int ii = 0; ii < 14; ii++){
        i2c_read_blocking(i2c_default, IMU_ADDR, &buf+ii, 1, true);
    }
    return buf;  
}

int recombination(uint8_t buf[14]){
    int data [7];
    for (int ii = 0; ii < 13; ii+=2){
        data[ii/2] = (buf[ii]<<8)||buf[ii+1];
    }
    //fix units
    data[0] = data[0]*0.000061;
    data[1] = data[2]*0.000061;
    data[2] = data[3]*0.000061;

    data[3] = data[3]/340+36.53;
    data[4] = data[4]/340+36.53;

    data[5] = data[5]*0.007630;
    data[6] = data[6]*0.007630;
    data[7] = data[7]*0.007630;
}
// uint8_t readPin(int pinnum){
//     uint8_t buf = 0; 
//     i2c_write_blocking(i2c_default, CHIPADDR, &GPIO, 1, true);  // true to keep host control of bus
//     i2c_read_blocking(i2c_default, CHIPADDR, &buf, 1, false);  // false - finished with bus
//     return (buf>>pinnum)&1;
// }