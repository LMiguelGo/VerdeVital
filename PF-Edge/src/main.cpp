#include <Arduino.h>
#include "WiFiConnector.h"
#include "ESPNowReceiver.h"
#include "ESPNowSender.h"
#include "TelegramBot.h"
#include "ThresholdsController.h"
#include "dataSensor.h"
#include "dataActuator.h"
#include "SDLogger.h"
#include "RtcDS1302Helper.h"
#include <time.h>

// 📡 WiFi
const char* ssid = "Jose";
const char* password = "Viani1992";
WiFiConnector wifi(ssid, password);

// 🤖 Telegram
const String botToken = "7801122874:AAFSDyTc5P3iAw8Z5msvaTbN2O_DJXYEAlw";
const String chatId = "5152788448";
TelegramBot bot(botToken, chatId);
int lastUpdateId = 0;

// 🧠 RTC y SD
RtcDS1302Helper rtc(15, 14, 2);
SDLogger logger(5);

// 🌡️ Datos compartidos
SensorData lastReceivedData;
ActuatorState actuatorState;
bool hasData = false;

// Protecciones contra acceso concurrente
portMUX_TYPE dataMux = portMUX_INITIALIZER_UNLOCKED;

// 📥 ESP-NOW
ESPNowReceiver receiver(2);

// 🟢 Actuadores
const uint8_t actuatorMAC[] = {0xEC, 0xE3, 0x34, 0x8A, 0x55, 0xA0};
ESPNowActuatorSender actuatorSender(actuatorMAC, 2);

// 🔢 Umbrales
Thresholds thresholds;

// 🚨 Notificación
void notifyThresholdExceed(const String& tipo, float valor) {
    String mensaje = "⚠️ Umbral de " + tipo + " excedido: " + String(valor);
    bot.sendMessage(mensaje);
}

// ⏰ NTP
void syncRtcWithNTP() {
    configTime(-5 * 3600, 0, "pool.ntp.org");
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        rtc.updateFromNTP(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1,
                          timeinfo.tm_mday, timeinfo.tm_hour,
                          timeinfo.tm_min, timeinfo.tm_sec);
        Serial.println("✅ RTC sincronizado con NTP");
    } else {
        Serial.println("⚠️ No se pudo sincronizar RTC con NTP"); 
    }
}

// 🔁 Callback al recibir datos
void onSensorDataReceived(const SensorData& data) {
    portENTER_CRITICAL(&dataMux);
    lastReceivedData = data;
    hasData = true;
    actuatorState = thresholds.evaluate(data);
    portEXIT_CRITICAL(&dataMux);

    actuatorSender.sendData(actuatorState);

    Serial.println("📥 Datos recibidos:");
    Serial.println(thresholds.formatSensorData(data));
}

// Tarea 1: recibir datos de sensores
void ReceiveDataTask(void* pvParameters) {
    receiver.onReceive(onSensorDataReceived);
    while (true) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

// Tarea 2: comandos Telegram
void TelegramTask(void* pvParameters) {
    while (true) {
        String cmd = bot.getNextMessage(lastUpdateId);

        // Verificar el estado de los umbrales
        String alerta = thresholds.checkThresholds(lastReceivedData);
        if (!alerta.isEmpty()) {
            vTaskDelay(1000 / portTICK_PERIOD_MS); // Evita spam
            bot.sendMessage("⚠️ ALERTA DE UMBRAL ⚠️\n" + alerta);
        }


        if (cmd == "/datos") {
            portENTER_CRITICAL(&dataMux);
            bool available = hasData;
            SensorData copy = lastReceivedData;
            portEXIT_CRITICAL(&dataMux);

            if (available) {
                String msg = "📊 Últimos datos:\n";
                msg += "🕒 " + rtc.getTimestamp() + "\n";
                msg += thresholds.formatSensorData(copy);
                bot.sendMessage(msg);
            } else {
                bot.sendMessage("⏳ Aún no hay datos.");
            }

        } else if (cmd.startsWith("/umbral ")) {
            thresholds.updateFromCommand(cmd);
            bot.sendMessage("✅ Umbrales actualizados.");

        } else if (cmd == "/umbrales") {
            bot.sendMessage(thresholds.getStatus());

        } else if (cmd.startsWith("/activar ") || cmd.startsWith("/desactivar ")) {
            portENTER_CRITICAL(&dataMux);
            if (cmd.endsWith("bomba")) actuatorState.waterPump = cmd.startsWith("/activar ") ? 1 : 0;
            else if (cmd.endsWith("ventilador")) actuatorState.fan = cmd.startsWith("/activar ") ? 1 : 0;
            else if (cmd.endsWith("luces")) actuatorState.leds = cmd.startsWith("/activar ") ? 1 : 0;
            actuatorSender.sendData(actuatorState);
            portEXIT_CRITICAL(&dataMux);
            bot.sendMessage("✅ Actuador actualizado.");

        } else if (cmd == "/estado") {
            portENTER_CRITICAL(&dataMux);
            ActuatorState state = actuatorState;
            portEXIT_CRITICAL(&dataMux);
            bot.sendMessage("🔌 Estado:\n" + thresholds.formatActuatorState(state));
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Tarea 3: guardar en SD
void SDLoggerTask(void* pvParameters) {
    while (true) {
        portENTER_CRITICAL(&dataMux);
        bool available = hasData;
        SensorData copy = lastReceivedData;
        portEXIT_CRITICAL(&dataMux);

        if (available) {
            String timestamp = rtc.getTimestamp();
            logger.logSensorData(timestamp, "NODE_1", -40, copy);
        }

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}

// Tarea 4: actualizar RTC
void RTCUpdateTask(void* pvParameters) {
    while (true) {
        syncRtcWithNTP();
        vTaskDelay(1800000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!wifi.connect()) {
        Serial.println("🚫 WiFi no disponible.");
        while (true);
    }

    rtc.begin();
    syncRtcWithNTP();

    if (!logger.begin()) {
        Serial.println("⚠️ Error al iniciar SD");
    }

    receiver.begin();
    actuatorSender.begin();

    xTaskCreatePinnedToCore(ReceiveDataTask, "ReceiveData", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(TelegramTask, "Telegram", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(SDLoggerTask, "SDLogger", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(RTCUpdateTask, "RTCUpdate", 4096, NULL, 1, NULL, 1);

    Serial.println("✅ Sistema iniciado.");
}

void loop() {
    // No usar loop() cuando trabajas con FreeRTOS
}
