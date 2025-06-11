/**
 * @file main.cpp
 * @brief Sistema de automatización VerdeVital usando ESP32 y actuadores
 * 
 * @details Este archivo contiene la implementación del sistema de automatización
 *          VerdeVital, que utiliza un ESP32 para controlar diversos actuadores
 *          como bombas, ventiladores y LEDs.
 *          El sistema recibe comandos a través de ESP-NOW y controla los actuadores.
 * 
 *          Los actuadores y sus pines respectivos son:
 *          - Bomba de agua: Pin 25
 *          - Ventilador: Pin 26
 *          - LEDs RGB: Pines 14, 27, 33, 32
 *          
 *          También se implementa ESP-NOW para la comunicación entre dispositivos (Edge y Actuadores).
 * 
 * @author
 *          Valentina Muñoz Arcos
 *          Luis Miguel Gómez Muñoz
 *          David Alejandro Ortega Flórez
 * 
 * @version 4.0
 * @date 2025-06-11
 */


/// Librerías
#include <Arduino.h>
#include "ESPNowReceiver.h"
#include "Rele.h"
#include "LedRGB.h"

// Pines definidos
#define PIN_BOMBA      25 ///< Pin para la bomba de agua
#define PIN_VENTILADOR 26 ///< Pin para el ventilador
#define PIN_LED_1      14 ///< Pin para el LED 1
#define PIN_LED_2      27 ///< Pin para el LED 2
#define PIN_LED_3      33 ///< Pin para el LED 3
#define PIN_LED_4      32 ///< Pin para el LED 4

// Instancia del receptor
/**
 * @brief Instancia del receptor ESP-NOW para recibir estados de actuadores
 * @param canal Canal de comunicación ESP-NOW 
 */
ESPNowActuatorReceiver receiver(1);  // Canal 1

/** 
 * @brief Instancias de los actuadores
 * @details
 *         - Bomba de agua: Controlada por un relé en el pin 25
 *         - Ventilador: Controlado por un relé en el pin 26
 *         - LEDs RGB: Cuatro instancias para controlar los LEDs en los pines 14, 27, 33 y 32
 * 
 */
Rele bomba(PIN_BOMBA);
Rele ventilador(PIN_VENTILADOR);
LedRGB led1(PIN_LED_1);
LedRGB led2(PIN_LED_2);
LedRGB led3(PIN_LED_3);
LedRGB led4(PIN_LED_4);

/**
 * @brief Configura el estado de todos los LEDs
 * @param state Estado lógico para encender los LEDs con 1 o apagarlos con 0
 * @return Este es un proceso sin retorno
 */
void setAllLeds(bool state) {
    if (state) {
        led1.on(); led2.on(); led3.on(); led4.on();
    } else {
        led1.off(); led2.off(); led3.off(); led4.off();
    }
}

/**
 * @brief Función principal para el sistema de actuadores
 * @details Dependiendo de el parametro recibido activa o desactiva un actuador en especifico
 * @param state ActuadorState | Contiene información sobre el estado de los actuadores
 */
void onActuatorStateReceived(const ActuatorState& state) {
    Serial.println("🔄 Callback ejecutado con datos recibidos:");
    Serial.print("💧 Bomba: "); Serial.println(state.waterPump ? "Encendida" : "Apagada");
    Serial.print("🌬️ Ventilador: "); Serial.println(state.fan ? "Encendido" : "Apagado");
    Serial.print("💡 LEDs: "); Serial.println(state.leds ? "Encendidos" : "Apagados");

    bomba.setState(state.waterPump);
    ventilador.setState(state.fan);
    setAllLeds(state.leds);
}

/**
 * @brief Función principal del ESP-Edge
 * @details Se encarga de incializar todos los actuadores y el sistema ESP-NPW
 * @return Es un proceso sin retorno
 */
void setup() {
    Serial.begin(115200);
    bomba.begin();
    ventilador.begin();
    led1.begin();
    led2.begin();
    led3.begin();
    led4.begin();

    receiver.begin();
    receiver.onReceive(onActuatorStateReceived);

    Serial.println("🔌 Sistema receptor listo.");
}

void loop() {
}
