#ifndef LED_RGB_H
#define LED_RGB_H

#include <Arduino.h>

class LedRGB {
  private:
    int pin;

  public:
    LedRGB(int pin) {
      this->pin = pin;
    }

    void begin() {
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
    }

    void on() {
      digitalWrite(pin, HIGH);
    }

    void off() {
      digitalWrite(pin, LOW);
    }
};

#endif
