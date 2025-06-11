/**
 * @file Rele.h
 * @brief Clase para controlar un relé
 * @details
 * Esta clase permite controlar un relé conectado a un pin específico del ESP32.
 * Proporciona métodos para inicializar el relé y establecer su estado (activo o inactivo).
 * * El relé se activa con un valor lógico alto (HIGH) y se desactiva con un valor lógico bajo (LOW).
 */
#ifndef RELE_H
#define RELE_H

// Librerías
#include <Arduino.h>

/**
 * @class Rele
 * @brief Clase para controlar un relé
 * 
 * Esta clase permite controlar un relé conectado a un pin específico del ESP32.
 * Proporciona métodos para inicializar el relé y establecer su estado (activo o inactivo).
 */
class Rele {
private:
    int pin; ///< Pin al que está conectado el relé

public:
    /**
     * @brief Constructor de la clase Rele
     * @param pin Pin al que está conectado el relé
     */
    Rele(int pin) {
        this->pin = pin;
    }

    /**
     * @brief Inicializa el relé
     * 
     * Configura el pin del relé como salida y lo apaga inicialmente.
     */
    void begin() {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH); // Asumimos activo bajo (apagado por defecto)
    }

    /**
     * @brief Establece el estado del relé
     * @param activo Estado del relé (true: activo, false: inactivo)
     */
    void setState(bool activo) {
        digitalWrite(pin, activo ? HIGH : LOW); // Prueba lógica activa alta
    }
};

#endif // RELE_H
