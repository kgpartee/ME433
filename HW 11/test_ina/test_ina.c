#include <stdio.h>
#include "pico/stdlib.h"
#include "ina219.h"


int main()
{
    stdio_init_all();
    sleep_ms(3000);
    init_ina219(); 
    printf("initalized");
    while (true) {
        printf("in while");
        float current = read_ina219();
        printf("Current: %f\n", current);
        sleep_ms(10);
    }
}
