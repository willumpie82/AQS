#pragma once

#include "Arduino.h"
#include "mqtt.h"
#include "config.h"

#include <WiFi.h>         // ESP32 WiFi driver
#include <PubSubClient.h> // For MQTT
#include <string>
#include "RunningMedian.h"

void initMQTT(void);
void handleMQTT(void);
bool initWifi();

uint32_t getDeviceID(void);

void publishSingle(uint32_t value, char *topic);