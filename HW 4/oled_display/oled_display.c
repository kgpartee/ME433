#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "font.h"
#include "ssd1306.h"
#include "display.h"
// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
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


    gpio_init(16);
    gpio_set_dir(16, GPIO_OUT);

    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c
    
    while (true) {
        // clear
        // pixel on 
        // pixel off 
        unsigned int start = to_us_since_boot(get_absolute_time());
        int i = 15;
        char message[50]; 
        sprintf(message, "bignumberreallybig%d", 12345678); 
        drawMessage(0,0,message);
        drawMessage(0,8, message);
        drawMessage(0,16, message);
        drawMessage(0, 24, message);
        ssd1306_update();
        unsigned int end = to_us_since_boot(get_absolute_time());
        sprintf(message, "fps: %f", (end-start)/1000.0);
        drawMessage(0, 24, message);
        ssd1306_update();
        gpio_put(16, 1);
        sleep_ms(1000);

        gpio_put(16, 0);
        sleep_ms(1000);
    }
}

