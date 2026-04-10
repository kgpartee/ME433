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

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    while (true) {
        // clear
        // pixel on 
        // pixel off 

        sleep_ms(1000);
    }
}


// move to display. and display.h
// copy i2c init
//copy / generalize the 
void drawMessage(uint8_t x, uint8_t y, char* message){
    // loop through each char of message until null character is hit
    // draw each letter
    // offset x nad y, check to see if next row is hit 
    int i = 0;
    while(message[i] !=0){
        for (int j = 0; j < 10; j++){
            drawLetter(message[i], x+i*5, y+j*8);
            i++;
        }

    }

}

void drawLetter(uint8_t x, uint8_t y, char letter){
    for(int i=0; i<=4; i++){
        char col = ASCII(letter - 0x20)(i);
        for(int j = 0; j<=7; j++){
            if ((col>>j) &0b1== 0b1){
                ssd1306_drawPixel(x+i, y+j, 1);
            }
            else{ 
                ssd1306_drawPixel(x+i, y+j, 0);
            }
        }
    }
}