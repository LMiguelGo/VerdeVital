#ifndef SENSOR_MQ135_H
#define SENSOR_MQ135_H

#include <Arduino.h>

class SensorMQ135 {
private:
    uint8_t _pin;

public:
    SensorMQ135(uint8_t pin) : _pin(pin) {
        pinMode(_pin, INPUT);
    }

    void begin() {
        Serial.println("🔧 MQ135 inicializado (modo simple)");
    }

    float readCO2() {
        int rawValue = analogRead(_pin);
        return (float)rawValue;  // Se interpreta directamente como valor "ppm"
    }
};

#endif
