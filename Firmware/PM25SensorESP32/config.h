#include "Arduino.h"

/* ----------------- General Config -------------------------------- */
/* WiFi config */
#define WIFI_SSID                   "poes"   // Your WiFi SSID
#define WIFI_PASSWORD               "Rijnstraat214"     // Your WiFi password

/* MQTT */
#define mqtt_broker            "192.168.2.5" // IP address of your MQTT broker
#define     REPORT_MQTT_SEPARATE  true               // Report each value to its own topic
#define     REPORT_MQTT_JSON      true               // Report all values in a JSON message
#define status_topic           "events"        // MQTT topic to report startup
#define device_name            "AQS"

/* Particulate Matter Sensor */
#define    g_pms_warmup_period     30             // Seconds to warm up PMS before reading
#define    g_pms_report_period    120             // Seconds between reports

/* Use WiFi. If this is off, MQTT won't run */
#define    ENABLE_WIFI       true

/* Serial */
#define     SERIAL_BAUD_RATE       115200              // Speed for USB serial console

/* Environmental sensor */
#define SEA_LEVEL_PRESSURE_HPA (1013.25)

/* ----------------- Hardware-specific Config ---------------------- */
/* Mode button connection (momentary between this pin and GND) */
#define MODE_BUTTON_PIN    0

/* RGB LEDs */
#define WS2812B_PIN       13
#define NUM_RGB_LEDS       0

/* LCD */
#define TFT_DC_PIN         2
#define TFT_RST_PIN        4
#define TFT_CS_PIN        26
#define TFT_SCLK_PIN      18
#define TFT_MISO_PIN      19
#define TFT_MOSI_PIN      23

/* I2C */
#define I2C_SDA_PIN       21
#define I2C_SCL_PIN       22
//#define BME680_I2C_ADDR 0x76

/* Particulate Matter Sensor */
#define PMS_BAUD_RATE   9600
#define PMS_SET_PIN       25
#define PMS_RESET_PIN     26
#define PMS_RX_PIN        27 //  Tx to the PMS SHOULD BE 16
#define PMS_TX_PIN        14 //  Rx from the PMS SHOULD BE 17
