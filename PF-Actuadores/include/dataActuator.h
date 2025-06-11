/**
 * @file dataActuator.h
 * @brief Definición de la estructura ActuatorState para representar el estado de los actuadores
 * @details
 * Esta estructura contiene los estados de los actuadores como la bomba de agua, el ventilador y los LEDs.
 * * Cada actuador se representa con un valor de 0 (apagado) o 1 (encendido).
 */

#ifndef DATA_ACTUATOR_H
#define DATA_ACTUATOR_H

// Librerías
#include <cstdint>

/**
 * @struct ActuatorState
 * @brief Estructura para representar el estado de los actuadores
 * 
 * Esta estructura contiene los estados de los actuadores como la bomba de agua, el ventilador y los LEDs.
 * Cada actuador se representa con un valor de 0 (apagado) o 1 (encendido).
 */
struct ActuatorState {
    uint8_t waterPump = 0; ///< Estado de la bomba de agua (0: apagada, 1: encendida)
    uint8_t fan = 0;       ///< Estado del ventilador (0: apagado, 1: encendido)
    uint8_t leds = 0;      ///< Estado de los LEDs (0: apagados, 1: encendidos)
};

#endif // DATA_ACTUATOR_H
