#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19
// add second cs pin

// copy over hw 7 functions 

// wire hold high 

//ram inint
//ram read int16, in8*, int
//ram write int16, in8*, int

//update dac from ram 

int main()
{
    stdio_init_all();

    // initialize cs pins and set high 

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS,   GPIO_FUNC_SIO);
    gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

    while (true) {
        // update dac from ram 
        // for i 0 to 1000, update them sleep 1 ms
        sleep_ms(1000);
    }
}

//ram inint
// data 0 data 1
// cs low
// write to ram
// cs high

// ram write
// write 5 byte ppackets first byte is instruction (0b00000010)to hw_write_maskedsecond and thirds are halves of address
// addr>>8; addr&0xFF to separate into two bytes
// last two are data[0] and data[1] 
// cs selsct
// spi write
// cs deselect

// ram write sine 
// (sin (2 pi * i/1024) + 1)*511
// write two bytes
// increment address by 2
// data_short = data_short | (0b1111111111 & (v<<2));
// ^ constrains it to a 10 bit number 
// data [0] = data_short >> 8;
// data [1] = data_short &0xFF


// update dac from ram 
// read two bytes from rma 
// write two bits to dac 

// spi ram read(address data (pinter), len)
// write instruction (11)
// twll address
// send zeros 
// init buffer to store read data
// save buffer into given pointer 