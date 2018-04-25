#include <StandardCplusplus.h>
#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>

#include "Time.hpp"
#include "Lamps.hpp"
#include "LightsManager.hpp"
#include "ExtendedMath.hpp"
#include "DoubleEndedSequence.hpp"
#include "MotionDetector.hpp"

const time_t MIN_WAIT_TIME = 400; // milliseconds
const time_t DEFAULT_TURNING_ON_TIME = 200; // milliseconds

Time* timeManager = new Time();
LightsManager* lightsManager = new LightsManager(Lamps::NUMBER_OF_LAMPS);

class LampBrightnessChange {
public:
    pin_index_t pinIndex;
    Lamp::brightness_t brightness;

    LampBrightnessChange(pin_index_t _pinIndex, Lamp::brightness_t _brightness): pinIndex(_pinIndex), brightness(_brightness) {}
};

using LampsSequence = DoubleEndedSequence<LampBrightnessChange>;
using LampsSequenceStep = LampsSequence::SequenceStep;
using SequenceIterator = LampsSequence::iterator;

LampsSequence leftSequence;
LampsSequence rightSequence;
LampsSequence offSequence;

Timer leftSequenceTimer;
Timer rightSequenceTimer;
Timer offSequenceTimer;
Timer waitTimer;

bool runLeftSequence = false;
bool runRightSequence = false;
bool runOffSequence = false;
bool turnedOn = false;
bool doneSequences = false;

SequenceIterator rightSequenceIter;
SequenceIterator leftSequenceIter;
SequenceIterator offSequenceIter;

void setup() {
    Serial.begin(115200);

    lightsManager->addLamp(new Lamp(Lamps::LAMP_1));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_2));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_3));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_4));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_5));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_6));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_7));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_8));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_9));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_10));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_11));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_12));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_13));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_14));
    lightsManager->addLamp(new Lamp(Lamps::LAMP_15));

    // Create sequences
    for (int i = 0; i < Lamps::NUMBER_OF_LAMPS; ++i) {
        LampsSequenceStep left(10 * i, {
            LampBrightnessChange(Lamps::get(i), Lamp::MAX_BRIGHTNESS)
        });
        LampsSequenceStep right(10 * i, {
            LampBrightnessChange(Lamps::get(Lamps::NUMBER_OF_LAMPS - i - 1), Lamp::MAX_BRIGHTNESS)
        });

        leftSequence.addStepBack(left);
        rightSequence.addStepBack(right);
    }

    if (isOdd(Lamps::NUMBER_OF_LAMPS)) {
        LampsSequenceStep off(0, {
            LampBrightnessChange(Lamps::LAMP_8, Lamp::MIN_BRIGHTNESS)
        });

        offSequence.addStepBack(off);
    }

    for (int i = Lamps::NUMBER_OF_LAMPS / 2 - 1, j = Lamps::NUMBER_OF_LAMPS / 2 + (isEven(Lamps::NUMBER_OF_LAMPS) ? 0 : 1); i >= 0; --i, ++j) {
        LampsSequenceStep off(10 * i, {
            LampBrightnessChange(Lamps::get(i), Lamp::MIN_BRIGHTNESS),
            LampBrightnessChange(Lamps::get(j), Lamp::MIN_BRIGHTNESS)
        });

        offSequence.addStepBack(off);
    }

    leftSequenceIter = leftSequence.begin();
    rightSequenceIter = rightSequence.begin();
    offSequenceIter = offSequence.begin();
}

void loop() {
    delay(1);
    timeManager->updateTimeByDelta(1);

    // Read left motion detector input
    if (MotionDetectors::readInput(MotionDetectors::FIRST_DETECTOR) && !turnedOn && !runOffSequence) {
        runLeftSequence = true;
    }

    // Read right motion detector input
    if (MotionDetectors::readInput(MotionDetectors::SECOND_DETECTOR) && !turnedOn && !runOffSequence) {
        runRightSequence = true;
    }

    if (turnedOn) {
        waitTimer.updateTimeByDelta(timeManager->getTimeDelta());
        if (waitTimer.getTime() == MIN_WAIT_TIME) {
            runOffSequence = true;
            waitTimer.reset();
        }
    }

    auto updateLights = [](Timer& timer, SequenceIterator& sequenceIter, const SequenceIterator& endIterator, void(*done)()) -> void {
        timer.updateTimeByDelta(timeManager->getTimeDelta());
        if (sequenceIter != endIterator && sequenceIter->getStartTime() <= timer.getTime()) {
            bool anyLampAdded = false;
            while (!anyLampAdded && sequenceIter != endIterator) {
                for (size_t i = 0; i < sequenceIter->size(); ++i) {
                    auto lamp = lightsManager->getLamp(sequenceIter->get(i).pinIndex);
                    bool lampAdded = lightsManager->addToUpdate(lamp, sequenceIter->get(i).brightness);
                    if (lampAdded) {
                        anyLampAdded = true;
                        auto brightnessChangeTime = lamp->getBrightnessChangeTime(sequenceIter->get(i).brightness);
                        auto newTimestamp = max(timer.getTime() + brightnessChangeTime, timer.getTimestamp());
                        timer.setTimestamp(newTimestamp);
                    }
                }

                if (anyLampAdded) {
                    timer.setTime(sequenceIter->getStartTime());
                    ++sequenceIter;
                } else {
                    timer.setTimestamp(timer.getTime());
                }
            }
        }

        // Timestamp hit does not occur unless sequence is complete
        if (timer.timestampHit()) {
            done();
        }
    };

    auto leftSequenceCallback = []() -> void {
        turnedOn = true;
        runLeftSequence = false;
        leftSequenceIter = leftSequence.begin();
        leftSequenceTimer.reset();
    };

    auto rightSequenceCallback = []() -> void {
        turnedOn = true;
        runRightSequence = false;
        rightSequenceIter = rightSequence.begin();
        rightSequenceTimer.reset();
    };

    auto offSequenceCallback = []() -> void {
        turnedOn = false;
        runOffSequence = false;
        offSequenceIter = offSequence.begin();
        offSequenceTimer.reset();
    };

    if (runLeftSequence) {
        updateLights(leftSequenceTimer, leftSequenceIter, leftSequence.end(), leftSequenceCallback);
    }

    if (runRightSequence) {
        updateLights(rightSequenceTimer, rightSequenceIter, rightSequence.end(), rightSequenceCallback);
    }

    if (runOffSequence) {
        updateLights(offSequenceTimer, offSequenceIter, offSequence.end(), offSequenceCallback);
    }

    //Serial.println(static_cast<int>(timeManager->getTime())); // Debug logging

    lightsManager->update();
}