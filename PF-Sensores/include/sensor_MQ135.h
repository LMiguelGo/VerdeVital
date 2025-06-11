/**
 * @file sensor_MQ135.h
 * @brief Clase para manejar el sensor MQ135 (calidad del aire).
 * Esta clase permite leer el valor de CO2 del sensor MQ135 conectado a un pin analógico.
 */

#ifndef SENSOR_MQ135_H
#define SENSOR_MQ135_H

// Librerías
#include <Arduino.h>

/**
 * @class SensorMQ135
 * @brief Clase para manejar el sensor MQ135 (calidad del aire).
 * Esta clase permite leer el valor de CO2 del sensor MQ135 conectado a un pin analógico.
 * @details
 * La clase SensorMQ135 encapsula la funcionalidad de un sensor MQ135, permitiendo
 * inicializar el sensor y leer el valor de CO2 en partes por millón (ppm).
 */
class SensorMQ135 {
private:
    uint8_t _pin; ///< Pin al que está conectado el sensor MQ135

public:
    /**
     * @brief Constructor de la clase SensorMQ135.
     * @param pin Pin al que está conectado el sensor MQ135.
     */
    SensorMQ135(uint8_t pin) : _pin(pin) {
        pinMode(_pin, INPUT);
    }

    /**
     * @brief Inicializa el sensor MQ135.
     * @details Debe ser llamado antes de leer datos del sensor.
     * En este modo simple, no se realiza calibración ni ajustes adicionales.
     */
    void begin() {
        Serial.println("🔧 MQ135 inicializado (modo simple)");
    }

    /**
     * @brief Lee el valor de CO2 del sensor MQ135.
     * @return Valor de CO2 en partes por millón (ppm).
     * @details La lectura se realiza directamente desde el pin analógico.
     *          El valor se interpreta como un valor "ppm" sin calibración adicional.
     */
    float readCO2() {
        int rawValue = analogRead(_pin);
        return (float)rawValue;  // Se interpreta directamente como valor "ppm"
    }
};

#endif // SENSOR_MQ135_H
