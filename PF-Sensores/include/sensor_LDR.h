/**
 * @file sensor_LDR.h
 * @brief Clase para manejar sensores LDR (Light Dependent Resistor).
 */

#ifndef SENSOR_LDR_H
#define SENSOR_LDR_H

// Librerías
#include <Arduino.h>

/**
 * @class SensorLDR
 * @brief Clase para manejar sensores LDR (Light Dependent Resistor).
 * Esta clase permite leer el valor de luz de un sensor LDR conectado a un pin analógico.
 */
class SensorLDR {
private:
    uint8_t pin; ///< Pin al que está conectado el sensor LDR
    unsigned short lastLight; ///< Último valor de luz leído

public:
    /**
     * @brief Constructor de la clase SensorLDR.
     * @param ldrPin Pin al que está conectado el sensor LDR.
     */
    SensorLDR(uint8_t ldrPin) : pin(ldrPin), lastLight(0) {}

    /**
     * @brief Inicializa el sensor LDR.
     * @details Debe ser llamado antes de leer datos del sensor.
     */
    void begin() {
        pinMode(pin, INPUT);
    }

    /**
     * @brief Lee el valor de luz del sensor LDR.
     * @return Último valor de luz leído (0-1023).
     * @details Si la lectura falla, devuelve el último valor válido.
     */
    unsigned short readLight() {
        lastLight = analogRead(pin);
        return lastLight;
    }
};

#endif // SENSOR_LDR_H
