#pragma once

#include "Arduino.h"
#include "config.h"
#include <vector>

#include "PMS.h"             // Particulate Matter Sensor driver (embedded)
#include "RunningMedian.h"
#include "sensor.h"

uint32_t getLCDvalue(void);

void initParticles( void );
void handleParticles( void );