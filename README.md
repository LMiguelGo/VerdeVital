# VerdeVital - Sistema Integral de Monitoreo y Control Ambiental

**VerdeVital** es un ecosistema modular diseñado para la gestión inteligente de invernaderos y entornos agrícolas. Este sistema combina tecnologías avanzadas de adquisición de datos, procesamiento, comunicación inalámbrica y control de actuadores para ofrecer una solución integral de monitoreo y automatización ambiental.

## Arquitectura del Sistema

El ecosistema **VerdeVital** está compuesto por tres módulos principales que trabajan de manera coordinada:

1. **PF-Sensores:** Captura datos ambientales (temperatura, humedad, luz, CO2, humedad del suelo, voltaje) y los transmite al módulo central mediante ESP-NOW.
2. **PF-Edge:** Actúa como el núcleo del sistema, procesando los datos recibidos, evaluando umbrales, generando alertas, registrando información y enviando comandos a los actuadores.
3. **PF-Actuadores:** Recibe comandos del módulo central y controla dispositivos como bombas de agua, ventiladores y LEDs RGB.

## Características Principales

### PF-Sensores
- **Adquisición de Datos:**  
    Captura información de sensores como:
    - **DHT11:** Temperatura y humedad.
    - **LDR:** Intensidad lumínica.
    - **MQ135:** Calidad del aire (CO2 en ppm).
    - **YL-69:** Humedad del suelo.
    - **Divisor de Voltaje:** Voltaje de entrada.
- **Comunicación Inalámbrica:**  
    Transmite los datos al módulo **PF-Edge** mediante ESP-NOW.
- **Multitarea con FreeRTOS:**  
    Ejecuta tareas concurrentes para la lectura de sensores y envío de datos.

### PF-Edge
- **Procesamiento Centralizado:**  
    Recibe datos de **PF-Sensores**, evalúa umbrales configurados y genera alertas.
- **Control de Actuadores:**  
    Envía comandos a **PF-Actuadores** para activar o desactivar dispositivos según las condiciones ambientales.
- **Registro de Datos:**  
    Almacena información en una tarjeta SD en formato CSV para análisis histórico.
- **Interacción con Telegram:**  
    Permite a los usuarios consultar el estado del sistema y controlar actuadores mediante un bot de Telegram.
- **Visualización en Pantalla OLED:**  
    Muestra datos en tiempo real, alertas y estado del sistema.

### PF-Actuadores
- **Control de Dispositivos:**  
    Maneja actuadores como:
    - Bomba de agua.
    - Ventilador.
    - LEDs RGB.
- **Recepción de Comandos:**  
    Escucha las instrucciones enviadas por **PF-Edge** mediante ESP-NOW.
- **Configuración Modular:**  
    Permite la expansión para incluir más actuadores según las necesidades del sistema.

## Tecnologías Utilizadas

- **Microcontrolador:** ESP32.
- **Protocolos de Comunicación:** ESP-NOW, WiFi.
- **Sistema Operativo en Tiempo Real:** FreeRTOS.
- **Almacenamiento:** Tarjeta SD.
- **Interfaz de Usuario:** Pantalla OLED y bot de Telegram.
- **Lenguaje de Programación:** C/C++.

## Configuración del Hardware

### PF-Sensores
- **DHT11:** Pin `4`.
- **LDR:** Pin `34`.
- **MQ135:** Pin `35`.
- **YL-69:** Pin `32`.
- **Divisor de Voltaje:** Pin `33`.

### PF-Edge
- **Pantalla OLED:** Pines SDA y SCL conectados al ESP32.
- **Tarjeta SD:** Pin CS `5`.
- **RTC DS1302:** Pines IO, SCLK, CE conectados al ESP32.

### PF-Actuadores
- **Bomba de Agua:** Pin `25`.
- **Ventilador:** Pin `26`.
- **LEDs RGB:** Pines `14`, `27`, `33`, `32`.

## Instalación y Configuración

### Requisitos Previos
1. **Hardware Necesario:**
     - ESP32.
     - Sensores (DHT11, LDR, MQ135, YL-69, divisor de voltaje).
     - Actuadores (bomba de agua, ventilador, LEDs RGB).
     - Pantalla OLED, tarjeta SD y módulo RTC DS1302.
2. **Software Necesario:**
     - [PlatformIO](https://platformio.org/) o Arduino IDE con soporte para ESP32.
     - Librerías requeridas:
         - Adafruit_GFX
         - Adafruit_SSD1306
         - RtcDS1302
         - SD

### Pasos de Configuración
1. **Clonar el Repositorio:**
     ```bash
     git clone https://github.com/usuario/VerdeVital.git
     ```
2. **Configurar el Entorno de Desarrollo:**
     - Instalar las dependencias necesarias.
     - Configurar credenciales WiFi y token del bot de Telegram en los archivos correspondientes.
3. **Conectar el Hardware:**
     - Realizar las conexiones físicas según los pines especificados.
4. **Compilar y Subir el Código:**
     - Compilar el firmware de cada módulo y subirlo al ESP32 correspondiente.
5. **Verificar la Comunicación:**
     - Asegurarse de que los canales ESP-NOW coincidan entre los módulos.

## Uso del Sistema

- **Inicio del Sistema:**  
    Al encender los módulos, PF-Sensores comenzará a capturar datos y enviarlos a PF-Edge. PF-Edge procesará los datos, evaluará umbrales y enviará comandos a PF-Actuadores.
- **Interacción con Telegram:**  
    Consultar datos y estado del sistema mediante comandos como `/datos`, `/estado`, `/umbrales`, etc.
- **Visualización en Pantalla OLED:**  
    Monitorear datos en tiempo real y alertas directamente en la pantalla OLED.
- **Registro de Datos:**  
    Los datos se almacenarán automáticamente en la tarjeta SD para análisis posterior.

## Integración del Ecosistema

- **PF-Sensores:** Captura datos ambientales y los transmite a PF-Edge.
- **PF-Edge:** Procesa los datos, genera alertas y controla actuadores.
- **PF-Actuadores:** Ejecuta las acciones necesarias para mantener las condiciones óptimas.

## Próximos Pasos y Mejoras Futuras

- Ampliar la compatibilidad con más sensores y actuadores.
- Implementar control de LEDs RGB con variaciones de color y brillo.
- Integrar servicios en la nube para análisis avanzado de datos.
- Desarrollar una interfaz web para monitoreo y control remoto.

## Contribuciones

Las contribuciones son bienvenidas. Si deseas proponer mejoras, reportar problemas o colaborar en el desarrollo, abre un issue o envía un pull request en el repositorio oficial.

## Licencia

Este proyecto se distribuye bajo la Licencia MIT.

Desarrollado con pasión por el equipo de VerdeVital.