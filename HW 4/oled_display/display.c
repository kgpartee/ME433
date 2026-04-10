#include <stdio.h>
#include "pico/stdlib.h"
#include "display.h"

void drawMessage(uint8_t x, uint8_t y, char* message){
    // loop through each char of message until null character is hit
    // draw each letter
    // offset x nad y, check to see if next row is hit 
    int i = 0;
    while(message[i] !=0){
        for (int j = 0; j < 10; j++){
            drawLetter(message[i], x+i*5, y+j*8);
            i++;
        }

    }

}

void drawLetter(uint8_t x, uint8_t y, char letter){
    for(int i=0; i<=4; i++){
        char col = ASCII(letter - 0x20)(i);
        for(int j = 0; j<=7; j++){
            if ((col>>j) &0b1== 0b1){
                ssd1306_drawPixel(x+i, y+j, 1);
            }
            else{ 
                ssd1306_drawPixel(x+i, y+j, 0);
            }
        }
    }
}