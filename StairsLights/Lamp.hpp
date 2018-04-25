#ifndef LAMP_HPP
#define LAMP_HPP

#include <Arduino.h>
#include "Time.hpp"
#include "ExtendedMath.hpp"

extern const time_t DEFAULT_TURNING_ON_TIME;

class Lamp {
public:
    using brightness_t = double;

    const static int MAX_BRIGHTNESS = 255;
    const static int MIN_BRIGHTNESS = 0;

    enum State {
        TURNED_ON, TURNED_OFF, INTERMEDIATE
    };

private:
    int _pinIndex;
    double _currentBrightness;
    long long _turningOnTime;

public:
    Lamp(int pinIndex, time_t turningOnTime = DEFAULT_TURNING_ON_TIME): _currentBrightness(0), _pinIndex(pinIndex), _turningOnTime(turningOnTime) {}
    Lamp(int pinIndex, brightness_t brightness, time_t turningOnTime): _currentBrightness(brightness), _pinIndex(pinIndex), _turningOnTime(turningOnTime) {
        analogWrite(_pinIndex, _currentBrightness);
    }

    time_t getBrightnessChangeTime() {
        return _turningOnTime;
    }

    time_t getBrightnessChangeTime(brightness_t brightness) {
        return static_cast<time_t>(_turningOnTime * (abs(_currentBrightness - brightness) / static_cast<double>(MAX_BRIGHTNESS)));
    }

    brightness_t getCurrentBrightness() {
        return _currentBrightness;
    }

    int getIndex() {
        return _pinIndex;
    }

    void changeBrightness(brightness_t brightness) {
        if (brightness > Lamp::MAX_BRIGHTNESS) {
            brightness = Lamp::MAX_BRIGHTNESS;
        } else if (brightness < Lamp::MIN_BRIGHTNESS) {
            brightness = Lamp::MIN_BRIGHTNESS;
        }

        _currentBrightness = brightness;
        analogWrite(_pinIndex, _currentBrightness);
    }
};

#endif // !LAMP_HPP
