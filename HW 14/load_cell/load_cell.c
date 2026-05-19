#include <stdio.h>
#include "pico/stdlib.h"

#define CLKPIN
#define DTPIN

int main()
{
    stdio_init_all();

    // init chip 
    
    while (true) {

        //wait for number of samples to collect 
        // for 
        // save raw value
        // save iir filtered value
        // save time in ms 
        // print al data 
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

