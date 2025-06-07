#ifndef WIFI_CONNECTOR_H
#define WIFI_CONNECTOR_H

#include <WiFi.h>

class WiFiConnector {
private:
    const char* _ssid;
    const char* _password;

public:
    WiFiConnector(const char* ssid, const char* password)
        : _ssid(ssid), _password(password) {}

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

    int getChannel() {
        return WiFi.channel();
    }

    IPAddress getIP() {
        return WiFi.localIP();
    }

    String getSSID() {
        return WiFi.SSID();
    }
};

#endif
