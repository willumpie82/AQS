#pragma once

#include "Arduino.h"
#include "particles.h"

void initLCD();
void handleLCD();

void plotNeedle(int16_t angle, uint16_t ms_delay, uint32_t value);
