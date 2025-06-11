# VerdeVital - PF-Sensores

PF-Sensores es uno de los tres módulos principales del ecosistema VerdeVital. Este proyecto se encarga de la adquisición de datos de múltiples sensores ambientales mediante un ESP32, utilizando FreeRTOS para gestionar tareas concurrentes y ESP-NOW para transmitir datos de manera inalámbrica. La información recogida se integra con los módulos PF-Actuadores y PF-Edge para ofrecer una solución integral de monitoreo y control ambiental.

## Características

- **Lectura de Sensores Ambientales**  
  Se capturan datos de:
  - **DHT:** Medición de temperatura y humedad (por ejemplo, DHT11).
  - **LDR:** Detección de intensidad lumínica (valor analógico entre 0 y 1023).
  - **MQ135:** Evaluación de la calidad del aire mediante la medición de CO2 (ppm).
  - **YL-69:** Medición de la humedad del suelo, convertida a un valor porcentual.
  - **Sensor de Voltaje:** Lectura del voltaje presente en un divisor de voltaje.

- **Estructura de Datos Unificada**  
  Todos los valores se agrupan en la estructura definida en `sensorData.h`, que almacena:
  - Temperatura (ºC)
  - Humedad (%)
  - Luz
  - CO2 (ppm)
  - Humedad del suelo (%)
  - Voltaje

- **Comunicación Inalámbrica con ESP-NOW**  
  La clase `ESPNowSender` (en `ESPNowSender.h`) gestiona la configuración y el envío de los datos mediante ESP-NOW, estableciendo el modo WiFi en STA, configurando el canal de comunicación y registrando callbacks para el monitoreo del estado de envío.

- **Multitarea con FreeRTOS**  
  Se ejecuta una tarea periódica (cada 3 segundos) que:
  - Lee los datos de cada sensor.
  - Los almacena en la estructura `SensorData`.
  - Envía la información al nodo receptor configurado.

## Tecnologías y Plataforma

- **Microcontrolador:** ESP32
- **Lenguaje:** C/C++
- **Sistema Operativo en Tiempo Real:** FreeRTOS
- **Comunicación Inalámbrica:** ESP-NOW

## Configuración y Pines

El firmware se configura en `src/main.cpp` con los siguientes pines:

- **Sensor DHT:**  
  - Pin: `4`  
  - Tipo: `DHT11`

- **Sensor LDR:**  
  - Pin: `34`

- **Sensor MQ135:**  
  - Pin: `35`

- **Sensor YL-69:**  
  - Pin: `32`

- **Sensor de Voltaje:**  
  - Pin: `33`

Además, se define la dirección MAC del receptor y el canal de comunicación para ESP-NOW:
- Dirección MAC: `{0xE8, 0x6B, 0xEA, 0xDF, 0x21, 0x0C}`
- Canal: `2`

## Implantación y Uso

1. **Instalación del Entorno:**
   - Clona el repositorio
   - Abre el proyecto en [PlatformIO](https://platformio.org/).
   - Instala las dependencias necesarias (bibliotecas para el manejo de sensores y ESP-NOW).

2. **Configuración del Hardware:**
   - Conecta el ESP32 y los sensores de acuerdo con los pines especificados en [main.cpp](http://_vscodecontentref_/2).
   - Verifica que las conexiones y la alimentación sean correctas.

3. **Compilación y Subida:**
   - Compila el firmware mediante PlatformIO.
   - Sube el firmware al ESP32.

4. **Ejecución:**
   - El ESP32 inicializa los sensores, configura la comunicación ESP-NOW y arranca la tarea FreeRTOS que lee y transmite los datos periódicamente.
   - Los datos transmitidos se integran con PF-Actuadores y PF-Edge para ofrecer control y visualización en tiempo real.

## Integración con el Ecosistema VerdeVital

PF-Sensores se integra con:
- **PF-Actuadores:** Recibe los datos sensoriales y controla dispositivos (bombas, ventiladores, LEDs, etc.).
- **PF-Edge:** Proporciona visualización en tiempo real, almacenamiento histórico y notificaciones (por ejemplo, mediante interfaces web o aplicaciones móviles).

## Contribuciones

Las contribuciones son bienvenidas. Si deseas proponer mejoras o reportar problemas, abre un issue o envía un pull request.

## Licencia

Este proyecto se distribuye bajo la Licencia MIT.

---

Desarrollado por el equipo de VerdeVital.
