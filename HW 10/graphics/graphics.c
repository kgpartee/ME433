#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTONPIN 2
#define POTPIN

int main()
{
    stdio_init_all();

    // init button pin
    gpio_init(BUTTONPIN);
    gpio_set_dir(BUTTONPIN, GPIO_IN);
    // init potentiometer pin
    adc_init();
    adc_gpio_init(26); // pin GP26 is pin ADC0
    adc_select_input(0); // sample from ADC0

    while (true) {
        // digital read button for jumps
        int jump = gpio_get(BUTTONPIN);
        // read potentiometer pin
        int turn = adc_read();
    
        // print output

        printf("(%d,%d)", jump, turn);
    }
}
