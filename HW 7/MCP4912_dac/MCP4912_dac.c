#include <stdio.h>
#include "pico/stdlib.h"
//add math.h
// init spi
//.include command function
// sine wave and triangle wave 0 to 3.3V at same time
// steps: Write a function that takes the channel and the voltage (as an unsigned short, 0-1023) as inputs. 
// At first, just hard code the entire 16 bits to send to the DAC, with something like 1.65V as the output, to test your SPI writes. 
// Then actually use the channel and voltage.

// make sure to plug sdi-sdo and sdo-sdi 
// vref a and vref b to 3.3. 
// vdd also 3.3
// vss gnd 
// stie ldac low, tie shdw high 

// first four bits we write are always 1


void write_voltage(int channel, unsigned short voltage);

int main()
{

    or make tabel with 100 values where we precalculate sine values and just loop through table 
    float t = 0; 
    t = t+0.01; 
    stdio_init_all();
    float voltageA = 3.3/2*(sine(2*pi*f*t)+1);
    while (true) {
        // update dac
            //set voltage a
            //set voltage b
        // sleep for 0.01 s
        //
    }
}


// pseudo code from class:
void write_voltage(int channel, float voltage){
    uint8_t data[2];
    data[0] = 0b01110000; 
    data[0] = data[0]| ((channel &0b1)<<7); // put channel in position
    uint16_t volt_analog = voltage / 3.3 * 1023; 

    data[0] = data[0] | (volt_analog >> 6); 
    data[1] = (volt_analog<<2) & 0xFF;
    // map float to uint8 3.3 to 1023
    sc_select(PIN_CS;
        spi_write_blocking
        cs_deselect
}