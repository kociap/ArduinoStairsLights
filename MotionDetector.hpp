#ifndef MOTION_DETECTOR_HPP
#define MOTION_DETECTOR_HPP

#include <Arduino.h>
#include "BasicTypes.hpp"

namespace MotionDetectors {
    static const pin_index_t FIRST_DETECTOR = 1;
    static const pin_index_t SECOND_DETECTOR = 2;

    static const int INPUT_DEAD_ZONE = 400;

    bool readInput(pin_index_t pin) {
        return analogRead(pin) > INPUT_DEAD_ZONE;
    }
}

#endif // !MOTION_DETECTOR_HPP

