
#include "hx711.h"


void hx711_init(){
    gpio_init(CLKPIN);
    gpio_init(DTPIN);

    gpio_set_dir(CLKPIN, GPIO_OUT);
    gpio_set_dir(DTPIN, GPIO_IN);
}

int data_read(){
    //read 24 bits from sensor
    // wait for DT pin low
    // blink CLK 25 times, read DT 24 times 
    // shift data nad convert to signed into
        // from assignment: 
        // if (raw & 0x800000) {
        // raw |= 0xFF000000;
    // }
}