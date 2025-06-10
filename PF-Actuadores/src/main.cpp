#include <Arduino.h>
#include "ESPNowReceiver.h"
#include "Rele.h"
#include "LedRGB.h"

// Pines definidos (ajusta según tu circuito)
#define PIN_BOMBA      25
#define PIN_VENTILADOR 26
#define PIN_LED_1      14
#define PIN_LED_2      27
#define PIN_LED_3      33
#define PIN_LED_4      32

// Instancia del receptor
ESPNowActuatorReceiver receiver(1);  // Canal 1

// Actuadores
Rele bomba(PIN_BOMBA);
Rele ventilador(PIN_VENTILADOR);
LedRGB led1(PIN_LED_1);
LedRGB led2(PIN_LED_2);
LedRGB led3(PIN_LED_3);
LedRGB led4(PIN_LED_4);

// Función para activar o desactivar los LEDs
void setAllLeds(bool state) {
    if (state) {
        led1.on(); led2.on(); led3.on(); led4.on();
    } else {
        led1.off(); led2.off(); led3.off(); led4.off();
    }
}

// Callback personalizado al recibir datos
void onActuatorStateReceived(const ActuatorState& state) {
    Serial.println("🔄 Callback ejecutado con datos recibidos:");
    Serial.print("💧 Bomba: "); Serial.println(state.waterPump ? "Encendida" : "Apagada");
    Serial.print("🌬️ Ventilador: "); Serial.println(state.fan ? "Encendido" : "Apagado");
    Serial.print("💡 LEDs: "); Serial.println(state.leds ? "Encendidos" : "Apagados");

    bomba.setState(state.waterPump);
    ventilador.setState(state.fan);
    setAllLeds(state.leds);
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Inicializar actuadores
    bomba.begin();
    ventilador.begin();
    led1.begin();
    led2.begin();
    led3.begin();
    led4.begin();

    // Inicializar receptor y registrar callback
    receiver.begin();
    receiver.onReceive(onActuatorStateReceived);

    Serial.println("🔌 Sistema receptor listo.");
}

void loop() {
    delay(1000);  // Esperar datos sin saturar el puerto serie
}
