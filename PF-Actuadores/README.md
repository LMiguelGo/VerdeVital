# VerdeVital - PF-Actuadores

Este repositorio contiene el código fuente de **PF-Actuadores**, uno de los tres proyectos fundamentales del sistema integral **VerdeVital**. Este proyecto se encarga de controlar los actuadores (bomba de agua, ventilador y LEDs RGB) de manera remota mediante el protocolo ESP-NOW, comunicándose con el dispositivo central **PF-Edge**.

## Arquitectura del Sistema

El sistema **VerdeVital** se compone de tres proyectos principales:

1. **PF-Sensores:** Captura datos ambientales y los envía al dispositivo central.
2. **PF-Edge:** Recibe la información de los sensores, procesa los datos y envía comandos a los actuadores.
3. **PF-Actuadores:** Este proyecto, que recibe los comandos de PF-Edge y controla físicamente los dispositivos (bomba, ventilador, LEDs).

## Funcionamiento General

- **Inicialización:**  
  - Configuración de los pines del ESP32 para cada actuador.
  - Inicialización del sistema ESP-NOW para la comunicación inalámbrica.

- **Recepción de Comandos:**  
  - El receptor ESP-NOW escucha los mensajes enviados por PF-Edge.
  - La función `onActuatorStateReceived` es invocada con la información del estado de cada actuador.
  
- **Control de Actuadores:**  
  - Se actualizan los estados de la bomba, ventilador y LEDs según los comandos recibidos.
  - Los pines de salida se configuran para encender o apagar los dispositivos de acuerdo a la señal.

## Componentes Clave del Código

- **`src/main.cpp`:**  
  Archivo principal que implementa la lógica del sistema:
  - **Definición de Pines:**  
    ```cpp
    #define PIN_BOMBA      25 ///< Pin para la bomba de agua
    #define PIN_VENTILADOR 26 ///< Pin para el ventilador
    #define PIN_LED_1      14 ///< Pin para el LED 1
    #define PIN_LED_2      27 ///< Pin para el LED 2
    #define PIN_LED_3      33 ///< Pin para el LED 3
    #define PIN_LED_4      32 ///< Pin para el LED 4
    ```
  - **Inicialización de Actuadores:**  
    Se crea una instancia para cada actuador utilizando las clases `Rele` y `LedRGB`.
  - **Callback (`onActuatorStateReceived`):**  
    Función que procesa los datos recibidos y actualiza el estado de los actuadores.
  - **Configuración del ESP-NOW:**  
    Se inicia el receptor en un canal específico y se asocia el callback.

- **Clases de Actuadores y Comunicación:**  
  - **ESPNowActuatorReceiver:** Encargada de la comunicación ESP-NOW.
  - **Rele:** Maneja el control de la bomba de agua y el ventilador.
  - **LedRGB:** Controla los LEDs RGB.

## Requisitos

- **Hardware:**
  - ESP32.
  - Actuadores físicos: bomba de agua, ventilador y LEDs RGB.
  
- **Software:**
  - Arduino IDE con soporte para ESP32.
  - Librerías requeridas: `ESPNowReceiver`, `Rele`, y `LedRGB` (asegúrese de instalarlas a través del Administrador de Librerías).

## Instalación y Configuración

1. **Configurar el ESP32 en el Arduino IDE:**  
   Instale y configure el soporte para ESP32 según las [instrucciones oficiales](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).

2. **Instalar Librerías Necesarias:**  
   Utilice el Administrador de Librerías del Arduino IDE para instalar las dependencias.

3. **Cargar el Código:**  
   Abra [main.cpp](http://_vscodecontentref_/0), seleccione el puerto y placa correctos para su ESP32 y cargue el proyecto.

4. **Conectar los Actuadores:**  
   - Asigne el hardware a los pines indicados:
     - Bomba de agua: Pin 25.
     - Ventilador: Pin 26.
     - LEDs RGB: Pines 14, 27, 33 y 32.

5. **Verificar la Comunicación:**  
   Asegúrese de que el canal configurado para ESP-NOW en el PF-Edge coincide con el definido en este proyecto.

## Consideraciones

- **Compatibilidad de Canales:**  
  Verificar que el canal ESP-NOW usado por el dispositivo Edge esté correctamente configurado en el ESP32 de los actuadores.
  
- **Conexiones Físicas:**  
  Revise las conexiones de hardware para garantizar un correcto funcionamiento de los actuadores.

- **Integración:**  
  Este proyecto forma parte de un sistema mayor. La correcta interacción entre PF-Sensores, PF-Edge y PF-Actuadores es esencial para el funcionamiento integral.

## Próximos Pasos y Potenciales Mejoras

- Ampliar el control de los LEDs para incluir variaciones de color y brillo.
- Incorporar más actuadores conforme sea necesario.
- Implementar un sistema de registro/log para facilitar la depuración.

---

Disfruta programando y mejorando el sistema VerdeVital.