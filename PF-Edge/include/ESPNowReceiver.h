#ifndef ESP_NOW_RECEIVER_H
#define ESP_NOW_RECEIVER_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "dataSensor.h"

class ESPNowReceiver {
private:
    uint8_t _channel;
    void (*_onReceiveCallback)(const SensorData&);
    static ESPNowReceiver* _instance;

    static constexpr unsigned long TIMEOUT_MS = 10000;  // 10 segundos
    unsigned long _lastReceivedTime = 0;
    bool _connected = false;

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

            if (_instance) {
                _instance->_lastReceivedTime = millis();
                if (!_instance->_connected) {
                    Serial.println("🔄 Nodo conectado nuevamente");
                }
                _instance->_connected = true;

                if (_instance->_onReceiveCallback) {
                    _instance->_onReceiveCallback(data);
                }
            }
        });

        _instance = this;

        Serial.println("✅ ESP-NOW Receptor listo");
    }

    void onReceive(void (*callback)(const SensorData&)) {
        _onReceiveCallback = callback;
    }

    void update() {
        if (_connected && millis() - _lastReceivedTime > TIMEOUT_MS) {
            _connected = false;
            Serial.println("⚠️ Nodo inactivo (timeout)");
        }
    }

    bool isConnected() const {
        return _connected;
    }
};

ESPNowReceiver* ESPNowReceiver::_instance = nullptr;

#endif
