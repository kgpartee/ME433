#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "math.h"
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
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS   17
#define PIN_SCK  18
#define PIN_MOSI 19

void write_voltage(int channel, float voltage);
void setup_spi(); 
int main()
{

    //or make tabel with 100 values where we precalculate sine values and just loop through table 
    
    stdio_init_all();
    setup_spi(); 

    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    float voltageA[100];
    float voltageB[200];
    voltageB[0] = 0;
    
    for(int i = 0; i < 100; i++){
        voltageA[i] = (sin(2*3.14*i/100)+1)*512;
    }    
    for(int j = 1; j<100; j++){
        
        voltageB[j] = voltageB[j -1] + 1.024;
    }
    for (int j = 100; j<200; j++){
        voltageB[j] = voltageB[j-1] - 1.024; 
    }

 

    while (true) {
       write_voltage(1, 1.65);
       sleep_ms(500);
       write_voltage(1, 5);
       sleep_ms(500);
        
        // for(int index = 0; index < 200; index++){
        //     if (index < 100){
            
        //     write_voltage(0, voltageA[index]);
        //     write_voltage(1, voltageB[index]);
        //     sleep_ms(5);
        // }
        // else {
        //     write_voltage(0, voltageA[index - 100]);
        //     write_voltage(1, voltageB[index]);
        //     sleep_ms(5);
        // }
        // }


    }
}

static inline void cs_select(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 0);
    asm volatile("nop \n nop \n nop"); // FIXME
}

static inline void cs_deselect(uint cs_pin) {
    asm volatile("nop \n nop \n nop"); // FIXME
    gpio_put(cs_pin, 1);
    asm volatile("nop \n nop \n nop"); // FIXME
}
// pseudo code from class:
void write_voltage(int channel, float voltage){
    uint8_t data[2];
    uint16_t data_short = 0; 

    
    data_short = data_short| ((channel &0b1)<<15); // put channel in position
    data_short = data_short | (0b111<<12);
    uint16_t volt_analog = voltage; 
    data_short = data_short | (volt_analog << 2);


    data[0] = data_short >> 8;  
    data[1] = data_short & 0xFF;
    // map float to uint8 3.3 to 1023
    cs_select(PIN_CS);
    spi_write_blocking(spi0, data, 2); // where data is a uint8_t array with length len
    cs_deselect(PIN_CS);
}

void setup_spi(){
    spi_init(spi_default, 100 * 1000); // the baud, or bits per second
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
}