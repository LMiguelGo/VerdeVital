#ifndef THRESHOLDS_CONTROLLER_H
#define THRESHOLDS_CONTROLLER_H

#include "dataSensor.h"
#include "dataActuator.h"
#include <Arduino.h>

class Thresholds {
private:
    float minSoilMoisture = 30.0;
    float maxTemperature = 28.0;
    float maxCO2 = 800.0;
    int minLight = 300;
    float minVoltage = 5.5;

public:
    Thresholds() {}

    ActuatorState evaluate(const SensorData& data) {
        ActuatorState state;
        state.waterPump = 0;
        state.fan = 0;
        state.leds = 0;

        if (data.soilMoisture < minSoilMoisture) {
            state.waterPump = 1;
        }
        if (data.temperature > maxTemperature || data.co2ppm > maxCO2) {
            state.fan = 1;
        }
        if (data.light < minLight) {
            state.leds = 1;
        }

        return state;
    }

    // Nueva función que describe los valores fuera de rango y acciones tomadas
    String checkThresholds(const SensorData& data) {
        String msg = "";

        if (data.soilMoisture < minSoilMoisture) {
            msg += "🌱 Humedad del suelo baja (" + String(data.soilMoisture, 1) + "%).\n";
            msg += "🔁 Activando bomba de agua.\n";
        }

        if (data.temperature > maxTemperature) {
            msg += "🌡️ Temperatura alta (" + String(data.temperature, 1) + "°C).\n";
            msg += "🔁 Activando ventilador.\n";
        }

        if (data.co2ppm > maxCO2) {
            msg += "🫁 CO2 alto (" + String(data.co2ppm) + " ppm).\n";
            msg += "🔁 Activando ventilador.\n";
        }

        if (data.light < minLight) {
            msg += "💡 Luz insuficiente (" + String(data.light) + ").\n";
            msg += "🔁 Encendiendo LEDs.\n";
        }

        if (data.voltage < minVoltage) {
            msg += "🔋 Voltaje bajo (" + String(data.voltage, 2) + " V).\n";
            msg += "⚠️ Revisa la fuente de alimentación.\n";
        }

        return msg; // Si está vacío, todo está normal
    }

    // Versión anterior para compatibilidad, si se requiere
    bool isNormal(const SensorData& data) {
        return checkThresholds(data).isEmpty();
    }

    String getStatus() const {
        String status = "📐 Umbrales actuales:\n";
        status += "🌱 Hum. Suelo < " + String(minSoilMoisture) + " %\n";
        status += "🌡️ Temp. > " + String(maxTemperature) + " °C\n";
        status += "🫁 CO2 > " + String(maxCO2) + " ppm\n";
        status += "💡 Luz < " + String(minLight) + "\n";
        status += "🔋 Voltaje > " + String(minVoltage) + " V";
        return status;
    }

    void updateFromCommand(const String& cmd) {
        int firstSpace = cmd.indexOf(' ');
        int secondSpace = cmd.indexOf(' ', firstSpace + 1);
        if (firstSpace == -1 || secondSpace == -1) return;

        String tipo = cmd.substring(firstSpace + 1, secondSpace);
        float valor = cmd.substring(secondSpace + 1).toFloat();

        if (tipo == "suelo") minSoilMoisture = valor;
        else if (tipo == "temp") maxTemperature = valor;
        else if (tipo == "co2") maxCO2 = valor;
        else if (tipo == "luz") minLight = (int)valor;
        else if (tipo == "voltaje") minVoltage = valor;
    }

    String formatSensorData(const SensorData& d) const {
        String msg = "";
        msg += "🌡️ Temp: " + String(d.temperature, 1) + " °C\n";
        msg += "💧 Hum: " + String(d.humidity, 1) + " %\n";
        msg += "☀️ Luz: " + String(d.light) + "\n";
        msg += "🫁 CO2: " + String(d.co2ppm) + " ppm\n";
        msg += "🌱 Suelo: " + String(d.soilMoisture, 1) + " %\n";
        msg += "🔋 Volt: " + String(d.voltage, 2) + " V";
        return msg;
    }

    String formatActuatorState(const ActuatorState& state) const {
        String s = "";
        s += "💧 Bomba: " + String(state.waterPump ? "ON" : "OFF") + "\n";
        s += "🌀 Ventilador: " + String(state.fan ? "ON" : "OFF") + "\n";
        s += "💡 LEDs: " + String(state.leds ? "ON" : "OFF");
        return s;
    }
};

#endif
