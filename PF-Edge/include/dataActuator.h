#ifndef DATA_ACTUATOR_H
#define DATA_ACTUATOR_H

#include <cstdint>

struct ActuatorState {
    uint8_t waterPump = 0;   // 0 = OFF, 1 = ON
    uint8_t fan = 0;         // 0 = OFF, 1 = ON
    uint8_t leds = 0;        // 0 = OFF, 1 = ON
};

#endif // DATA_ACTUATOR_H
