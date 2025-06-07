#ifndef SENSOR_DHT_H
#define SENSOR_DHT_H

#include <Arduino.h>
#include "DHT.h"

class SensorDHT {
private:
    DHT dht;
    float lastTemperature;
    float lastHumidity;

public:
    SensorDHT(uint8_t pin, uint8_t type) : dht(pin, type), lastTemperature(0.0f), lastHumidity(0.0f) {}

    void begin() {
        dht.begin();
    }

    float readTemperature() {
        float temp = dht.readTemperature();
        if (!isnan(temp)) lastTemperature = temp;
        return lastTemperature;
    }

    float readHumidity() {
        float hum = dht.readHumidity();
        if (!isnan(hum)) lastHumidity = hum;
        return lastHumidity;
    }
};

#endif
