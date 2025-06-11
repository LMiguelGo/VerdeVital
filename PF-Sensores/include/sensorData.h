/**
 * @file sensorData.h
 * @brief Estructura para almacenar datos de sensores.
 * Esta estructura contiene campos para temperatura, humedad, luz, CO2, humedad del suelo y voltaje.
 */

#ifndef SENSORDATA_H
#define SENSORDATA_H

// Librerías
#include <cstdint>

/**
 * @struct SensorData
 * @brief Estructura para almacenar datos de sensores.
 * Esta estructura contiene campos para temperatura, humedad, luz, CO2, humedad del suelo y voltaje.
 * @details
 * - temperature: Temperatura en grados Celsius.
 * - humidity: Humedad relativa en porcentaje.
 * - light: Valor de luz (0-65535).
 * - co2ppm: Concentración de CO2 en partes por millón (ppm).
 * - soilMoisture: Humedad del suelo en porcentaje.
 * - voltage: Voltaje medido por el sensor.
 */
struct SensorData {
    float temperature = 0.0f;
    float humidity = 0.0f;
    uint16_t light = 0;
    float co2ppm = 0.0f;
    float soilMoisture = 0.0f;
    float voltage = 0.0f;
};

#endif // SENSORDATA_H
