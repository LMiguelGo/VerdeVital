/**
 * @file SDLogger.h
 * @brief Clase para registrar datos de sensores en una tarjeta SD.
 * Esta clase maneja la creación de carpetas y archivos, y el registro de datos en formato CSV.
 */
#ifndef SDLOGGER_H
#define SDLOGGER_H

// Librerías
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include "dataSensor.h"

/**
 * @class SDLogger
 * @brief Clase para manejar el registro de datos en una tarjeta SD.
 * 
 * Esta clase permite inicializar la tarjeta SD, crear directorios y archivos, y registrar datos de sensores en formato CSV.
 */
class SDLogger {
public:
    /**
     * @brief Constructor de la clase SDLogger.
     * @param csPin Pin CS (Chip Select) utilizado para la tarjeta SD. Por defecto es 5.
     */
    SDLogger(int csPin = 5) : _csPin(csPin) {}

    /**
     * @brief Inicializa la tarjeta SD.
     * @return true si la inicialización fue exitosa, false en caso contrario.
     */
    bool begin() {
        return SD.begin(_csPin);
    }

    /**
     * @brief Registra datos de sensores en un archivo CSV.
     * 
     * Crea los directorios necesarios y escribe los datos en un archivo CSV con encabezados si el archivo está vacío.
     * 
     * @param timestamp Marca de tiempo en formato "YYYY-MM-DD HH:MM:SS".
     * @param nodeId Identificador del nodo que envía los datos.
     * @param rssi Intensidad de la señal WiFi en dBm.
     * @param data Estructura con los datos de los sensores.
     * @param systemState Estado del sistema (por ejemplo, 0 para normal, 1 para alerta).
     * @return true si los datos se registraron correctamente, false en caso de error.
     */
    bool logSensorData(const String& timestamp, const String& nodeId, int rssi, const SensorData& data, int systemState) {
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
            file.println("timestamp,nodeId,rssi,temp,hum,light,co2ppm,soilMoisture,voltage,state");
        }

        String csvLine = timestamp + "," + nodeId + "," + String(rssi) + "," +
                         String(data.temperature, 2) + "," +
                         String(data.humidity, 2) + "," +
                         String(data.light) + "," +
                         String(data.co2ppm) + "," +
                         String(data.soilMoisture) + "," +
                         String(data.voltage, 2) + "," +
                         String(systemState);

        file.println(csvLine);
        file.close();
        return true;
    }

private:
    int _csPin; ///< Pin CS (Chip Select) utilizado para la tarjeta SD.

    /**
     * @brief Obtiene la ruta del directorio donde se almacenarán los datos.
     * 
     * La ruta se genera en función de la fecha y la hora de la marca de tiempo.
     * 
     * @param timestamp Marca de tiempo en formato "YYYY-MM-DD HH:MM:SS".
     * @return Ruta del directorio en formato "/YYYY-MM-DD/HH".
     */
    String getFolderPath(const String& timestamp) {
        String date = timestamp.substring(0, 10);    // YYYY-MM-DD
        String hour = timestamp.substring(11, 13);   // HH
        return "/" + date + "/" + hour;
    }

    /**
     * @brief Obtiene la ruta completa del archivo CSV donde se registrarán los datos.
     * 
     * @param timestamp Marca de tiempo en formato "YYYY-MM-DD HH:MM:SS".
     * @return Ruta completa del archivo en formato "/YYYY-MM-DD/HH/data.csv".
     */
    String getFilePath(const String& timestamp) {
        return getFolderPath(timestamp) + "/data.csv";
    }

    /**
     * @brief Crea los directorios necesarios para almacenar los datos.
     * 
     * Verifica si los directorios existen y los crea si es necesario.
     * 
     * @param folderPath Ruta del directorio en formato "/YYYY-MM-DD/HH".
     */
    void ensureDirectoriesExist(const String& folderPath) {
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
