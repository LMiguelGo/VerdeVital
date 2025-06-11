/**
 * @file sensor_Voltage.h
 * @brief Clase para manejar sensores de voltaje.
 * Esta clase permite leer el voltaje de un divisor de voltaje conectado a un pin analógico.
 */
#ifndef SENSOR_VOLTAGE_H
#define SENSOR_VOLTAGE_H

// Librerías
#include <Arduino.h>

/**
 * @class VoltageSensor
 * @brief Clase para manejar sensores de voltaje.
 * Esta clase permite leer el voltaje de un divisor de voltaje conectado a un pin analógico.
 * @details
 * La clase VoltageSensor encapsula la funcionalidad de un sensor de voltaje,
 * permitiendo inicializar el sensor, leer el voltaje y manejar los valores leídos.
 * Los valores de resistencia y voltaje de referencia se pueden ajustar en el constructor.
 */
class VoltageSensor {
private:
    int _adcPin; ///< Pin al que está conectado el sensor de voltaje
    float _R1; ///< Resistencia superior (ohmios)
    float _R2; ///< Resistencia inferior (ohmios)
    float _vRef; ///< Voltaje de referencia ADC (e.g. 3.3V)
    int _adcMax; ///< Valor máximo ADC (e.g. 4095 para 12 bits)

public:
    /**
     * @brief Constructor de la clase VoltageSensor.
     * @param adcPin Pin al que está conectado el sensor de voltaje.
     * @param R1 Resistencia superior del divisor de voltaje (ohmios).
     * @param R2 Resistencia inferior del divisor de voltaje (ohmios).
     * @param vRef Voltaje de referencia del ADC (e.g. 3.3V).
     * @param adcMax Valor máximo del ADC (e.g. 4095 para 12 bits).
     */
    VoltageSensor(int adcPin, float R1 = 30000.0f, float R2 = 7500.0f, float vRef = 3.3f, int adcMax = 4095)
    : _adcPin(adcPin), _R1(R1), _R2(R2), _vRef(vRef), _adcMax(adcMax) {
        pinMode(_adcPin, INPUT);
    }

    /**
     * @brief Inicializa el sensor de voltaje.
     * @details Debe ser llamado antes de leer datos del sensor.
     */
    float readVoltage() {
        int adcValue = analogRead(_adcPin);
        float vOut = (adcValue / (float)_adcMax) * _vRef;
        float vIn = (vOut * (_R1 + _R2) / _R2)*1.02266;
        return vIn;
    }
};

#endif // SENSOR_VOLTAGE_H
