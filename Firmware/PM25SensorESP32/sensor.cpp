#include "sensor.h"


std::vector<AQSSensor*> sensorList;

void publishAll(void);

void reportToSerial(void)
{
  /* Report PM1.0 AE value */
  for( auto && sensor : sensorList)
  {
    sensor->print();
  }
}

void addSensorToList(AQSSensor* sensor)
{
  sensorList.push_back(sensor);
  Serial.printf("AQSSensor: added %s", sensor->getName().c_str());
}


void publishAll(void)
{
  for(auto && sensor : sensorList)
  {
    sensor->print();
    sensor->publishMQTT();
  }
}

void initSensor(void)
{
    Serial.print("InitSensor:");
    Serial.println(" OK");
}

void handleSensor(void)
{
  publishAll();
}