#ifndef ESP_NOW_ACTUATOR_SENDER_H
#define ESP_NOW_ACTUATOR_SENDER_H

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "dataActuator.h"  // Incluimos la estructura ActuatorState

// Estructura que se enviará por ESP-NOW (idéntica a ActuatorState)
typedef ActuatorState struct_message;

class ESPNowActuatorSender {
private:
    uint8_t _peerAddress[6];
    uint8_t _channel;

public:
    ESPNowActuatorSender(const uint8_t mac[6], uint8_t channel) {
        memcpy(_peerAddress, mac, 6);
        _channel = channel;
    }

    void begin() {
        WiFi.mode(WIFI_STA);
        esp_wifi_set_channel(_channel, WIFI_SECOND_CHAN_NONE);

        if (esp_now_init() != ESP_OK) {
            Serial.println("❌ Error al iniciar ESP-NOW");
            return;
        }

        esp_now_register_send_cb([](const uint8_t *mac_addr, esp_now_send_status_t status) {
            Serial.print("📤 Estado del envío de actuadores: ");
            Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Éxito" : "Fallo");
        });

        esp_now_peer_info_t peerInfo = {};
        memcpy(peerInfo.peer_addr, _peerAddress, 6);
        peerInfo.channel = _channel;
        peerInfo.encrypt = false;
        peerInfo.ifidx = WIFI_IF_STA;

        if (!esp_now_is_peer_exist(_peerAddress)) {
            if (esp_now_add_peer(&peerInfo) != ESP_OK) {
                Serial.println("❌ Error al añadir el peer");
                return;
            }
        }

        Serial.println("✅ ESP-NOW Emisor de actuadores listo");
    }

    void sendData(const ActuatorState& state) {
        esp_err_t result = esp_now_send(_peerAddress, (uint8_t *)&state, sizeof(state));
        if (result == ESP_OK) {
            Serial.println("📨 Estado de actuadores enviado correctamente");
        } else {
            Serial.print("⚠️ Error al enviar estado: ");
            Serial.println(esp_err_to_name(result));
        }
    }
};

#endif
