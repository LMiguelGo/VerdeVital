/**
 * @file RtcDS1302Helper.h
 * @brief Clase auxiliar para manejar el RTC DS1302.
 */

#ifndef RTC_DS1302_HELPER_H
#define RTC_DS1302_HELPER_H

// Librerías
#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

/**
 * @class RtcDS1302Helper
 * @brief Clase para interactuar con el módulo RTC DS1302.
 * 
 * Esta clase proporciona métodos para inicializar, configurar y obtener datos del reloj en tiempo real DS1302.
 */
class RtcDS1302Helper {
public:
    /**
     * @brief Constructor de la clase RtcDS1302Helper.
     * @param ioPin Pin de datos I/O del DS1302.
     * @param sclkPin Pin de reloj (SCLK) del DS1302.
     * @param cePin Pin de habilitación (CE) del DS1302.
     */
    RtcDS1302Helper(uint8_t ioPin, uint8_t sclkPin, uint8_t cePin)
        : _wire(ioPin, sclkPin, cePin), _rtc(_wire) {}

    /**
     * @brief Inicializa el módulo RTC DS1302.
     * 
     * Configura el módulo, desactiva la protección de escritura, inicia el reloj si no está corriendo
     * y actualiza la fecha/hora desde el tiempo de compilación si es necesario.
     */
    void begin() {
        _rtc.Begin();

        if (_rtc.GetIsWriteProtected()) {
            _rtc.SetIsWriteProtected(false);
        }

        if (!_rtc.GetIsRunning()) {
            _rtc.SetIsRunning(true);
        }

        if (!_rtc.IsDateTimeValid()) {
            updateFromCompileTime();
        }
    }

    /**
     * @brief Obtiene el año actual del RTC.
     * @return Año actual.
     */
    int getYear() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Year();
    }

    /**
     * @brief Obtiene el mes actual del RTC.
     * @return Mes actual.
     */
    int getMonth() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Month();
    }

    /**
     * @brief Obtiene el día actual del RTC.
     * @return Día actual.
     */
    int getDay() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Day();
    }

    /**
     * @brief Obtiene la hora actual del RTC.
     * @return Hora actual.
     */
    int getHour() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Hour();
    }

    /**
     * @brief Obtiene el minuto actual del RTC.
     * @return Minuto actual.
     */
    int getMinute() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Minute();
    }

    /**
     * @brief Obtiene el segundo actual del RTC.
     * @return Segundo actual.
     */
    int getSecond() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Second();
    }

    /**
     * @brief Actualiza la fecha y hora del RTC con la fecha y hora de compilación.
     * 
     * Si la fecha/hora actual del RTC no es válida o es anterior a la fecha/hora de compilación,
     * se actualiza con esta última.
     */
    void updateFromCompileTime() {
        RtcDateTime compiled(__DATE__, __TIME__);
        RtcDateTime now = _rtc.GetDateTime();

        if (!now.IsValid() || now < compiled) {
            _rtc.SetDateTime(compiled);
        }
    }

    /**
     * @brief Actualiza la fecha y hora del RTC con valores proporcionados.
     * @param year Año a establecer.
     * @param month Mes a establecer.
     * @param day Día a establecer.
     * @param hour Hora a establecer.
     * @param minute Minuto a establecer.
     * @param second Segundo a establecer.
     */
    void updateFromNTP(int year, int month, int day, int hour, int minute, int second) {
        RtcDateTime dt(year, month, day, hour, minute, second);
        _rtc.SetDateTime(dt);
    }

    /**
     * @brief Obtiene una marca de tiempo completa (fecha y hora) del RTC.
     * @return Cadena con la marca de tiempo en formato "YYYY-MM-DD HH:MM:SS".
     */
    String getTimestamp() {
        RtcDateTime now = _rtc.GetDateTime();
        return getDate() + " " + getTime();
    }

    /**
     * @brief Obtiene la fecha actual del RTC.
     * @return Cadena con la fecha en formato "YYYY-MM-DD".
     */
    String getDate() {
        RtcDateTime now = _rtc.GetDateTime();
        return String(now.Year()) + "-" + padZero(now.Month()) + "-" + padZero(now.Day());
    }

    /**
     * @brief Obtiene la hora actual del RTC.
     * @return Cadena con la hora en formato "HH:MM:SS".
     */
    String getTime() {
        RtcDateTime now = _rtc.GetDateTime();
        return padZero(now.Hour()) + ":" + padZero(now.Minute()) + ":" + padZero(now.Second());
    }

private:
    ThreeWire _wire; ///< Objeto para manejar la comunicación de tres cables con el DS1302.
    RtcDS1302<ThreeWire> _rtc; ///< Objeto para interactuar con el módulo RTC DS1302.

    /**
     * @brief Agrega un cero a la izquierda si el número es menor a 10.
     * @param number Número a formatear.
     * @return Cadena con el número formateado.
     */
    String padZero(int number) {
        return (number < 10 ? "0" : "") + String(number);
    }
};

#endif // RTC_DS1302_HELPER_H
