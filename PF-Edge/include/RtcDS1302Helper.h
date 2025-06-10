#ifndef RTC_DS1302_HELPER_H
#define RTC_DS1302_HELPER_H

#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

class RtcDS1302Helper {
public:
    RtcDS1302Helper(uint8_t ioPin, uint8_t sclkPin, uint8_t cePin)
        : _wire(ioPin, sclkPin, cePin), _rtc(_wire) {}

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

    // Functions to get separate values
    int getYear() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Year();
    }

    int getMonth() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Month();
    }

    int getDay() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Day();
    }

    int getHour() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Hour();
    }

    int getMinute() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Minute();
    }

    int getSecond() {
        RtcDateTime now = _rtc.GetDateTime();
        return now.Second();
    }

    void updateFromCompileTime() {
        RtcDateTime compiled(__DATE__, __TIME__);
        RtcDateTime now = _rtc.GetDateTime();

        if (!now.IsValid() || now < compiled) {
            _rtc.SetDateTime(compiled);
        }
    }

    void updateFromNTP(int year, int month, int day, int hour, int minute, int second) {
        RtcDateTime dt(year, month, day, hour, minute, second);
        _rtc.SetDateTime(dt);
    }

    String getTimestamp() {
        RtcDateTime now = _rtc.GetDateTime();
        return getDate() + " " + getTime();
    }

    String getDate() {
        RtcDateTime now = _rtc.GetDateTime();
        return String(now.Year()) + "-" + padZero(now.Month()) + "-" + padZero(now.Day());
    }

    String getTime() {
        RtcDateTime now = _rtc.GetDateTime();
        return padZero(now.Hour()) + ":" + padZero(now.Minute()) + ":" + padZero(now.Second());
    }

private:
    ThreeWire _wire;
    RtcDS1302<ThreeWire> _rtc;

    String padZero(int number) {
        return (number < 10 ? "0" : "") + String(number);
    }
};

#endif
