/**
 * @file ESPNowReceiver.h
 * @brief Este archivo define la clase ESPNowReceiver para recibir datos de sensores a través de ESP-NOW.
 * @details La clase maneja la inicialización de ESP-NOW, el registro de callbacks para recibir datos, y la gestión de la conexión.
 */

#ifndef ESP_NOW_RECEIVER_H
#define ESP_NOW_RECEIVER_H

// Librerías
#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "dataSensor.h"

/**
 * @class ESPNowReceiver
 * @brief Clase para manejar la recepción de datos mediante ESP-NOW.
 * 
 * Esta clase permite inicializar ESP-NOW, registrar un callback para procesar los datos recibidos y gestionar el estado de la conexión.
 */
class ESPNowReceiver {
private:
    uint8_t _channel; ///< Canal de comunicación ESP-NOW.
    void (*_onReceiveCallback)(const SensorData&); ///< Callback para procesar los datos recibidos.
    static ESPNowReceiver* _instance; ///< Instancia única de la clase para manejar el callback estático.

    static constexpr unsigned long TIMEOUT_MS = 10000; ///< Tiempo de espera (en milisegundos) para considerar la conexión como inactiva.
    unsigned long _lastReceivedTime = 0; ///< Marca de tiempo del último paquete recibido.
    bool _connected = false; ///< Estado de la conexión con el nodo.

public:
    /**
     * @brief Constructor de la clase ESPNowReceiver.
     * @param channel Canal de comunicación ESP-NOW (por defecto es 1).
     */
    ESPNowReceiver(uint8_t channel = 1) : _channel(channel), _onReceiveCallback(nullptr) {}

    /**
     * @brief Inicializa ESP-NOW y configura el receptor.
     * 
     * Configura el modo WiFi en STA, establece el canal de comunicación y registra el callback para recibir datos.
     */
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

    /**
     * @brief Registra un callback para procesar los datos recibidos.
     * @param callback Función a ejecutar cuando se reciben datos.
     */
    void onReceive(void (*callback)(const SensorData&)) {
        _onReceiveCallback = callback;
    }

    /**
     * @brief Actualiza el estado de la conexión.
     * 
     * Verifica si el nodo ha dejado de enviar datos durante un tiempo mayor al definido en `TIMEOUT_MS`.
     */
    void update() {
        if (_connected && millis() - _lastReceivedTime > TIMEOUT_MS) {
            _connected = false;
            Serial.println("⚠️ Nodo inactivo (timeout)");
        }
    }

    /**
     * @brief Verifica si el nodo está conectado.
     * @return true si el nodo está conectado, false en caso contrario.
     */
    bool isConnected() const {
        return _connected;
    }
};

ESPNowReceiver* ESPNowReceiver::_instance = nullptr; 

#endif // ESP_NOW_RECEIVER_H
