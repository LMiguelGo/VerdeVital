/**
 * @file dataActuator.h
 * @brief Este archivo 
 */

#ifndef DATA_ACTUATOR_H
#define DATA_ACTUATOR_H

// Librerías
#include <cstdint>

/**
 * @struct ActuatorState
 * @brief Estructura para almacenar el estado de los actuadores del invernadero.
 */
struct ActuatorState {
    uint8_t waterPump = 0; ///< Estado de la bomba de agua: 0 = apagada, 1 = encendida
    uint8_t fan = 0;       ///< Estado del ventilador: 0 = apagado, 1 = encendido
    uint8_t leds = 0;      ///< Estado de las luces LED: 0 = apagadas, 1 = encendidas
};

#endif // DATA_ACTUATOR_H
