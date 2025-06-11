# VerdeVital - PF-Edge
**PF-Edge** es uno de los tres módulos principales del ecosistema **VerdeVital**, diseñado para la gestión integral de invernaderos inteligentes. Este módulo actúa como el núcleo central del sistema, encargado de recibir, procesar y visualizar los datos provenientes de los sensores, así como de enviar comandos a los actuadores. Además, proporciona funcionalidades avanzadas como registro de datos, generación de alertas y comunicación con servicios externos como Telegram.

## Arquitectura del Sistema

El ecosistema **VerdeVital** está compuesto por los siguientes módulos:

1. **PF-Sensores:** Captura datos ambientales (temperatura, humedad, luz, CO₂, etc.) y los envía a PF-Edge mediante ESP-NOW.
2. **PF-Actuadores:** Recibe comandos de PF-Edge para controlar dispositivos como bombas, ventiladores y LEDs.
3. **PF-Edge:** Centraliza la información, procesa los datos, genera alertas y controla la interacción con los usuarios.

## Funcionalidades Principales

- **Recepción de datos de sensores:** Utiliza ESP-NOW para recibir datos de múltiples sensores ambientales conectados al módulo **PF-Sensores**.
- **Control de actuadores:** Envía comandos a **PF-Actuadores** para activar o desactivar dispositivos según los umbrales configurados.
- **Gestión de umbrales y alertas:** Evalúa los datos de los sensores en función de umbrales definidos, activa alertas y genera mensajes de estado.
- **Registro de datos:** Almacena los datos de los sensores en una tarjeta SD en formato CSV para análisis histórico.
- **Interacción con Telegram:** Permite enviar notificaciones y recibir comandos a través de un bot de Telegram.
- **Visualización en pantalla OLED:** Muestra información en tiempo real, como datos de sensores, estado de actuadores y alertas.

## Tecnologías y Plataforma

- **Microcontrolador:** ESP32
- **Lenguaje:** C++
- **Protocolos de comunicación:** ESP-NOW, WiFi
- **Almacenamiento:** Tarjeta SD
- **Interfaz de usuario:** Pantalla OLED y Telegram

## Estructura del Proyecto

El proyecto está organizado en los siguientes componentes clave:

- `DisplayManager.h`: Maneja la visualización de datos en la pantalla OLED.
- `ESPNowReceiver.h`: Gestiona la recepción de datos mediante ESP-NOW.
- `ESPNowSender.h`: Gestiona el envío de comandos a los actuadores mediante ESP-NOW.
- `ThresholdsController.h`: Evalúa los datos de sensores y controla los actuadores según los umbrales configurados.
- `SDLogger.h`: Registra los datos de los sensores en una tarjeta SD.
- `TelegramBot.h`: Interactúa con la API de Telegram para enviar y recibir mensajes.
- `WifiConnector.h`: Gestiona la conexión WiFi del dispositivo.

## Configuración del Hardware

### Conexiones Principales

- **Pantalla OLED:**  
    - Dirección I2C: `0x3C`
    - Pines: SDA y SCL conectados al ESP32.
- **Tarjeta SD:**  
    - Pin CS: `5`
- **Módulo RTC DS1302:**  
    - Pines: IO, SCLK, CE conectados al ESP32.

### Requisitos de Hardware

- ESP32
- Pantalla OLED (128x64)
- Tarjeta SD y módulo lector
- RTC DS1302
- Sensores ambientales (conectados a PF-Sensores)
- Actuadores (conectados a PF-Actuadores)

## Instalación y Configuración

1. **Clonar el repositorio:**
     ```bash
     git clone https://github.com/usuario/VerdeVital-PF-Edge.git
     ```
2. **Configurar el entorno de desarrollo:**
     - Instalar PlatformIO o configurar el soporte para ESP32 en el IDE de Arduino.
     - Instalar las bibliotecas necesarias:
         - Adafruit_GFX
         - Adafruit_SSD1306
         - RtcDS1302
         - SD
     - Configurar credenciales WiFi en el archivo correspondiente.
3. **Compilar y subir el código:**
     - Conectar el ESP32 al ordenador.
     - Compilar y subir el firmware al ESP32.
4. **Configurar el bot de Telegram:**
     - Crear un bot en Telegram utilizando BotFather.
     - Obtener el token del bot y configurarlo en el archivo `TelegramBot.h`.

## Uso del Sistema

- **Inicio del sistema:**  
    Al encender el ESP32, se conectará a la red WiFi configurada y comenzará a recibir datos de los sensores.
- **Visualización de datos:**  
    Los datos de los sensores y el estado del sistema se mostrarán en la pantalla OLED.
- **Control de actuadores:**  
    Los actuadores se activarán o desactivarán automáticamente según los umbrales configurados.
- **Interacción con Telegram:**  
    Se pueden enviar comandos al bot para consultar el estado del sistema o modificar configuraciones.
- **Registro de datos:**  
    Los datos se almacenarán automáticamente en la tarjeta SD en formato CSV.

## Integración con el Ecosistema VerdeVital

- **PF-Sensores:** Recibe datos de sensores ambientales mediante ESP-NOW.
- **PF-Actuadores:** Envía comandos para controlar dispositivos como bombas, ventiladores y LEDs.

## Contribuciones

Las contribuciones son bienvenidas. Si deseas proponer mejoras o reportar problemas, abre un issue o envía un pull request.

## Licencia

Este proyecto se distribuye bajo la Licencia MIT.
