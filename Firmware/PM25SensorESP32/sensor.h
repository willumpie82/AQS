#pragma once

#include "Arduino.h"
#include "mqtt.h"

class AQSSensor
{
    char _topic[50];
    char _topic_1h[50];
    char _topic_24h[255];
    const String _name;
    uint32_t _value;
    RunningMedian _average1h;
    RunningMedian _average24h;
    bool _publish = false;

public:
    AQSSensor(String name, String deviceID, uint8_t avgSamples) : _name(name),
                                                                  _average1h(avgSamples),
                                                                  _average24h(avgSamples * 24)
    {
        sprintf(_topic, "Sensors/%s/%s", deviceID.c_str(), name.c_str());
        sprintf(_topic_1h, "Sensors/%s/%s/1h/", deviceID.c_str(), name.c_str());
        sprintf(_topic_24h, "Sensors/%s/%s/24h/", deviceID.c_str(), name.c_str());
    }

    void publish(void)
    {
        _publish = true;
    }

    String getName(void)
    {
        return _name;
    }

    void publishMQTT(void)
    {
        if (_publish == true)
        {
            publishSingle(_value, _topic);
            publishSingle(_average1h.getMedian(), _topic_1h);
            publishSingle(_average24h.getMedian(), _topic_24h);
            _publish = false;
        }
    }

    virtual void set(uint32_t value)
    {
        _value = value;
        _average1h.add(_value);
        _average24h.add(_value);
    }

    uint32_t value() { return _value; }

    void print(void)
    {
        if (_publish)
        {
            Serial.printf("sensor:%s = %d\n", _name.c_str(), _value);
        }
    }
};

void addSensorToList(AQSSensor *sensor);
void reportToSerial(void);

void initSensor(void);
void handleSensor(void);