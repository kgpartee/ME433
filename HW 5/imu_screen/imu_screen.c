#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "display.h"
#include "font.h"
#include "imu.h"
#include "ssd1306.h"

#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

uint8_t WHO_AM_I = 0x75;

int main()
{
    stdio_init_all();
    // I2C Initialisation. Using it at 1700Khz.
    i2c_init(I2C_PORT, 1700*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    ssd1306_setup();
    gpio_init(16);
    gpio_set_dir(16, GPIO_OUT);
    uint8_t buf = 0;

    i2c_write_blocking(i2c_default, IMU_ADDR, &WHO_AM_I, 1, true);  // true to keep host control of bus
    i2c_read_blocking(i2c_default, IMU_ADDR, &buf, 1, false);  // false - finished with bus

    if (buf == 0x68 || buf == 0x98){
        sleep_ms(10000);
        printf("buf = %d\n", buf);
    }
    else{
        while(true){
            gpio_put(16, 1);
            sleep_ms(1000);
            gpio_put(16, 0);
            sleep_ms(1000);
        }
    }
}
