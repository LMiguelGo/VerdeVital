/**
 * @file main.cpp
 * @brief Programa principal para leer sensores y enviar datos usando ESP-NOW.
 * Este programa lee datos de varios sensores (DHT, LDR, MQ135, YL69, voltaje)
 * y envía los datos a través de ESP-NOW a un dispositivo receptor específico.
 * El programa utiliza FreeRTOS para ejecutar la lectura y envío de datos en una tarea separada.
 * @details
 * - Utiliza la biblioteca DHT para leer temperatura y humedad.
 * - Utiliza un LDR para medir la luz ambiental.
 * - Utiliza un sensor MQ135 para medir la calidad del aire (CO2).
 * - Utiliza un sensor YL69 para medir la humedad del suelo.
 * - Utiliza un divisor de voltaje para medir el voltaje de entrada.
 * - Envía los datos a través de ESP-NOW a un dispositivo con una dirección MAC específica.
 */


// Librerías
#include <Arduino.h>
#include "sensor_DHT.h"
#include "sensor_LDR.h"
#include "sensor_MQ135.h"
#include "sensor_YL69.h"
#include "sensor_Voltage.h"
#include "sensorData.h"
#include "ESPNowSender.h"

// Pines
#define DHT_PIN 4 ///< Pin al que está conectado el sensor DHT
#define DHT_TYPE DHT11 ///< Tipo de sensor DHT (DHT11, DHT22, etc.)
#define LDR_PIN 34 ///< Pin al que está conectado el sensor LDR
#define MQ135_PIN 35 ///< Pin al que está conectado el sensor MQ135
#define YL69_PIN 32 ///< Pin al que está conectado el sensor YL-69
#define VOLTAGE_PIN 33 ///< Pin al que está conectado el sensor de voltaje

// Dirección MAC del receptor 
uint8_t receiverMac[] = {0xE8, 0x6B, 0xEA, 0xDF, 0x21, 0x0C}; ///< Dirección MAC del dispositivo receptor (6 bytes)
#define CHANNEL 2 ///< Canal de comunicación para ESP-NOW (1-14)

// Instancias
SensorDHT dhtSensor(DHT_PIN, DHT_TYPE); ///< Sensor DHT para temperatura y humedad
SensorLDR ldrSensor(LDR_PIN); ///< Sensor LDR para medir luz ambiental
SensorMQ135 mq135Sensor(MQ135_PIN); ///< Sensor MQ135 para medir calidad del aire (CO2)
SensorYL69 yl69Sensor(YL69_PIN); ///< Sensor YL69 para medir humedad del suelo
VoltageSensor voltageSensor(VOLTAGE_PIN); ///< Sensor de voltaje
ESPNowSender espNowSender(receiverMac, CHANNEL); ///< Sender ESP-NOW

/**
 * @brief Tarea que lee los sensores y envía los datos a través de ESP-NOW.
 * Esta tarea se ejecuta en un bucle infinito, leyendo los sensores cada 3 segundos
 * y enviando los datos al dispositivo receptor.
 * @param parameter Parámetro de entrada (no utilizado).
 */
void taskReadAndSend(void *parameter) {
  while (true) {
    SensorData data;

    // Leer sensores
    data.temperature  = dhtSensor.readTemperature();
    data.humidity     = dhtSensor.readHumidity();
    data.light        = ldrSensor.readLight();
    data.co2ppm       = mq135Sensor.readCO2();
    data.soilMoisture = yl69Sensor.readPercentage();
    data.voltage      = voltageSensor.readVoltage();

    // Enviar datos
    espNowSender.sendData(data);

    vTaskDelay(pdMS_TO_TICKS(3000)); 
  }
}

/**
 * @brief Configuración inicial del programa.
 * Esta función se ejecuta una vez al inicio del programa.
 */
void setup() {
  Serial.begin(115200);
  
  dhtSensor.begin();
  ldrSensor.begin();
  mq135Sensor.begin();
  yl69Sensor.begin();
  espNowSender.begin();

  // Crear una sola tarea
  xTaskCreatePinnedToCore(taskReadAndSend, "LeerYEnviar", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Nada que hacer aquí
}
