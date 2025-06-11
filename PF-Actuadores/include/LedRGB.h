/**
 * @file LedRGB.h
 * @brief Clase para controlar un LED RGB
 * @details
 * Esta clase permite controlar un LED RGB conectado a un pin específico del ESP32.
 * Proporciona métodos para encender y apagar el LED.
 */

#ifndef LED_RGB_H
#define LED_RGB_H

// Librerías
#include <Arduino.h>

/**
 * @class LedRGB
 * @brief Clase para controlar un LED RGB
 * 
 * Esta clase permite controlar un LED RGB conectado a un pin específico del ESP32.
 * Proporciona métodos para encender y apagar el LED.
 */
class LedRGB {
  private:
    int pin; ///< Pin al que está conectado el LED RGB

  public:
    /**
     * @brief Constructor de la clase LedRGB
     * @param pin Pin al que está conectado el LED RGB
     */
    LedRGB(int pin) {
      this->pin = pin;
    }
    /**
     * @brief Inicializa el LED RGB
     * 
     * Configura el pin del LED como salida y lo apaga inicialmente.
     */
    void begin() {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
    }
    /**
     * @brief Enciende el LED RGB
     * 
     * Establece el pin del LED a alto, encendiendo el LED.
     */
    void on() {
      digitalWrite(pin, HIGH);
    }
    /**
     * @brief Apaga el LED RGB
     * 
     * Establece el pin del LED a bajo, apagando el LED.
     */
    void off() {
      digitalWrite(pin, LOW);
    }
};

#endif // LED_RGB_H
