#ifndef LIGHTS_MANAGER_HPP
#define LIGHTS_MANAGER_HPP
using namespace std;

#include "Lamp.hpp"
#include "Lamps.hpp"
#include "Time.hpp"
#include <map>
#include <deque>

/* TO DO
 *   Only one change per lamp (running multiple will interfere)
 */

extern Time* timeManager;

constexpr Lamp::brightness_t brightnessLerp(Lamp::brightness_t minBrightness, Lamp::brightness_t maxBrightness, double step) {
    return minBrightness + (maxBrightness - minBrightness) * step;
}

class LightsManager {
private:
    class BrightnessChange {
    public:
        Lamp::brightness_t to;
        Lamp::brightness_t from;
        double progress;
        Lamp* lamp;

        BrightnessChange(Lamp* l, Lamp::brightness_t target): lamp(l), to(target), from(), progress(0) {
            from = lamp->getCurrentBrightness();
            if (to > Lamp::MAX_BRIGHTNESS) {
                to = Lamp::MAX_BRIGHTNESS;
            } else if (to < Lamp::MIN_BRIGHTNESS) {
                to = Lamp::MIN_BRIGHTNESS;
            }
        }
    };

    std::map<int, int> _mapper;
    std::deque<BrightnessChange> _toUpdate;
    Lamp** _lamps;
    int _maxNumberOfLamps;
    int _numberOfLamps;
    int _lighteningUpTime;

public: 
    LightsManager(int numberOfLamps): _numberOfLamps(0), _maxNumberOfLamps(numberOfLamps), _lamps(new Lamp*[numberOfLamps]) {};

    ~LightsManager() {
        for (int i = 0; i < _numberOfLamps; ++i) {
            delete _lamps[i];
        }

        delete _lamps;
    }

    // Assumes ownership over the object
    void addLamp(Lamp* lamp) {
        _lamps[_numberOfLamps] = lamp;
        _mapper[lamp->getIndex()] = _numberOfLamps;
        ++_numberOfLamps;
    }

    Lamp* getLamp(int lampIndex) {
        return _lamps[_mapper[lampIndex]];
    }

    // Returns a bool indicating whether the lamp has been added to the update list
    bool addToUpdate(Lamp* lamp, Lamp::brightness_t targetBrightness) {
        if (lamp->getCurrentBrightness() == targetBrightness) {
            return false;
        }
        
        _toUpdate.push_back(BrightnessChange(lamp, targetBrightness));
        return true;
    }

    void update() {
        for (deque<BrightnessChange>::iterator i = _toUpdate.begin(); i != _toUpdate.end();) {
            i->progress += static_cast<double>(timeManager->getTimeDelta()) / static_cast<double>(i->lamp->getBrightnessChangeTime());
            Lamp::brightness_t newBrightness = brightnessLerp(i->from, i->to, i->progress);
            i->lamp->changeBrightness(newBrightness);

            if (i->lamp->getCurrentBrightness() == i->to) {
                i = _toUpdate.erase(i);
            } else {
                ++i;
            }
        }
    }
};

#endif // !LIGHTS_MANAGER_HPP
