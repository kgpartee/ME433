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

int main()
{
    stdio_init_all();
    // I2C Initialisation. Using it at 1700Khz.
    i2c_init(I2C_PORT, 1700*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);



    ssd1306_setup();




    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
