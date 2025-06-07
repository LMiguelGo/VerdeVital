#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

class DisplayManager {
private:
    Adafruit_SSD1306 display;
    unsigned long lastPageChange = 0;
    uint8_t currentPage = 0;
    const uint8_t totalPages = 4;
    unsigned long interval = 3000; // Tiempo entre páginas en ms

    // Datos simulados / de prueba (debes actualizar desde tu código real)
    String wifiStatus = "OK";
    String hora = "00:00:00";
    String sensorData = "T:-- H:--";
    String actuadorEstado = "Luces: OFF";
    String telegramCmd = "/start";
    String alerta = "";

public:
    DisplayManager()
        : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1) {}

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

    void setWifiStatus(const String& status) {
        wifiStatus = status;
    }

    void setHora(const String& h) {
        hora = h;
    }

    void setSensorData(const String& datos) {
        sensorData = datos;
    }

    void setActuadorEstado(const String& estado) {
        actuadorEstado = estado;
    }

    void setTelegramCmd(const String& cmd) {
        telegramCmd = cmd;
    }

    void setAlerta(const String& msg) {
        alerta = msg;
    }

    void mostrarPagina() {
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);

        switch (currentPage) {
            case 0:
                display.println("Sistema activo");
                display.print("WiFi: "); display.println(wifiStatus);
                display.print("Hora: "); display.println(hora);
                break;

            case 1:
                display.println("Sensores:");
                display.println(sensorData);
                break;

            case 2:
                display.println("Actuadores:");
                display.println(actuadorEstado);
                break;

            case 3:
                display.println("Telegram:");
                display.print("Cmd: "); display.println(telegramCmd);
                if (alerta != "") {
                    display.println(alerta);
                }
                break;
        }

        display.display();
    }

    void actualizar() {
        unsigned long now = millis();
        if (now - lastPageChange >= interval) {
            currentPage = (currentPage + 1) % totalPages;
            mostrarPagina();
            lastPageChange = now;
        }
    }

    void mostrarMensajeRapido(const String& msg) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.println(msg);
        display.display();
    }
};

#endif
