#ifndef SENSOR_VOLTAGE_H
#define SENSOR_VOLTAGE_H

#include <Arduino.h>

class VoltageSensor {
private:
    int _adcPin;
    float _R1; // Resistencia superior (ohmios)
    float _R2; // Resistencia inferior (ohmios)
    float _vRef; // Voltaje de referencia ADC (e.g. 3.3V)
    int _adcMax; // Valor máximo ADC (e.g. 4095 para 12 bits)

public:
    VoltageSensor(int adcPin, float R1 = 30000.0f, float R2 = 7500.0f, float vRef = 3.3f, int adcMax = 4095)
    : _adcPin(adcPin), _R1(R1), _R2(R2), _vRef(vRef), _adcMax(adcMax) {
        pinMode(_adcPin, INPUT);
    }

    float readVoltage() {
        int adcValue = analogRead(_adcPin);
        float vOut = (adcValue / (float)_adcMax) * _vRef;
        float vIn = (vOut * (_R1 + _R2) / _R2)*1.02266;
        return vIn;
    }
};

#endif // SENSOR_VOLTAGE_H
