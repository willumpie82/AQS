#include "CO2_sensor.h"

Adafruit_SCD30 scd30;

#define CO2_INTERVAL 120000 //ms

uint8_t CO2_samples = 3600 / g_pms_report_period;

AQSSensor SCD30_temperature("Temperature", device_name, CO2_samples);
AQSSensor SCD30_Humidity("Humidity", device_name, CO2_samples);
AQSSensor SCD30_CO2("CO2", device_name, CO2_samples);

uint32_t CO2_lastUpdate = 0;

void initCO2sensor(void)
{
    Serial.print("Init SCD30:");
    if (!scd30.begin(true))
    {
        Serial.println("Failed to find SCD30 device");
    }

    addSensorToList(&SCD30_temperature);
    addSensorToList(&SCD30_Humidity);
    addSensorToList(&SCD30_CO2);

    Serial.println(" OK");
}

void handleCO2sensor(void)
{
    uint32_t currentmillis = millis();

    if (currentmillis - CO2_lastUpdate > CO2_INTERVAL)
    {
        SCD30_temperature.set(uint32_t(scd30.temperature));
        SCD30_Humidity.set(uint32_t(scd30.relative_humidity));
        SCD30_CO2.set(uint32_t(scd30.CO2));

        SCD30_temperature.publish();
        SCD30_Humidity.publish();
        SCD30_CO2.publish();
    }
}