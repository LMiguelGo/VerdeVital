#ifndef RELE_H
#define RELE_H

#include <Arduino.h>

class Rele {
private:
    int pin;

public:
    Rele(int pin) {
        this->pin = pin;
    }

    void begin() {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH); // Asumimos activo bajo (apagado por defecto)
    }

    void setState(bool activo) {
        digitalWrite(pin, activo ? HIGH : LOW); // Prueba lógica activa alta
    }
};

#endif
