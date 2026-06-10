#include <stdio.h>
#include "pico/stdlib.h"
#include "encoder.h"
#include "hx711.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define STM32_ADDR 69

static struct repeating_timer timer1;
volatile float Kp_position = 0.1;

volatile float Kd_position = 0;

volatile uint16_t* desired_position;

volatile int last_desired_position = 0; 
volatile int last_pos = 0; 
volatile int last_error = 0;

volatile int num_desired_pos = 0;

volatile int des_current_out = 0.0; 
volatile int angle;

#define I2C_PORT0 i2c0
#define I2C_SDA0 8
#define I2C_SCL0 9

#define I2C_PORT1 i2c1
#define I2C_SDA1 14
#define I2C_SCL1 15


#define N 4000
#define TRI_AMP 35.0f
#define NUM_TRI 6
#define ZEROS 50

int16_t curve1[N]; // cw
int16_t curve2[N]; //ccw



void desired_current_init();

bool position_control(__unused struct repeating_timer *t){
    angle = read_angle();
    if (angle > 4000){
        angle = 4000; 
    }
    if (angle < 20){
        angle = 20; 
    }
    int32_t force = HX711_read(); 
    
    if (angle - last_pos > 0){
        desired_position = curve1; 
    }
    else{
        desired_position = curve2; 
    }
            
    static int eder = 0;
    int error = (int)desired_position[angle] - angle;

    eder = error - last_error;         

    des_current_out = Kp_position*error + Kd_position*eder;
    if (des_current_out>255){
        des_current_out = 255;
    }
    else if (des_current_out < -255){
        des_current_out = -255;
    }
    last_desired_position = (int)desired_position[angle];
    last_pos = angle; 
    last_error = error;
    uint8_t send[2];
    if (des_current_out < 0){
        send[0] = abs(des_current_out);
        send[1] = 0;
    }
    else{
        send[1] = abs(des_current_out);
        send[0] = 0;
    }

    i2c_write_blocking(I2C_PORT1, STM32_ADDR, send, 2, false);
    return true;

}


int main()
{
    stdio_init_all();
    add_repeating_timer_ms(1, position_control, NULL, &timer1);
    i2c_init(I2C_PORT0, 400*1000);
    
    gpio_set_function(I2C_SDA0, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL0, GPIO_FUNC_I2C);

    i2c_init(I2C_PORT1, 100*1000);
    
    gpio_set_function(I2C_SDA1, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL1, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA1);
    gpio_pull_up(I2C_SCL1);
    HX711_init();
    encoder_init();
    desired_current_init();
    for (int i = 0; i < 4000; i++){
            printf("(%d, %d)\n", curve1[1], curve2[i]);
        }
    while (true) {
       
        printf("pos: %d\n", angle);
        printf("sent: %d\n", des_current_out);
        sleep_ms(1000);
    }
}


void desired_current_init(){
 

    // -----------------------------------------
    // First 100 = -255
    // -----------------------------------------
    for (int i = 0; i < 100; i++) {
        curve1[i] = -255;
        curve2[i] = -255;
    }

    int idx = 100;

    // -----------------------------------------
    // Middle region: 6 triangles, each followed by 50 zeros
    // -----------------------------------------
    for (int i = 100; i < N - 100; i++) {
       
        curve1[i] = 0;
    }
    // int middle = N - 200;                 // 3800 samples
    // int block = middle / NUM_TRI;         // 633 samples per block
    // int tri_len = block - ZEROS;          // 583-sample triangle

    // for (int t = 0; t < NUM_TRI; t++) {

    //     // ---- Triangle (length tri_len) ----
    //     for (int i = 0; i < tri_len; i++) {

    //         float phase = (float)i / (tri_len - 1);   // 0 → 1
    //         float tri = 4.0f * fabsf(phase - 0.5f) - 1.0f;
    //         tri *= 0.98f;                             // soften edges
    //         curve1[idx++] = (int16_t)(tri * TRI_AMP);
    //     }

    //     // ---- 50 zeros ----
    //     for (int i = 0; i < ZEROS; i++) {
    //         curve1[idx++] = 0;
    //     }
    // }

    // -----------------------------------------
    // Last 100 = +255
    // -----------------------------------------
    for (int i = N - 100; i < N; i++) {
        curve1[i] = 255;
        curve2[i] = 255;
    }

    // -----------------------------------------
    // Curve 2: linear ramp -255 → +255
    // -----------------------------------------
    for (int i = 100; i < N - 100; i++) {
        float t = (float)(i - 100) / (float)(N - 200);
        curve2[i] = (int16_t)(0 + t * (255.0f));
    }

}

// take position measurement from encoder
// take force measurement from sensor to determine direction of pushing

// use force to determine which curve to follow
// use position to determine how much pwm to apply to the motor 

// send this information to the STM with some protocol 
