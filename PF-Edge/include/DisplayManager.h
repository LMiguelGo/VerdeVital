#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "dataSensor.h"
#include "dataActuator.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

class DisplayManager {
private:
    Adafruit_SSD1306 display;
    unsigned long lastPageChange = 0;
    uint8_t currentPage = 0;
    const uint8_t totalPages = 5;
    unsigned long interval = 3000; // Tiempo entre páginas automáticas (ms)
    bool autoChange = false;

    // Datos simulados / reales (pueden ser actualizados desde fuera)
    String wifiStatus = "OK";
    String hora = "00:00:00";
    String fecha = "01-01-2023";
    String sensorData = "T:-- H:--";
    String actuadorEstado = "Luces: OFF";
    String telegramCmd = "/start";
    String alerta = "";

public:
    // Constructor sin botón
    DisplayManager(bool autoMode = false)
        : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1), autoChange(autoMode) {}

    bool begin() {
        if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
            Serial.println(F("❌ No se encontró la pantalla OLED"));
            return false;
        }

        display.clearDisplay();
        display.display();
        Serial.println("✅ Pantalla OLED inicializada");
        return true;
    }

    // Métodos para actualizar datos externos
    void setWifiStatus(const String& status) { wifiStatus = status; }

    void setHora(const String& h) { hora = h; }

    void setFecha(const String& f) { fecha = f; }

    void setSensorData(const SensorData& data, const String& rssiWifi = "") {
        sensorData = "";
        sensorData += "Temp:  " + String(data.temperature, 1) + " C\n";
        sensorData += "Hum:   " + String(data.humidity, 1) + " %\n";
        sensorData += "Luz:   " + String(data.light) + "\n";
        sensorData += "CO2:   " + String(data.co2ppm) + " ppm\n";
        sensorData += "Suelo: " + String(data.soilMoisture, 1) + " %\n";
        sensorData += "Volt:  " + String(data.voltage, 2) + " V\n";
        sensorData += "RSSI:  " + rssiWifi + " dBm\n";
    }

    void setActuadorEstado(const ActuatorState& state) {
        actuadorEstado = "";
        actuadorEstado += "Bomba: " + String(state.waterPump ? "ON" : "OFF") + "\n";
        actuadorEstado += "Vent:  " + String(state.fan ? "ON" : "OFF") + "\n";
        actuadorEstado += "Luces: " + String(state.leds ? "ON" : "OFF");
    }

    void setTelegramCmd(const String& cmd) {
        telegramCmd = cmd;
    }

    void setAlerta(bool criticState, bool lsoilM, bool hsoilM, bool lTemp, bool hTemp, bool co2, bool light, bool voltage, bool rssi, bool espsensor, bool espactuator) {
        alerta = "";
        if (criticState) alerta += "FALLA CRITICA!\n";
        if (voltage) alerta += " - Voltaje bajo\n";
        if (espsensor) alerta += " - Sensor ESP N.C.\n";
        if (espactuator) alerta += " - Actuador ESP N.C.\n";
        if (lsoilM) alerta += "Hum. Suelo baja\n";
        if (hsoilM) alerta += "Hum. Suelo alta\n";
        if (lTemp) alerta += "Temp. baja\n";
        if (hTemp) alerta += "Temp. alta\n";
        if (co2) alerta += "CO2 alto\n";
        if (light) alerta += "Luz insuficiente\n";
        if (rssi) alerta += "Conexión WiFi débil\n";
    }

    void mostrarPagina() {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);

        switch (currentPage) {
            case 0:
                display.println("Sistema activo");
                display.print("WiFi: "); display.println(wifiStatus);
                display.print("Fecha: "); display.println(fecha);
                display.print("Hora: "); display.println(hora);
                break;

            case 1:
                display.println("Ultimas mediciones:");
                display.println(sensorData);
                break;

            case 2:
                display.println("Actuadores:");
                display.println(actuadorEstado);
                break;

            case 3:
                display.println("Telegram: OK");
                display.print("Ultimo comando: \n");
                display.println(telegramCmd);
                break;

            case 4:
                display.println("ALERTAS:");
                if (alerta.length() > 0) {
                    display.println(alerta);
                } else {
                    display.println("No hay alertas activas");
                }
                break;
        }

        display.display();
    }

    void actualizar() {
        unsigned long now = millis();

        // ✅ Cambio automático si está habilitado
        if (autoChange && now - lastPageChange >= interval) {
            currentPage = (currentPage + 1) % totalPages;
            mostrarPagina();
            lastPageChange = now;
        }
    }

    void cambiarPagina() {
        currentPage = (currentPage + 1) % totalPages;
        mostrarPagina();
    }

    void mostrarMensajeRapido(const String& msg) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.println(msg);
        display.display();
    }
};

#endif
