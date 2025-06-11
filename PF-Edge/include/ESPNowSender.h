/**
 * @file ESPNowActuatorSender.h
 * @brief Este archivo define la clase ESPNowActuatorSender para enviar datos de actuadores a través de ESP-NOW.
 */

#ifndef ESP_NOW_ACTUATOR_SENDER_H
#define ESP_NOW_ACTUATOR_SENDER_H

// Librerías
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "dataActuator.h"

/**
 * @typedef struct_message
 * @brief Alias para la estructura ActuatorState que se enviará por ESP-NOW.
 */
typedef ActuatorState struct_message;

/**
 * @class ESPNowActuatorSender
 * @brief Clase para manejar el envío de datos de actuadores mediante ESP-NOW.
 * 
 * Esta clase permite inicializar ESP-NOW, configurar el peer receptor y enviar datos de actuadores.
 */
class ESPNowActuatorSender {
private:
    uint8_t _peerAddress[6]; ///< Dirección MAC del peer receptor.
    uint8_t _channel; ///< Canal de comunicación ESP-NOW.
    static bool _peerConnected; ///< Estado de conexión con el peer.

public:
    /**
     * @brief Constructor de la clase ESPNowActuatorSender.
     * @param mac Dirección MAC del peer receptor.
     * @param channel Canal de comunicación ESP-NOW.
     */
    ESPNowActuatorSender(const uint8_t mac[6], uint8_t channel) {
        memcpy(_peerAddress, mac, 6);
        _channel = channel;
    }

    /**
     * @brief Inicializa ESP-NOW y configura el peer receptor.
     * 
     * Configura el modo WiFi en STA, establece el canal de comunicación y registra el callback para el estado de envío.
     */
    void begin() {
        WiFi.mode(WIFI_STA);
        esp_wifi_set_channel(_channel, WIFI_SECOND_CHAN_NONE);

        if (esp_now_init() != ESP_OK) {
            Serial.println("❌ Error al iniciar ESP-NOW");
            return;
        }

        esp_now_register_send_cb([](const uint8_t *mac_addr, esp_now_send_status_t status) {
            Serial.print("📤 Estado del envío de actuadores: ");
            if (status == ESP_NOW_SEND_SUCCESS) {
                Serial.println("Éxito");
                _peerConnected = true;  // Actualiza estado
            } else {
                Serial.println("Fallido");
                _peerConnected = false;
            }
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

    /**
     * @brief Envía datos de actuadores al peer receptor.
     * @param state Estructura ActuatorState con los datos de los actuadores.
     */
    void sendData(const ActuatorState& state) {
        esp_err_t result = esp_now_send(_peerAddress, (uint8_t *)&state, sizeof(state));
        if (result == ESP_OK) {
            Serial.println("📨 Estado de actuadores enviado correctamente");
        } else {
            Serial.print("⚠️ Error al enviar estado: ");
            Serial.println(esp_err_to_name(result));
            _peerConnected = false;
        }
    }

    /**
     * @brief Verifica si el peer receptor está conectado.
     * @return true si el peer está conectado, false en caso contrario.
     */
    bool isPeerConnected() const {
        return _peerConnected;
    }
};

// Definición del atributo estático
bool ESPNowActuatorSender::_peerConnected = false;

#endif // ESP_NOW_ACTUATOR_SENDER_H
