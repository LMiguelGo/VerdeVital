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

// Instancias
SensorDHT dhtSensor(DHT_PIN, DHT_TYPE);
SensorLDR ldrSensor(LDR_PIN);
SensorMQ135 mq135Sensor(MQ135_PIN);
SensorYL69 yl69Sensor(YL69_PIN);
VoltageSensor voltageSensor(VOLTAGE_PIN);
ESPNowSender espNowSender(receiverMac, CHANNEL);

// Tarea única: Leer sensores y enviar
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

    vTaskDelay(pdMS_TO_TICKS(3000)); // Ejecutar cada 3 segundos
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

  // Crear una sola tarea
  xTaskCreatePinnedToCore(taskReadAndSend, "LeerYEnviar", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Nada que hacer aquí
}
