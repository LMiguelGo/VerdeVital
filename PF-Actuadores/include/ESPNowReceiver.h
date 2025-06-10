#ifndef ESP_NOW_ACTUATOR_RECEIVER_H
#define ESP_NOW_ACTUATOR_RECEIVER_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "dataActuator.h"  // Usamos la estructura para estados de actuadores

class ESPNowActuatorReceiver {
private:
    uint8_t _channel;
    void (*_onReceiveCallback)(const ActuatorState&);  // Callback con estructura ActuatorState

public:
    ESPNowActuatorReceiver(uint8_t channel = 1) : _channel(channel), _onReceiveCallback(nullptr) {}

    void begin() {
        WiFi.mode(WIFI_STA);
        esp_wifi_set_channel(_channel, WIFI_SECOND_CHAN_NONE);

        if (esp_now_init() != ESP_OK) {
            Serial.println("❌ Error al iniciar ESP-NOW");
            return;
        }

        esp_now_register_recv_cb([](const uint8_t *mac, const uint8_t *incomingDataRaw, int len) {
            ActuatorState state;
            memcpy(&state, incomingDataRaw, sizeof(ActuatorState));
            
            Serial.println("📥 Estado de actuadores recibido:");
            Serial.print("💧 Bomba: "); Serial.print(state.waterPump ? "ON" : "OFF");
            Serial.print(" | 🌬️ Ventilador: "); Serial.print(state.fan ? "ON" : "OFF");
            Serial.print(" | 💡 LEDs: "); Serial.println(state.leds ? "ON" : "OFF");

            // Llama al callback si está definido
            if (_instance && _instance->_onReceiveCallback) {
                _instance->_onReceiveCallback(state);
            }
        });

        _instance = this;

        Serial.println("✅ ESP-NOW Receptor de actuadores listo");
    }

    // Registrar callback personalizado
    void onReceive(void (*callback)(const ActuatorState&)) {
        _onReceiveCallback = callback;
    }

private:
    static ESPNowActuatorReceiver* _instance;
};

// Definición del puntero estático
ESPNowActuatorReceiver* ESPNowActuatorReceiver::_instance = nullptr;

#endif
