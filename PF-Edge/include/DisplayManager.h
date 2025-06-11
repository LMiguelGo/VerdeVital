/**
 * @file DisplayManager.h
 * @brief Este archivo define la clase DisplayManager para manejar la pantalla OLED del invernadero.
 */

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

// Librerías
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "dataSensor.h"
#include "dataActuator.h"

// Definiciones
#define SCREEN_WIDTH 128 ///< Ancho de la pantalla OLED en píxeles
#define SCREEN_HEIGHT 64 ///< Alto de la pantalla OLED en píxeles
#define OLED_ADDR 0x3C ///< Dirección I2C de la pantalla OLED

/**
 * @class DisplayManager
 * @brief Clase para manejar la pantalla OLED del invernadero.
 * Esta clase permite mostrar información del sistema, datos de sensores, estado de actuadores, comandos de Telegram y alertas.
 */
class DisplayManager {
private:
    Adafruit_SSD1306 display; ///< Objeto de la pantalla OLED
    unsigned long lastPageChange = 0; ///< Marca de tiempo del último cambio de página
    uint8_t currentPage = 0; ///< Página actual que se está mostrando
    const uint8_t totalPages = 5; ///< Total de páginas disponibles
    unsigned long interval = 3000; ///< Intervalo de tiempo entre cambios de página
    bool autoChange = false; ///< Indica si el cambio de página es automático

    // Datos simulados / reales (pueden ser actualizados desde fuera)
    String wifiStatus = "OK"; ///< Estado de la conexión WiFi
    String hora = "00:00:00"; ///< Hora actual
    String fecha = "01-01-2023"; ///< Fecha actual
    String sensorData = "T:-- H:--"; ///< Datos de los sensores
    String actuadorEstado = "Luces: OFF"; ///< Estado de los actuadores
    String telegramCmd = "/start"; ///< Último comando de Telegram
    String alerta = ""; ///< Mensaje de alerta

public:
    /**
     * @brief Constructor de la clase DisplayManager.
     * @param autoMode Indica si el cambio de página es automático (por defecto es false).
     */
    DisplayManager(bool autoMode = false)
        : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1), autoChange(autoMode) {}

    /**
     * @brief Inicializa la pantalla OLED.
     * @return true si la inicialización fue exitosa, false en caso contrario.
     */
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

    /**
     * @brief Actualiza el estado de la conexión WiFi.
     * @param status Estado actual del WiFi.
     */
    void setWifiStatus(const String& status) { wifiStatus = status; }

    /**
     * @brief Actualiza la hora mostrada en la pantalla.
     * @param h Hora en formato de cadena.
     */
    void setHora(const String& h) { hora = h; }

    /**
     * @brief Actualiza la fecha mostrada en la pantalla.
     * @param f Fecha en formato de cadena.
     */
    void setFecha(const String& f) { fecha = f; }

    /**
     * @brief Actualiza los datos de los sensores mostrados en la pantalla.
     * @param data Estructura con los datos de los sensores.
     * @param rssiWifi (Opcional) Intensidad de la señal WiFi en dBm.
     */
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

    /**
     * @brief Actualiza el estado de los actuadores mostrado en la pantalla.
     * @param state Estructura con el estado de los actuadores.
     */
    void setActuadorEstado(const ActuatorState& state) {
        actuadorEstado = "";
        actuadorEstado += "Bomba: " + String(state.waterPump ? "ON" : "OFF") + "\n";
        actuadorEstado += "Vent:  " + String(state.fan ? "ON" : "OFF") + "\n";
        actuadorEstado += "Luces: " + String(state.leds ? "ON" : "OFF");
    }

    /**
     * @brief Actualiza el último comando de Telegram mostrado en la pantalla.
     * @param cmd Comando recibido de Telegram.
     */
    void setTelegramCmd(const String& cmd) {
        telegramCmd = cmd;
    }

    /**
     * @brief Actualiza los mensajes de alerta mostrados en la pantalla.
     * @param criticState Indica si hay una falla crítica.
     * @param lsoilM Indica si la humedad del suelo es baja.
     * @param hsoilM Indica si la humedad del suelo es alta.
     * @param lTemp Indica si la temperatura es baja.
     * @param hTemp Indica si la temperatura es alta.
     * @param co2 Indica si el nivel de CO2 es alto.
     * @param light Indica si la luz es insuficiente.
     * @param voltage Indica si el voltaje es bajo.
     * @param rssi Indica si la conexión WiFi es débil.
     * @param espsensor Indica si el ESP de sensores no está conectado.
     * @param espactuator Indica si el ESP de actuadores no está conectado.
     */
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

    /**
     * @brief Muestra la página actual en la pantalla OLED.
     */
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

    /**
     * @brief Actualiza la pantalla, cambiando de página automáticamente si está habilitado.
     */
    void actualizar() {
        unsigned long now = millis();

        // ✅ Cambio automático si está habilitado
        if (autoChange && now - lastPageChange >= interval) {
            currentPage = (currentPage + 1) % totalPages;
            mostrarPagina();
            lastPageChange = now;
        }
    }

    /**
     * @brief Cambia manualmente a la siguiente página.
     */
    void cambiarPagina() {
        currentPage = (currentPage + 1) % totalPages;
        mostrarPagina();
    }

    /**
     * @brief Muestra un mensaje rápido en la pantalla OLED.
     * @param msg Mensaje a mostrar.
     */
    void mostrarMensajeRapido(const String& msg) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.println(msg);
        display.display();
    }
};

#endif // DISPLAY_MANAGER_H
