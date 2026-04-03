#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
//brown ground, red power, orange signal
// run 5V

#define MOTORPIN 15
void set_servo(float angle);

int main()
{
    stdio_init_all();
    // initialize pwm , div 50 wrap 60000
    gpio_set_function(MOTORPIN, GPIO_FUNC_PWM); // Set the LED Pin to be PWM
    uint slice_num = pwm_gpio_to_slice_num(MOTORPIN); // Get PWM slice number
    // the clock frequency is 150MHz divided by a float from 1 to 255
    float div = 50; // must be between 1-255
    pwm_set_clkdiv(slice_num, div); // sets the clock speed
    uint16_t wrap = 60000; // when to rollover, must be less than 65535
    // set the PWM frequency and resolution
    // this sets the PWM frequency to 150MHz / div / wrap
    pwm_set_wrap(slice_num, wrap);
    pwm_set_enabled(slice_num, true); // turn on the PWM
   

    while (true) {
        // set index
        int i = 10;
        if (i < 170){
            set_servo(i)
        }
        else {
            i = 10
        }
        // loop through degree values in set servo function
    }
}

void set_servo(float angle){
    int level = (5 + ((angle - 10) * 5 / 160)) / 100;
    pwm_set_gpio_level(MOTORPIN, 60000 * level); // set the duty cycle to 50%
}
// write set servo with pwm, convert angle to wrap number for percent duty cycle 
