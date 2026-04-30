#include "imu.h"

void imu_init(){
    uint8_t pwrbuf[2] = {PWR_MGMT_1, 0x00};
    uint8_t accelbuf[2] = {ACCEL_CONFIG, 0x00}; // set value to 0 to select 2g mode
    uint8_t gyrobuf[2] = {GYRO_CONFIG, 0x03}; // set value to 3 to select 2000 dps mode
    i2c_write_blocking(i2c_default, IMU_ADDR, pwrbuf, 1, true);
    i2c_write_blocking(i2c_default, IMU_ADDR, accelbuf, 1, true);
    i2c_write_blocking(i2c_default, IMU_ADDR, gyrobuf, 1, true);
}

void read_data(uint8_t addr, uint8_t* buf){ 
    i2c_write_blocking(i2c_default, IMU_ADDR, &addr, 1, true);  // true to keep host control of bus
    //for (int ii = 0; ii < 14; ii++){
        i2c_read_blocking(i2c_default, IMU_ADDR, buf, 14, false);
    //}
}

void recombination(uint8_t* buf, int16_t* data){
    int16_t placeholder = 0; 
    for (int ii = 0; ii < 14; ii+=2){
        placeholder = 0; 
        placeholder = buf[ii]<<8;
       printf("placeholder:%d OR %d\n", placeholder, buf[ii+1]);
        // THE PROBLEM IS HERE
        data[ii/2] = placeholder|buf[ii+1];
        printf("data[%d] = %d\n", ii/2, data[ii/2]);
    }
    //fix units
    // data[0] = data[0]*0.000061;
    // data[1] = data[2]*0.000061;
    // data[2] = data[3]*0.000061;

    // data[3] = data[3]/340+36.53;
    
    // data[4] = data[7]*0.007630;
    // data[5] = data[5]*0.007630;
    // data[6] = data[6]*0.007630;
    
}
// uint8_t readPin(int pinnum){
//     uint8_t buf = 0; 
//     i2c_write_blocking(i2c_default, CHIPADDR, &GPIO, 1, true);  // true to keep host control of bus
//     i2c_read_blocking(i2c_default, CHIPADDR, &buf, 1, false);  // false - finished with bus
//     return (buf>>pinnum)&1;
// }

void num_to_line(int16_t accelx, int16_t accely){
    ssd1306_clear();
     // start in center of screen (64, 16)
    // map accel from 150 to 16500 --> 260 per pixel in x, 1000 per pixel in y
    int line_lim_x = accelx / 260;
    int line_lim_y = accely / 1000;
    if (line_lim_x>=0){
        for(int i = 0; i<line_lim_x; i++){
            ssd1306_drawPixel(64-i, 16, 1);
        }
    }
    else{
        for(int i = 0; i>line_lim_x; i--){
            ssd1306_drawPixel(64-i, 16, 1);
        }
    }
    if (line_lim_y>=0){
        for(int j = 0; j<line_lim_y; j++){
            ssd1306_drawPixel(64, 16+j, 1);
        }
    }
    else{
        for(int j = 0; j>line_lim_y; j--){
            ssd1306_drawPixel(64, 16+j, 1);
        }
    }
    ssd1306_update();
}