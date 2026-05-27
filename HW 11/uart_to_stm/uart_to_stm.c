#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart1
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 4
#define UART_RX_PIN 5



int main()
{
    stdio_init_all();
    sleep_ms(5000);
    // Set up our UART
    uart_init(UART_ID, BAUD_RATE);
    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    
    // Use some the various UART functions to send out data
    // In a default system, printf will also output via the default stdio (USB/UART0)
    
    // For more examples of UART use see https://github.com/raspberrypi/pico-examples/tree/master/uart
    char rx_data[100];

    while (true) {
        uart_puts(UART_ID, "100\n");
        printf("uart sent\n");
        int i = 0;
        while (uart_is_readable(UART_ID)) {
            printf("in the while loop\n");
            rx_data[i++] = uart_getc(UART_ID);
            if (uart_getc(UART_ID) == '\n'){
                break;
            }
            if (i= 100){
                break;
            }
        }
        printf("out of the loop\n");
        rx_data[i] = '\0';
        printf("Received on UART1: %s\n", rx_data);
       
        
    }
}
