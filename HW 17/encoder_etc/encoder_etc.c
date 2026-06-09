#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 14
#define I2C_SCL 15

#define AS5600_ADDR 0x36
#define RAW_ANGLE 0x0C
#define ZPOS 0x01
#define MPOS 0x03
#define MAGNET 0x0B

void encoder_init();
int read_angle();

int main()
{
    stdio_init_all();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c
    int angle = 0;
    encoder_init();
    while (true) {
       angle = read_angle();
       printf("angle: %d\r\n", angle);
    }
}

// i2c_write_blocking(i2c_default, SSD1306_ADDRESS, buf, 2, false);
// i2c_read_blocking(i2c_default, IMU_ADDR, buf, 14, false);

void encoder_init(){
    uint8_t zero_angle_buf[2];
    uint8_t max_angle_buf[2];
    uint8_t md; 

    i2c_read_blocking(i2c_default, MAGNET, &md, 1, true);

    if(md != 32){
        printf("NO MAGNET!!!\n");
    }
    else{
        printf("magnet detected :D\n");
    }

    i2c_read_blocking(i2c_default, RAW_ANGLE, zero_angle_buf, 2, true);
    i2c_write_blocking(i2c_default, ZPOS, zero_angle_buf, 2, true);
    printf("zero pos done\n");
    sleep_ms(5000);
    printf("reading max pos\n");
    i2c_read_blocking(i2c_default, RAW_ANGLE, max_angle_buf, 2, true);
    i2c_write_blocking(i2c_default, MPOS, max_angle_buf, 2, false);
    printf("max pos done\n");
    // read raw angle from two consecutive 
    // write raw angle to ZPOS
    // wait 5 s
    // rotate to stop position
    // read raw angle
    // write to MPOS

}

int read_angle(){
    uint8_t buf[2];
    uint16_t angle = 0;
    i2c_read_blocking(i2c_default, RAW_ANGLE, buf, 2, true);
    angle = (buf[1]<<8)|buf[2];
    return angle; 
    //i2c read
    //concatenate bits
    //success
}