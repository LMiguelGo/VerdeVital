/**
 * @file sensor_DHT.h
 * @brief Clase para manejar sensores DHT (temperatura y humedad).
 * Esta clase utiliza la biblioteca DHT para leer datos de temperatura y humedad.
 * @details
 * La clase SensorDHT encapsula la funcionalidad de un sensor DHT, permitiendo
 * inicializar el sensor, leer la temperatura y la humedad, y manejar
 * los valores leídos. Los valores de temperatura y humedad se almacenan
 * en variables privadas y se pueden acceder a través de métodos públicos.
 */

#ifndef SENSOR_DHT_H
#define SENSOR_DHT_H

// Librerías
#include <Arduino.h>
#include "DHT.h"

/**
    * @class SensorDHT
    * @brief Clase para manejar sensores DHT (temperatura y humedad).
    * Esta clase utiliza la biblioteca DHT para leer datos de temperatura y humedad.
    * @details
    * La clase SensorDHT encapsula la funcionalidad de un sensor DHT, permitiendo
    * inicializar el sensor, leer la temperatura y la humedad, y manejar
    * los valores leídos. Los valores de temperatura y humedad se almacenan
    * en variables privadas y se pueden acceder a través de métodos públicos.
 */
class SensorDHT {
private:
    DHT dht; ///< Objeto DHT para manejar el sensor
    float lastTemperature; ///< Última temperatura leída
    float lastHumidity; ///< Última humedad leída

public:
    /**
     * @brief Constructor de la clase SensorDHT.
     * @param pin Pin al que está conectado el sensor DHT.
     * @param type Tipo de sensor DHT (DHT11, DHT22, etc.).
     */
    SensorDHT(uint8_t pin, uint8_t type) : dht(pin, type), lastTemperature(0.0f), lastHumidity(0.0f) {}

    /**
     * @brief Inicializa el sensor DHT.
     * @details Debe ser llamado antes de leer datos del sensor.
     */
    void begin() {
        dht.begin();
    }

    /**
     * @brief Lee la temperatura del sensor DHT.
     * @return Última temperatura leída en grados Celsius.
     * @details Si la lectura falla, devuelve el último valor válido.
     */
    float readTemperature() {
        float temp = dht.readTemperature();
        if (!isnan(temp)) lastTemperature = temp;
        return lastTemperature;
    }

    /**
     * @brief Lee la humedad del sensor DHT.
     * @return Última humedad leída en porcentaje.
     * @details Si la lectura falla, devuelve el último valor válido.
     */
    float readHumidity() {
        float hum = dht.readHumidity();
        if (!isnan(hum)) lastHumidity = hum;
        return lastHumidity;
    }
};

#endif // SENSOR_DHT_H
