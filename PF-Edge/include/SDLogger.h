#ifndef SDLOGGER_H
#define SDLOGGER_H

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include "dataSensor.h" // Para usar SensorData

class SDLogger {
public:
    SDLogger(int csPin = 5) : _csPin(csPin) {}

    bool begin() {
        return SD.begin(_csPin);
    }

    bool logSensorData(const String& timestamp, const String& nodeId, int rssi, const SensorData& data) {
        String folderPath = getFolderPath(timestamp);
        String filePath = getFilePath(timestamp);

        ensureDirectoriesExist(folderPath);

        File file = SD.open(filePath, FILE_APPEND);
        if (!file) {
            Serial.println("❌ No se pudo abrir el archivo para escritura.");
            return false;
        }

        // Si el archivo estaba vacío, escribe encabezados
        if (file.size() == 0) {
            file.println("timestamp,nodeId,rssi,temp,hum,light,co2ppm,soilMoisture,voltage");
        }

        String csvLine = timestamp + "," + nodeId + "," + String(rssi) + "," +
                         String(data.temperature, 2) + "," +
                         String(data.humidity, 2) + "," +
                         String(data.light) + "," +
                         String(data.co2ppm) + "," +
                         String(data.soilMoisture) + "," +
                         String(data.voltage, 2);

        file.println(csvLine);
        file.close();
        return true;
    }

private:
    int _csPin;

    String getFolderPath(const String& timestamp) {
        String date = timestamp.substring(0, 10);    // YYYY-MM-DD
        String hour = timestamp.substring(11, 13);   // HH
        return "/" + date + "/" + hour;
    }

    String getFilePath(const String& timestamp) {
        return getFolderPath(timestamp) + "/data.csv";
    }

    void ensureDirectoriesExist(const String& folderPath) {
        // Extrae las partes intermedias y crea directorios uno a uno
        int secondSlash = folderPath.indexOf('/', 1); // omitimos el primer '/'
        if (secondSlash == -1) return;

        String firstLevel = folderPath.substring(0, secondSlash); // /YYYY-MM-DD
        String secondLevel = folderPath;                          // /YYYY-MM-DD/HH

        if (!SD.exists(firstLevel)) {
            SD.mkdir(firstLevel);
        }

        if (!SD.exists(secondLevel)) {
            SD.mkdir(secondLevel);
        }
    }
};

#endif
