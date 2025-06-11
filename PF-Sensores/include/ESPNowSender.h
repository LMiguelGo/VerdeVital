/**
 * @file ESPNowSender.h
 * @brief Clase para enviar datos usando ESP-NOW en ESP32.
 * Esta clase permite enviar datos de sensores a través de ESP-NOW.
 * La estructura de datos enviada es idéntica a SensorData.
 * @note Asegúrate de incluir la estructura SensorData en tu proyecto.
 *       Esta clase está diseñada para ser utilizada en un entorno ESP32.
 */

#ifndef ESP_NOW_SENDER_H
#define ESP_NOW_SENDER_H

// Librerías
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "sensorData.h"

// Estructura que se enviará por ESP-NOW (idéntica a SensorData)
typedef SensorData struct_message;

/**
 * @class ESPNowSender
 * @brief Clase para enviar datos de sensores usando ESP-NOW.
 * 
 * Esta clase permite inicializar el envío de datos a un dispositivo específico
 * utilizando ESP-NOW. Se encarga de configurar el canal y la dirección MAC del peer.
 */
class ESPNowSender {
private:
    uint8_t _peerAddress[6]; ///< Dirección MAC del dispositivo receptor
    uint8_t _channel; ///< Canal de comunicación

public:
    /**
     * @brief Constructor de la clase ESPNowSender.
     * @param mac Dirección MAC del dispositivo receptor (6 bytes).
     * @param channel Canal de comunicación (1-14).
     */
    ESPNowSender(const uint8_t mac[6], uint8_t channel) {
        memcpy(_peerAddress, mac, 6);
        _channel = channel;
    }

    /**
     * @brief Constructor que inicializa el envío de datos.
     * @details Configura el modo WiFi, el canal y la dirección del peer.
     *          Registra el callback de envío y añade el peer si no existe.
     */
    void begin() {
        WiFi.mode(WIFI_STA);
        esp_wifi_set_channel(_channel, WIFI_SECOND_CHAN_NONE);

        if (esp_now_init() != ESP_OK) {
            Serial.println("❌ Error al iniciar ESP-NOW");
            return;
        }

        esp_now_register_send_cb([](const uint8_t *mac_addr, esp_now_send_status_t status) {
            Serial.print("📤 Estado del envío: ");
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

        Serial.println("✅ ESP-NOW Emisor listo");
    }

    /**
     * @brief Envía los datos del sensor a través de ESP-NOW.
     * @param data Estructura SensorData que contiene los datos del sensor.
     */
    void sendData(const SensorData& data) {
        esp_err_t result = esp_now_send(_peerAddress, (uint8_t *)&data, sizeof(data));
        if (result == ESP_OK) {
            Serial.println("📨 Datos enviados correctamente");
        } else {
            Serial.print("⚠️ Error al enviar datos: ");
            Serial.println(esp_err_to_name(result));
        }
    }
};

#endif // ESP_NOW_SENDER_H
