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
    sleep_ms(10000);
    // I2C Initialisation. Using it at 1700Khz.
    printf("init all\n");
    i2c_init(I2C_PORT, 1700*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    printf("init i2c\n");
    ssd1306_setup();
    imu_init();
    gpio_init(16);
    gpio_set_dir(16, GPIO_OUT);

    printf("init everything else\n");
    ssd1306_clear();
    ssd1306_update(); 

    while(true){
        // char message[10] = "in loop";
        // drawMessage(10, 10, message);
        // ssd1306_update();
           // clear
        // pixel on 
        // pixel off 
        unsigned int start = to_us_since_boot(get_absolute_time());
        int i = 15;
        char message[50]; 
        sprintf(message, "i2cgood%d", 12345678); 
        drawMessage(0,0,message);
        drawMessage(0,8, message);
        drawMessage(0,16, message);
        drawMessage(0, 24, message);
        ssd1306_update();
        unsigned int end = to_us_since_boot(get_absolute_time());
        sprintf(message, "fps: %f", (end-start)/1000.0);
        drawMessage(0, 24, message);
        ssd1306_update();
        // gpio_put(16, 1);
        // sleep_ms(1000);

        // gpio_put(16, 0);
        // sleep_ms(1000);
        // sleep_ms(3000);
        uint8_t buf[14];
        int16_t data[7];
        read_data(ACCEL_XOUT_H, buf);
        recombination(buf, data);
        
        printf("accel = (%d, %d, %d)\r\n temp = %d \r\n gyro = (%d, %d, %d)\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6]);
        
        //num_to_line(data[0], data[1]);
        // for (int i = 1; i <+14; i++){
        //     printf("%d: %d\n", i, buf[i]);
        // }
        // gpio_put(16, 1);
        // sleep_ms(700);

        // gpio_put(16, 0);
        // sleep_ms(700);
    }

    
}
