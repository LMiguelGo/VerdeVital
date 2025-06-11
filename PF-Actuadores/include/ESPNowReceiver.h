/**
 * @file ESPNowActuatorReceiver.h
 * @brief Clase para recibir estados de actuadores a través de ESP-NOW
 * @details
 * Esta clase permite recibir estados de actuadores (bomba de agua, ventilador y LEDs)
 * a través de ESP-NOW. Utiliza una estructura `ActuatorState` para representar el estado de los actuadores.
 * El receptor se inicializa en un canal específico y permite registrar un callback para manejar los datos recibidos.
 */

#ifndef ESP_NOW_ACTUATOR_RECEIVER_H
#define ESP_NOW_ACTUATOR_RECEIVER_H

/// Librerías
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "dataActuator.h"

/**
 * @class ESPNowActuatorReceiver
 * @brief Clase para recibir estados de actuadores a través de ESP-NOW
 * 
 * Esta clase permite recibir estados de actuadores (bomba de agua, ventilador y LEDs)
 * a través de ESP-NOW. Utiliza una estructura `ActuatorState` para representar el estado de los actuadores.
 * El receptor se inicializa en un canal específico y permite registrar un callback para manejar los datos recibidos.
 */
class ESPNowActuatorReceiver {
private:
    uint8_t _channel; ///< Canal de comunicación ESP-NOW
    void (*_onReceiveCallback)(const ActuatorState&);  ///< Callback para manejar los datos recibidos

public:
    /**
     * @brief Constructor de la clase ESPNowActuatorReceiver
     * @param channel Canal de comunicación ESP-NOW (por defecto es 1)
     */
    ESPNowActuatorReceiver(uint8_t channel = 1) : _channel(channel), _onReceiveCallback(nullptr) {}

    /**
     * @brief Inicializa el receptor ESP-NOW
     */
    void begin() {
        WiFi.mode(WIFI_STA);
        esp_wifi_set_channel(_channel, WIFI_SECOND_CHAN_NONE);

        if (esp_now_init() != ESP_OK) {
            Serial.println("❌ Error al iniciar ESP-NOW");
            return;
        }

        // ESP_NOW: Configura el dispositivo como receptor
        esp_now_register_recv_cb([](const uint8_t *mac, const uint8_t *incomingDataRaw, int len) {
            ActuatorState state;
            memcpy(&state, incomingDataRaw, sizeof(ActuatorState));
            
            Serial.println("📥 Estado de actuadores recibido:");
            Serial.print("💧 Bomba: "); Serial.print(state.waterPump ? "ON" : "OFF");
            Serial.print(" | 🌬️ Ventilador: "); Serial.print(state.fan ? "ON" : "OFF");
            Serial.print(" | 💡 LEDs: "); Serial.println(state.leds ? "ON" : "OFF");

            if (_instance && _instance->_onReceiveCallback) {
                _instance->_onReceiveCallback(state);
            }
        });

        _instance = this;

        Serial.println("✅ ESP-NOW Receptor de actuadores listo");
    }

    /**
     * @brief Registra un callback para manejar los datos recibidos
     * @param callback Función que se llamará cuando se reciban datos
     */
    void onReceive(void (*callback)(const ActuatorState&)) {
        _onReceiveCallback = callback;
    }

private:
    static ESPNowActuatorReceiver* _instance; ///< Puntero estático a la instancia del receptor
};

// Definición del puntero estático
ESPNowActuatorReceiver* ESPNowActuatorReceiver::_instance = nullptr;

#endif  // ESP_NOW_ACTUATOR_RECEIVER_H
