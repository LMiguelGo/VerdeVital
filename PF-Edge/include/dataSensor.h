#ifndef DATASENSOR_H
#define DATASENSOR_H
#include <cstdint>

struct SensorData {
    float temperature = 0.0f;
    float humidity = 0.0f;
    uint16_t light = 0;
    float co2ppm = 0.0f;
    float soilMoisture = 0.0f;
    float voltage = 0.0f;
};

#endif // DATASENSOR_H
