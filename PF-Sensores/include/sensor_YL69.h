/**
 * @file sensor_YL69.h
 * @brief Clase para manejar el sensor de humedad YL-69.
 * Esta clase permite leer el nivel de humedad del suelo utilizando el sensor YL-69.
 */

#ifndef SENSOR_YL69_H
#define SENSOR_YL69_H

// Librerías
#include <Arduino.h>

/**
 * @class SensorYL69
 * @brief Clase para manejar el sensor de humedad YL-69.
 * Esta clase permite leer el nivel de humedad del suelo utilizando el sensor YL-69.
 */
class SensorYL69 {
private:
    int pin; ///< Pin al que está conectado el sensor YL-69
    int valorSeco = 4095;   ///< Lectura cuando el suelo está totalmente seco (aire libre)
    int valorHumedo = 2200; ///< Ajusta este valor según tus pruebas en suelo completamente húmedo

public:
    /**
     * @brief Constructor de la clase SensorYL69.
     * @param pinSensor Pin al que está conectado el sensor YL-69.
     */
    SensorYL69(int pinSensor) : pin(pinSensor) {}

    /**
     * @brief Inicializa el sensor YL-69.
     * @details Debe ser llamado antes de leer datos del sensor.
     */
    void begin() {
        pinMode(pin, INPUT);
    }

    /**
     * @brief Lee el nivel de humedad del suelo.
     * @return Nivel de humedad en porcentaje (0% seco, 100% húmedo).
     * @details La lectura se limita al rango esperado y se convierte a porcentaje.
     */
    float readPercentage() {
        int lectura = analogRead(pin);
        // Limitar lectura al rango esperado
        lectura = constrain(lectura, valorHumedo, valorSeco);
        
        // Convertir a porcentaje: 0% seco, 100% húmedo
        float porcentaje = (float)(valorSeco - lectura) * 100.0 / (valorSeco - valorHumedo);
        return porcentaje;
    }
};

#endif // SENSOR_YL69_H
