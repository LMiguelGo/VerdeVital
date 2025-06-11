/**
 * @file ThresholdsController.h
 * @brief Clase para manejar los umbrales y alertas del sistema.
 * 
 * Esta clase evalúa los datos de los sensores en función de los umbrales definidos, activa alertas y controla el estado de los actuadores.
 */
#ifndef THRESHOLDS_CONTROLLER_H
#define THRESHOLDS_CONTROLLER_H

// Librerías
#include "dataSensor.h"
#include "dataActuator.h"
#include <Arduino.h>

/**
 * @class Thresholds
 * @brief Clase para gestionar los umbrales, alertas y estados de los actuadores.
 * 
 * Proporciona métodos para evaluar los datos de los sensores, activar alertas, controlar actuadores y generar mensajes de estado.
 */
class Thresholds {
private:
    float minSoilMoisture = 45.0; ///< Umbral mínimo de humedad del suelo (%).
    float maxSoilMoisture = 70.0; ///< Umbral máximo de humedad del suelo (%).
    float minTemperature = 24.0; ///< Umbral mínimo de temperatura (°C).
    float maxTemperature = 28.0; ///< Umbral máximo de temperatura (°C).
    float maxCO2 = 800.0; ///< Umbral máximo de CO2 (ppm).
    int minLight = 300; ///< Umbral mínimo de luz.
    float minVoltage = 6.0; ///< Umbral mínimo de voltaje (V).
    int minRSSI = -70; ///< Umbral mínimo de intensidad de señal WiFi (RSSI).

public:
    /**
     * @brief Constructor de la clase Thresholds.
     */
    Thresholds() {}
    // RSSI Sensor y WiFi
    int RSSIWiFi = 0; ///< Valor por defecto para RSSI del WiFi

    // Alert flags
    bool alertLowSoilMoisture = false;
    bool alertHighSoilMoisture = false;
    bool alertLowTemperature = false;
    bool alertHighTemperature = false;
    bool alertCO2 = false;
    bool alertLight = false;
    bool alertVoltage = false;
    bool alertESPSensor = false;
    bool alertESPActuator = false; 
    bool alertWifi = false; 

    // Guarda los valores anteriores de los flags de alerta
    bool prevAlertLowSoilMoisture = false;
    bool prevAlertHighSoilMoisture = false;
    bool prevAlertLowTemperature = false;
    bool prevAlertHighTemperature = false;
    bool prevAlertCO2 = false;
    bool prevAlertLight = false;
    bool prevAlertVoltage = false;
    bool prevAlertESPSensor = false;
    bool prevAlertESPActuator = false;
    bool prevAlertWifi = false;

    /**
     * @brief Verifica si alguna alerta ha cambiado desde la última evaluación.
     * @return true si alguna alerta ha cambiado, false en caso contrario.
     */
    bool hasAlertChanged() {
        bool changed = 
            (alertLowSoilMoisture != prevAlertLowSoilMoisture) ||
            (alertHighSoilMoisture != prevAlertHighSoilMoisture) ||
            (alertLowTemperature != prevAlertLowTemperature) ||
            (alertHighTemperature != prevAlertHighTemperature) ||
            (alertCO2 != prevAlertCO2) ||
            (alertLight != prevAlertLight) ||
            (alertVoltage != prevAlertVoltage) ||
            (alertESPSensor != prevAlertESPSensor) ||
            (alertESPActuator != prevAlertESPActuator) ||
            (alertWifi != prevAlertWifi);

        // Actualiza los valores previos
        prevAlertLowSoilMoisture = alertLowSoilMoisture;
        prevAlertHighSoilMoisture = alertHighSoilMoisture;
        prevAlertLowTemperature = alertLowTemperature;
        prevAlertHighTemperature = alertHighTemperature;
        prevAlertCO2 = alertCO2;
        prevAlertLight = alertLight;
        prevAlertVoltage = alertVoltage;
        prevAlertESPSensor = alertESPSensor;
        prevAlertESPActuator = alertESPActuator;
        prevAlertWifi = alertWifi;

        return changed;
    }

    /**
     * @brief Verifica si alguna alerta de sensor está activa.
     * @return true si alguna alerta de sensor está activa, false en caso contrario.
     */
    bool anySensorAlertActive() const {
        return alertLowSoilMoisture ||
               alertHighSoilMoisture ||
               alertLowTemperature ||
               alertHighTemperature ||
               alertCO2 ||
               alertLight ||
               alertVoltage;
    }

    /**
     * @brief Verifica si hay una falla crítica en el sistema.
     * 
     * Una falla crítica ocurre si hay pérdida de conexión con nodos o voltaje bajo.
     * 
     * @return true si hay una falla crítica, false en caso contrario.
     */
    bool hasCriticalFailure() const {
        // Considera fallo crítico si hay pérdida de conexión con nodos o voltaje bajo
        return alertESPSensor || alertESPActuator || alertVoltage;
    }

    /**
     * @brief Evalúa los datos de los sensores y actualiza el estado de los actuadores.
     * 
     * Activa o desactiva los actuadores en función de los valores de los sensores y los umbrales definidos.
     * 
     * @param data Datos de los sensores.
     * @return Estado actualizado de los actuadores.
     */
    ActuatorState evaluate(const SensorData& data) {
        ActuatorState state;
        state.waterPump = 0;
        state.fan = 0;
        state.leds = 0;

        // Evaluar humedad del suelo
        if (data.soilMoisture < minSoilMoisture) {
            state.waterPump = 1;
            alertLowSoilMoisture = true;
        } else { 
            alertLowSoilMoisture = false;
        }

        if (data.soilMoisture > maxSoilMoisture) {
            alertHighSoilMoisture = true;
        } else {
            alertHighSoilMoisture = false;
        }

        // Evaluar temperatura
        if (data.temperature < minTemperature) {
            alertLowTemperature = true;
        } else {
            alertLowTemperature = false;
        }

        if (data.temperature > maxTemperature) {
            alertHighTemperature = true;
            state.fan = 1;
        } else {
            alertHighTemperature = false;
        }

        // Evaluar CO2
        if (data.co2ppm > maxCO2) {
            alertCO2 = true;
            state.fan = 1;
        } else {
            alertCO2 = false;
        }

        // Evaluar luz
        if (data.light < minLight) {
            alertLight = true;
            state.leds = 1;
        } else {
            alertLight = false;
        }

        // Evaluar voltaje
        if (data.voltage < minVoltage) {
            alertVoltage = true;
        } else {
            alertVoltage = false;
        }
        return state;

        // Evaluar conexión wiFi (opcional)
        if (RSSIWiFi < minRSSI) {
            alertWifi = true;
        } else {
            alertWifi = false;
        }
    }

    /**
     * @brief Genera un mensaje con las alertas activas.
     * 
     * Incluye alertas críticas y de sensores, junto con recomendaciones de acción.
     * 
     * @param data Datos de los sensores.
     * @return Cadena con el mensaje de alertas.
     */
    String returnAlarm(const SensorData& data) {
        String msg = "";
        
        if (hasCriticalFailure()) {
            msg += "🚨 ALERTA CRÍTICA DETECTADA:\n";
            if (alertESPSensor) {
                msg += "⚠️ Conexión Nodo Sensores perdida.\n";
                msg += "🔍 Revisa el sensor o reinicia el dispositivo.\n";
            }
            if (alertESPActuator) {
                msg += "⚠️ Conexión Nodo Actuadores perdida.\n";
                msg += "🔍 Revisa el actuador o reinicia el dispositivo.\n";
            }
            if (alertVoltage) {
                msg += "⚠️ Voltaje bajo (" + String(data.voltage, 2) + " V).\n";
                msg += "🔍 Revisa la fuente de alimentación.\n";
            }
            msg += "\n";
        }

        if (anySensorAlertActive()) {
            msg += "ALERTAS DE SENSORES:\n";
        }

        if (alertLowSoilMoisture) {
            msg += "🌱 Humedad del suelo baja (" + String(data.soilMoisture, 1) + "%).\n";
            msg += "🔁 Activando bomba de agua.\n\n";
        }
        
        if (alertHighSoilMoisture) {
            msg += "🌱 Humedad del suelo alta (" + String(data.soilMoisture, 1) + "%).\n";
            msg += "🔍 Revisa el drenaje.\n\n";
        }

        if (alertLowTemperature) {
            msg += "🌡️ Temperatura baja (" + String(data.temperature, 1) + "°C).\n";
            msg += "🔍 Revisa el calentador.\n\n";
        }

        if (alertHighTemperature) {
            msg += "🌡️ Temperatura alta (" + String(data.temperature, 1) + "°C).\n";
            msg += "🔁 Activando ventilador.\n\n";
        }

        if (alertCO2) {
            msg += "🫁 CO2 alto (" + String(data.co2ppm) + " ppm).\n";
            msg += "🔁 Activando ventilador.\n\n";
        }

        if (alertLight) {
            msg += "💡 Luz insuficiente (" + String(data.light) + ").\n";
            msg += "🔁 Encendiendo LEDs.\n\n";
        }

        if (alertWifi) {
            msg += "📶 Conexión WiFi débil (" + String(RSSIWiFi) + " dBm).\n";
            msg += "🔍 Revisa la conexión WiFi.\n\n";
        }

        return msg; // Si está vacío, todo está normal
    }

    /**
     * @brief Obtiene el estado actual de los umbrales configurados.
     * @return Cadena con los valores de los umbrales actuales.
     */
    String getStatus() const {
        String status = "📐 Umbrales actuales:\n";
        status += "🌱 Hum. Suelo: " + String(minSoilMoisture) + " % - " + String(maxSoilMoisture) + " %\n";
        status += "🌡️ Temp.: " + String(minTemperature) + " °C - " + String(maxTemperature) + " °C\n";
        status += "🫁 CO2 < " + String(maxCO2) + " ppm\n";
        status += "💡 Luz > " + String(minLight) + "\n";
        status += "🔋 Voltaje > " + String(minVoltage) + " V\n";
        status += "📶 RSSI > " + String(minRSSI) + " dBm\n";
        return status;
    }

    /**
     * @brief Actualiza los umbrales en función de un comando recibido.
     * 
     * El comando debe tener el formato "comando tipo valor", donde:
     * - `tipo` es el umbral a modificar (por ejemplo, "suelo_min").
     * - `valor` es el nuevo valor del umbral.
     * 
     * @param cmd Comando recibido.
     */
    void updateFromCommand(const String& cmd) {
        int firstSpace = cmd.indexOf(' ');
        int secondSpace = cmd.indexOf(' ', firstSpace + 1);
        if (firstSpace == -1 || secondSpace == -1) return;

        String tipo = cmd.substring(firstSpace + 1, secondSpace);
        float valor = cmd.substring(secondSpace + 1).toFloat();

        if (tipo == "suelo_min") minSoilMoisture = valor;
        else if (tipo == "suelo_max") maxSoilMoisture = valor;
        else if (tipo == "temp_min") minTemperature = valor;
        else if (tipo == "temp_max") maxTemperature = valor;
        else if (tipo == "co2_max") maxCO2 = valor;
        else if (tipo == "luz_min") minLight = (int)valor;
        else if (tipo == "voltaje_min") minVoltage = valor;
        else if (tipo == "rssi_min") minRSSI = (int)valor;
    }

    /**
     * @brief Formatea los datos de los sensores en una cadena legible.
     * 
     * @param d Datos de los sensores.
     * @return Cadena con los datos formateados.
     */
    String formatSensorData(const SensorData& d) const {
        String msg = "";
        msg += "🌡️ Temp: " + String(d.temperature, 1) + " °C\n";
        msg += "💧 Hum: " + String(d.humidity, 1) + " %\n";
        msg += "☀️ Luz: " + String(d.light) + "\n";
        msg += "🫁 CO2: " + String(d.co2ppm) + " ppm\n";
        msg += "🌱 Suelo: " + String(d.soilMoisture, 1) + " %\n";
        msg += "🔋 Volt: " + String(d.voltage, 2) + " V\n";
        msg += "📶 RSSI WiFi: " + String(RSSIWiFi) + " dBm\n";
        return msg;
    }

    /**
     * @brief Formatea el estado de los actuadores en una cadena legible.
     * 
     * @param state Estado de los actuadores.
     * @return Cadena con el estado formateado.
     */
    String formatActuatorState(const ActuatorState& state) const {
        String s = "";
        s += "💧 Bomba: " + String(state.waterPump ? "ON" : "OFF") + "\n";
        s += "🌀 Ventilador: " + String(state.fan ? "ON" : "OFF") + "\n";
        s += "💡 LEDs: " + String(state.leds ? "ON" : "OFF");
        return s;
    }
};

#endif
