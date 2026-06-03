#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "math.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PIN_MISO 16
#define PIN_CS_DAC   17
#define PIN_SCK  18
#define PIN_MOSI 19
// add second cs pin
#define PIN_CS_RAM 20
// copy over hw 7 functions 

union FloatInt{
    float f;
    uint32_t i;
};

void setup_spi();
void write_voltage(int channel, float voltage);


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

// wire hold high 

//ram inint
void spi_ram_init(); 
void float_to_int(union FloatInt num, uint8_t msg[4]);
void spi_ram_write(uint16_t, uint8_t *, int);
void spi_ram_read(uint16_t, uint8_t *, int);
void ram_write_sine();

void update_dac_from_ram(int); 
//ram read int16, in8*, int
//ram write int16, in8*, int

//update dac from ram 

int main()
{
    stdio_init_all();

    // initialize cs pins and set high 

    // SPI initialisation. This example will use SPI at 1MHz.
   setup_spi();
    
    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_set_dir(PIN_CS_DAC, GPIO_OUT);
    gpio_put(PIN_CS_DAC, 1);
    gpio_set_dir(PIN_CS_RAM, GPIO_OUT);
    gpio_put(PIN_CS_RAM, 1);
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi
    spi_ram_init();
    ram_write_sine();
    int i = 0; 
    while (true) {
        // update dac from ram 
        // for i 0 to 1000, update them sleep 1 ms

        for (i = 0; i < 1024*2; i+=2){
            update_dac_from_ram(i);
            sleep_ms(1);
        }
    
    }
}


void write_voltage(int channel, float voltage){
 uint8_t data[2];
    uint16_t data_short = 0; 

    
    data_short = data_short| ((channel &0b1)<<15); // put channel in position
    data_short = data_short | (0b111<<12);
    uint16_t volt_analog = voltage / 3.3 * 1024; 
    data_short = data_short | (volt_analog << 2);


    data[0] = data_short >> 8;  
    data[1] = data_short & 0xFF;
    // map float to uint8 3.3 to 1023
    cs_select(PIN_CS_DAC);
    spi_write_blocking(spi0, data, 2); // where data is a uint8_t array with length len
    cs_deselect(PIN_CS_DAC);
}

void setup_spi(){
        spi_init(SPI_PORT, 1000*1000);
        gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
        gpio_set_function(PIN_CS_DAC,   GPIO_FUNC_SIO);
        gpio_set_function(PIN_CS_RAM, GPIO_FUNC_SIO);
        gpio_set_function(PIN_SCK,  GPIO_FUNC_SPI);
        gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
}


void spi_ram_init(){
    uint8_t data[2];
    int len = 2; 
    data[0] = 0b00000001;
    data[1] = 0b01000000; 
    cs_select(PIN_CS_RAM);
    spi_write_blocking(SPI_PORT, data, len);
    cs_deselect(PIN_CS_RAM);


// data 0 data 1
// cs low
// write to ram
// cs high
}

void spi_ram_write(uint16_t addr, uint8_t *data, int len){
    uint8_t packet[5];
    packet[0] = 0b00000010; 
    packet[1] = addr >> 8; 
    packet[2] = addr&0xFF;
    packet[3] = data[0];
    packet[4] = data[1];

    cs_select(PIN_CS_RAM);
    spi_write_blocking(SPI_PORT, packet, 5);
    cs_deselect(PIN_CS_RAM);
}


void spi_ram_read(uint16_t addr, uint8_t *data, int len){
    uint8_t packet[5];
    packet[0] = 0b00000011; 
    packet[1] = addr >> 8; 
    packet[2] = addr&0xFF;
    packet[3] = data[0];
    packet[4] = data[1];

    uint8_t dst[5];
    cs_select(PIN_CS_RAM);
    spi_write_read_blocking(SPI_PORT, packet, dst, 5);
    cs_deselect(PIN_CS_RAM);
    data[0] = dst[3];
    data[1] = dst[4];

}
//ram inint
void float_to_int(union FloatInt num, uint8_t *msg){
    
    msg[0] = (num.i>>24)&0xFF;
    msg[1] = (num.i>>16)&0xFF;
    msg[2] = (num.i>>8)&0xFF;
    msg[3] = (num.i)&0xFF;

}


// ram write
// write 5 byte ppackets first byte is instruction (0b00000010)to hw_write_maskedsecond and thirds are halves of address
// addr>>8; addr&0xFF to separate into two bytes
// last two are data[0] and data[1] 
// cs selsct
// spi write
// cs deselect

void ram_write_sine(){
    int i = 0;
    uint8_t data[2];
    uint16_t data_short = 0;
    uint8_t channel = 0b0;
    float voltage = 0;
    uint16_t addr = 0;

    for (i = 0; i < 1024; i++){
        data_short = (channel&0b1)<<15;
        data_short = data_short | (0b111<<12);

        voltage = (sin(2*3.14*i/1024)+1)*512;

        uint16_t v = voltage;

        data_short = data_short | (0b111111111111 & v);

        data[0] = data_short >> 8; 
        data[1] = data_short & 0xFF; 

        spi_ram_write(addr, data, 2);
        addr = addr + 2;

    }
}

void update_dac_from_ram(int i){
    uint8_t data[2];
    spi_ram_read(i, data, 2); 

    cs_select(PIN_CS_DAC);
    spi_write_blocking(SPI_PORT, data, 2);
    cs_deselect(PIN_CS_DAC);
}

// update dac from ram 
// read two bytes from rma 
// write two bits to dac 

// spi ram read(address data (pinter), len)
// write instruction (11)
// twll address
// send zeros 
// init buffer to store read data
// save buffer into given pointer 