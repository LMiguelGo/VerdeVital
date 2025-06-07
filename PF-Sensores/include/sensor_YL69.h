#ifndef SENSOR_YL69_H
#define SENSOR_YL69_H

#include <Arduino.h>

class SensorYL69 {
private:
    int pin;
    int valorSeco = 4095;   // Lectura cuando el suelo está totalmente seco (aire libre)
    int valorHumedo = 2200; // Ajusta este valor según tus pruebas en suelo completamente húmedo

public:
    SensorYL69(int pinSensor) : pin(pinSensor) {}

    void begin() {
        pinMode(pin, INPUT);
    }

    float readPercentage() {
        int lectura = analogRead(pin);
        // Limitar lectura al rango esperado
        lectura = constrain(lectura, valorHumedo, valorSeco);
        
        // Convertir a porcentaje: 0% seco, 100% húmedo
        float porcentaje = (float)(valorSeco - lectura) * 100.0 / (valorSeco - valorHumedo);
        return porcentaje;
    }
};

#endif
