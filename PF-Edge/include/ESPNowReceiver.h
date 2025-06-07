#ifndef ESP_NOW_RECEIVER_H
#define ESP_NOW_RECEIVER_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "dataSensor.h"  // Incluimos la estructura actualizada

class ESPNowReceiver {
private:
    uint8_t _channel;
    void (*_onReceiveCallback)(const SensorData&);  // Callback con estructura SensorData

public:
    ESPNowReceiver(uint8_t channel = 1) : _channel(channel), _onReceiveCallback(nullptr) {}

    void begin() {
        WiFi.mode(WIFI_STA);
        esp_wifi_set_channel(_channel, WIFI_SECOND_CHAN_NONE);

        if (esp_now_init() != ESP_OK) {
            Serial.println("❌ Error al iniciar ESP-NOW");
            return;
        }

        esp_now_register_recv_cb([](const uint8_t *mac, const uint8_t *incomingDataRaw, int len) {
            SensorData data;
            memcpy(&data, incomingDataRaw, sizeof(SensorData));
            // Llama al callback si está definido
            if (_instance && _instance->_onReceiveCallback) {
                _instance->_onReceiveCallback(data);
            }
        });

        _instance = this;

        Serial.println("✅ ESP-NOW Receptor listo");
    }

    // Registrar callback personalizado
    void onReceive(void (*callback)(const SensorData&)) {
        _onReceiveCallback = callback;
    }

private:
    static ESPNowReceiver* _instance;
};

// Definición del puntero estático
ESPNowReceiver* ESPNowReceiver::_instance = nullptr;

#endif
