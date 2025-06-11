/**
 * @file WifiConnector.h
 * @brief Clase para manejar la conexión WiFi en modo estación.
 * 
 * Esta clase permite conectar un dispositivo a una red WiFi y obtener información sobre la conexión.
 */
#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

// Librerías
#include <WiFi.h>

/**
 * @class WiFiConnector
 * @brief Clase para gestionar la conexión WiFi.
 * 
 * Proporciona métodos para conectarse a una red WiFi y obtener detalles de la conexión, como el canal, la dirección IP, el SSID y la intensidad de la señal.
 */
class WiFiConnector {
private:
    const char* _ssid; ///< SSID de la red WiFi.
    const char* _password; ///< Contraseña de la red WiFi.

public:
    /**
     * @brief Constructor de la clase WiFiConnector.
     * @param ssid SSID de la red WiFi.
     * @param password Contraseña de la red WiFi.
     */
    WiFiConnector(const char* ssid, const char* password)
        : _ssid(ssid), _password(password) {}

    /**
     * @brief Conecta el dispositivo a la red WiFi.
     * 
     * Configura el dispositivo en modo estación, intenta conectarse a la red especificada y muestra información de la conexión.
     * 
     * @return true si la conexión fue exitosa, false en caso contrario.
     */
    bool connect() {
        Serial.println("🔌 Iniciando conexión WiFi...");
 
        WiFi.mode(WIFI_STA);  // Modo estación
        WiFi.begin(_ssid, _password);

        unsigned long startAttemptTime = millis();
        const unsigned long timeout = 10000; // 10 segundos

        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
            delay(500);
            Serial.print(".");
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\n✅ Conectado a WiFi");
            Serial.print("📶 SSID: "); Serial.println(WiFi.SSID());
            Serial.print("📡 Canal: "); Serial.println(WiFi.channel());
            Serial.print("🧠 IP local: "); Serial.println(WiFi.localIP());
            return true;
        } else {
            Serial.println("\n❌ Error: No se pudo conectar al WiFi");
            return false;
        }
    }

    /**
     * @brief Obtiene el canal de la red WiFi conectada.
     * @return Número del canal de la red WiFi.
     */
    int getChannel() {
        return WiFi.channel();
    }

    /**
     * @brief Obtiene la dirección IP local asignada al dispositivo.
     * @return Dirección IP local.
     */
    IPAddress getIP() {
        return WiFi.localIP();
    }

    /**
     * @brief Obtiene el SSID de la red WiFi conectada.
     * @return SSID de la red WiFi.
     */
    String getSSID() {
        return WiFi.SSID();
    }

    /**
     * @brief Obtiene la intensidad de la señal WiFi (RSSI).
     * @return Intensidad de la señal en dBm.
     */
    int getRSSI() {
        return WiFi.RSSI();
    }
};

#endif // WIFI_CONNECTOR_H
