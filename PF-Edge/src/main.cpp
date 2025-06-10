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
#include "DisplayManager.h"
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

// Control de pantalla OLED
#define BUTTON_PIN 27
DisplayManager display(false);

// 🔢 Umbrales
Thresholds thresholds;

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

// Actualiza la pantalla OLED con los datos actuales
void actualizarDisplay() {
    portENTER_CRITICAL(&dataMux);
    SensorData copy = lastReceivedData;
    ActuatorState state = actuatorState;
    portEXIT_CRITICAL(&dataMux);

    display.setAlerta(
        thresholds.hasCriticalFailure(),
        thresholds.alertLowSoilMoisture,
        thresholds.alertHighSoilMoisture,
        thresholds.alertLowTemperature,
        thresholds.alertHighTemperature,
        thresholds.alertCO2,
        thresholds.alertLight,
        thresholds.alertVoltage,
        thresholds.alertWifi,
        thresholds.alertESPSensor,
        thresholds.alertESPActuator
    );

    display.setHora(rtc.getTime());
    display.setFecha(rtc.getDate());
    display.setWifiStatus(WiFi.status() == WL_CONNECTED ? "OK" : "FAIL");
    display.setSensorData(copy, String(thresholds.RSSIWiFi));
    display.setActuadorEstado(actuatorState);
}

// 🔁 Callback al recibir datos
void onSensorDataReceived(const SensorData& data) {
    portENTER_CRITICAL(&dataMux);
    lastReceivedData = data;
    hasData = true;
    actuatorState = thresholds.evaluate(data);
    portEXIT_CRITICAL(&dataMux);

    actuatorSender.sendData(actuatorState);
    thresholds.alertESPActuator = !actuatorSender.isPeerConnected();

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
void TelegramReceiverTask(void* pvParameters) {
    // Mostrar mensaje de inicio
    bot.sendMessage("✅ Bot de Telegram iniciado. Usa /guia para ver comandos disponibles.");

    while (true) {
        String cmd = bot.getNextMessage(lastUpdateId);

        if (thresholds.hasAlertChanged()) {
            String alertMsg = thresholds.returnAlarm(lastReceivedData);
            if (!alertMsg.isEmpty()) {
                bot.sendMessage(alertMsg);
            } else {
                bot.sendMessage("✅ Sistema funcionando correctamente.");
            }
        }

        if (cmd == "/datos") {
            display.setTelegramCmd(cmd);
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

        } else if (cmd == "/estado") {
            display.setTelegramCmd(cmd);
            String alertMsg = thresholds.returnAlarm(lastReceivedData);
            if (!alertMsg.isEmpty()) {
                bot.sendMessage(alertMsg);
            } else {
                bot.sendMessage("✅ Sistema funcionando correctamente.");
            }
        
        } else if (cmd.startsWith("/umbral ")) {
            display.setTelegramCmd(cmd);
            thresholds.updateFromCommand(cmd);
            bot.sendMessage("✅ Umbrales actualizados.");

        } else if (cmd == "/umbrales") {
            display.setTelegramCmd(cmd);
            bot.sendMessage(thresholds.getStatus());

        } else if (cmd.startsWith("/activar ") || cmd.startsWith("/desactivar ")) {
            display.setTelegramCmd(cmd);
            portENTER_CRITICAL(&dataMux);
            if (cmd.endsWith("bomba")) actuatorState.waterPump = cmd.startsWith("/activar ") ? 1 : 0;
            else if (cmd.endsWith("ventilador")) actuatorState.fan = cmd.startsWith("/activar ") ? 1 : 0;
            else if (cmd.endsWith("luces")) actuatorState.leds = cmd.startsWith("/activar ") ? 1 : 0;
            actuatorSender.sendData(actuatorState);
            portEXIT_CRITICAL(&dataMux);
            bot.sendMessage("✅ Actuador actualizado.");

        } else if (cmd == "/actuadores") {
            display.setTelegramCmd(cmd);
            portENTER_CRITICAL(&dataMux);
            ActuatorState state = actuatorState; 
            portEXIT_CRITICAL(&dataMux);
            bot.sendMessage("🔌 Estado:\n" + thresholds.formatActuatorState(state));

        } else if (cmd == "/guia"){
            display.setTelegramCmd(cmd);
            String guide = "📖 Guía de comandos:\n";
            guide += "/datos - Obtener últimos datos de sensores.\n";
            guide += "/estado - Ver estado del sistema y alertas.\n";
            guide += "/umbral <tipo> <valor> - Actualizar umbrales (ej: /umbral suelo_min 40.0).\n";
            guide += "/umbrales - Mostrar umbrales actuales.\n";
            guide += "/activar <actuador> - Activar un actuador (bomba, ventilador, luces).\n";
            guide += "/desactivar <actuador> - Desactivar un actuador (bomba, ventilador, luces).\n";
            guide += "/actuadores - Mostrar estado de los actuadores.\n";
            bot.sendMessage(guide);

        } else{
            if (cmd.length() > 0) {
                display.setTelegramCmd(cmd);
                bot.sendMessage("⚠️ Comando no reconocido. Usa /guia para ayuda.");
            }
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// Tarea 3: guardar en SD
void SDLoggerTask(void* pvParameters) {
    int systemState = 0; // Estado del sistema (0: Todo OK, 1: Fallo conectividad, 2: Sensor desbordado, 3: Falla crítica)
    while (true) {
        portENTER_CRITICAL(&dataMux);
        bool available = hasData;
        SensorData copy = lastReceivedData;
        portEXIT_CRITICAL(&dataMux);

        if (thresholds.alertESPActuator || thresholds.alertWifi || thresholds.alertESPSensor) {
            systemState = 1; // Fallo de conectividad
        } else if (thresholds.anySensorAlertActive()) {
            systemState = 2; // Sensor desbordado
        } else if (thresholds.hasCriticalFailure()) {
            systemState = 3; // Falla crítica
        } else {
            systemState = 0; // Todo OK
        }
 
        if (available) {
            String timestamp = rtc.getTimestamp();
            logger.logSensorData(timestamp, "NODE_1", wifi.getRSSI(), copy, systemState);
        }

        vTaskDelay(10000 / portTICK_PERIOD_MS);

        thresholds.RSSIWiFi = WiFi.RSSI();
        receiver.update();
        thresholds.alertESPSensor = !receiver.isConnected();
    }
}

// Tarea 4: actualizar RTC
void RTCUpdateTask(void* pvParameters) {
    while (true) {
        syncRtcWithNTP();
        vTaskDelay(1800000 / portTICK_PERIOD_MS);
    }
}

// Tarea 5: verificar fecha y hora
void checkRtcTime(void* pvParameters) {
    while (true) {
        int year = rtc.getYear();
        int month = rtc.getMonth();
        int day = rtc.getDay();
        int hour = rtc.getHour();
        int minute = rtc.getMinute();
        int second = rtc.getSecond();

        // Verificar si la hora es inválida (por ejemplo, año fuera de rango razonable)
        if (year < 2020 || year > 2100 || month < 1 || month > 12 || day < 1 || day > 31 ||
            hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
            Serial.println("⏰ Hora RTC inválida, sincronizando con NTP...");
            syncRtcWithNTP();
        }

        vTaskDelay(60000 / portTICK_PERIOD_MS); // Verificar cada minuto
    }
}

// Tarea 6: actualizar pantalla OLED
void DisplayTask(void* pvParameters) {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    unsigned long lastPress = 0;

    while (true) {
        display.actualizar();

        if (digitalRead(BUTTON_PIN) == LOW && (millis() - lastPress > 300)) {
            actualizarDisplay();
            lastPress = millis();
            display.cambiarPagina();
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!wifi.connect()) {
        Serial.println("🚫 WiFi no disponible.");
        while (true);
    }

    thresholds.RSSIWiFi = wifi.getRSSI();
 
    rtc.begin();
    syncRtcWithNTP();

    if (!logger.begin()) {
        Serial.println("⚠️ Error al iniciar SD");
    }

    receiver.begin();
    actuatorSender.begin();

    display.begin();
    display.mostrarPagina(); 

    xTaskCreatePinnedToCore(ReceiveDataTask, "ReceiveData", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(TelegramReceiverTask, "Telegram", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(SDLoggerTask, "SDLogger", 4096, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(RTCUpdateTask, "RTCUpdate", 4096, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(checkRtcTime, "CheckRTC", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(DisplayTask, "Display", 4096, NULL, 1, NULL, 1);

    Serial.println("✅ Sistema iniciado.");
}

void loop() {
    // No usar loop() cuando trabajas con FreeRTOS
}
