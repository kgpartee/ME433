#include <stdio.h>
#include "pico/stdlib.h"

#include "font.h"
#include "ssd1306.h"

void drawMessage(uint8_t x, uint8_t y, char* message);
void drawLetter(uint8_t x, uint8_t y, char letter);