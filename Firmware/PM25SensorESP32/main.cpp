/*
  Particulate Matter Sensor firmware (ESP32 version)

  Written by Jonathan Oxer for www.superhouse.tv
   https://github.com/superhouse/PM25SensorESP32

  refactored by Willem Oldemans

  Read from a Plantower PMS7003 particulate matter sensor and a BME680
  environmental sensosr using an ESP32, and report the values to an MQTT
  broker and show them on a 240x240 LCD, with a mode button to change
  between display modes.

  External dependencies. Install using the Arduino library manager:
     "Adafruit GFX Library" by Adafruit
     TFT_eSPI.h
     Adafruit_NeoPixel.h
     Adafruit_Sensor.h
     Adafruit_BME680.h
     AdaFruit_SCD30.h
     "PubSubClient" by Nick O'Leary
     RunningMedian

  Bundled dependencies. No need to install separately:
     "PMS Library" by Mariusz Kacki, forked by SwapBap

  Inspired by https://github.com/SwapBap/WemosDustSensor/
*/
#define VERSION "3"

/*--------------------------- Configuration ------------------------------*/
// Configuration should be done in the included file:
#include "Arduino.h"
#include "config.h"

/*--------------------------- Modules ------------------------------------*/
#include "lcd.h"
#include "mqtt.h"
#include "particles.h"
#include "CO2_sensor.h"
#include "sensor.h"
#include "button.h"

/*--------------------------- Program ---------------------------------------*/
/**
  Setup
*/
void setup()
{
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println();
  Serial.print("Air Quality Sensor starting up, v");
  Serial.println(VERSION);

  initWifi();
  initMQTT();
  initSensor();
  initParticles();
  initCO2sensor();
  initLCD();
  initButtons();
}

/**
  Main loop
*/
void loop()
{
  handleParticles();
  handleCO2sensor();
  handleButtons();
  handleSensor();
  handleLCD();
  handleMQTT();
}
