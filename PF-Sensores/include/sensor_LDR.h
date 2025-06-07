#ifndef SENSOR_LDR_H
#define SENSOR_LDR_H

#include <Arduino.h>

class SensorLDR {
private:
    uint8_t pin;
    unsigned short lastLight;

public:
    SensorLDR(uint8_t ldrPin) : pin(ldrPin), lastLight(0) {}

    void begin() {
        pinMode(pin, INPUT);
    }

    unsigned short readLight() {
        lastLight = analogRead(pin);
        return lastLight;
    }
};

#endif
