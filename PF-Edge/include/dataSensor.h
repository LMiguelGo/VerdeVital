/**
 * @file dataSensor.h
 * @brief Este archivo define la estructura para almacenar los datos de los sensores del invernadero.
 */

#ifndef DATASENSOR_H
#define DATASENSOR_H

// Librerías
#include <cstdint>

/**
 * @struct SensorData
 * @brief Estructura para almacenar los datos de los sensores del invernadero.
 */
struct SensorData {
    float temperature = 0.0f;
    float humidity = 0.0f;
    uint16_t light = 0;
    float co2ppm = 0.0f;
    float soilMoisture = 0.0f;
    float voltage = 0.0f;
};

#endif // DATASENSOR_H
