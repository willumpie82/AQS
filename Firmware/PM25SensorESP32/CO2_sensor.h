#pragma once

#include "Arduino.h"
#include "config.h"

#include <Adafruit_SCD30.h>
#include <Adafruit_Sensor.h>

#include "sensor.h"


void initCO2sensor( void );
void handleCO2sensor( void );