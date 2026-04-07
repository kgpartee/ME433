#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

#define LED 7 // on expander 
#define BUTTON 0 // on expander
#define ERROR 16

uint8_t IODIR = 0x00;
uint8_t GPIO = 0x09;
uint8_t OLAT = 0x0A;
uint8_t PINMASK = 0b01111111;
uint8_t CHIPADDR = 0b0100000;


// step 1: make LED blink
// step 2: read button
// step 3: read from button to control LED 
void setPin(uint16_t pinaddr, uint8_t value); // sets value of pin
uint8_t readPin(int pinnum);
void initExpander();
void initI2C();

int main()
{
    stdio_init_all();
    initI2C();
    initExpander();

    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    while (true) {
        setPin(7, 1);
        sleep_ms(500);
        setPin(7, 0);
        sleep_ms(500);
        // writing : i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
        // reading: i2c_write_blocking(i2c_default, ADDR, &reg, 1, true);  // true to keep host control of bus
        //          i2c_read_blocking(i2c_default, ADDR, &buf, 1, false);  // false - finished with bus
    }
}

void setPin(uint16_t pinaddr, uint8_t value) {
    uint8_t buf[2] = {OLAT, 0x00};
    if(value != 0){
        buf[1] = 00000001<<pinaddr;
    }
    i2c_write_blocking(i2c_default, CHIPADDR, buf, 2, false);
}

uint8_t readPin(int pinnum){
    uint8_t buf = 0; 
    i2c_write_blocking(i2c_default, CHIPADDR, &GPIO, 1, true);  // true to keep host control of bus
    i2c_read_blocking(i2c_default, CHIPADDR, &buf, 1, false);  // false - finished with bus
    return (buf>>pinnum)&1;
}

void initI2C(){
        // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    // added external pull up resistors: 
    // gpio_pull_up(I2C_SDA);
    // gpio_pull_up(I2C_SCL);

}
void initExpander(){
    // pulse reset on expander before starting code -- default should be pulled high, pull low to reset 
// set gpio pins to be on/off in iodir reg 
    uint8_t gpio_init[2] = {IODIR, PINMASK};
    i2c_write_blocking(i2c_default, CHIPADDR, gpio_init, 2, false);

}