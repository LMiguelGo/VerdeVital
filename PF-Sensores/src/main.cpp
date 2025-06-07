#include <Arduino.h>
#include "sensor_DHT.h"
#include "sensor_LDR.h"
#include "sensor_MQ135.h"
#include "sensor_YL69.h"
#include "sensor_Voltage.h"
#include "sensorData.h"
#include "ESPNowSender.h"

// Pines
#define DHT_PIN 4
#define DHT_TYPE DHT11
#define LDR_PIN 34
#define MQ135_PIN 35
#define YL69_PIN 32
#define VOLTAGE_PIN 33

// Dirección MAC del receptor
uint8_t receiverMac[] = {0xE8, 0x6B, 0xEA, 0xDF, 0x21, 0x0C};
#define CHANNEL 2

// Instancias de sensores
SensorDHT dhtSensor(DHT_PIN, DHT_TYPE);
SensorLDR ldrSensor(LDR_PIN);
SensorMQ135 mq135Sensor(MQ135_PIN);
SensorYL69 yl69Sensor(YL69_PIN);
VoltageSensor voltageSensor(VOLTAGE_PIN);

// Sender ESP-NOW
ESPNowSender espNowSender(receiverMac, CHANNEL);

// Datos del sensor
SensorData sensorData;

// Protege acceso concurrente a sensorData
portMUX_TYPE sensorMux = portMUX_INITIALIZER_UNLOCKED;

// Tarea: Lectura de sensores
void taskReadSensors(void *parameter) {
  while (true) {
    // Leer DHT11 fuera de la sección crítica
    // para evitar bloqueos por tiempo de espera
    float temp = dhtSensor.readTemperature();
    float hum  = dhtSensor.readHumidity();

    portENTER_CRITICAL(&sensorMux);
    sensorData.temperature   = temp;
    sensorData.humidity      = hum;
    sensorData.light         = ldrSensor.readLight();
    sensorData.co2ppm        = mq135Sensor.readCO2();
    sensorData.soilMoisture  = yl69Sensor.readPercentage();
    sensorData.voltage       = voltageSensor.readVoltage();
    portEXIT_CRITICAL(&sensorMux);

    vTaskDelay(pdMS_TO_TICKS(1500));  // Leer cada 1.5 segundos
  }
}

// Tarea: Enviar datos por ESP-NOW
void taskSendESPNow(void *parameter) {
  while (true) {
    SensorData dataCopy;

    portENTER_CRITICAL(&sensorMux);
    dataCopy = sensorData;
    portEXIT_CRITICAL(&sensorMux);

    espNowSender.sendData(dataCopy);
    vTaskDelay(pdMS_TO_TICKS(3000));  // Enviar cada 3 segundos
  }
} 

void setup() {
  Serial.begin(115200);
  delay(2000);
  dhtSensor.begin();
  ldrSensor.begin();
  mq135Sensor.begin();
  yl69Sensor.begin();
  espNowSender.begin();

  // Crear tareas FreeRTOS
  xTaskCreatePinnedToCore(taskReadSensors, "LeerSensores", 4096, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(taskSendESPNow, "EnviarESPNow", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Nada que hacer en loop
}
