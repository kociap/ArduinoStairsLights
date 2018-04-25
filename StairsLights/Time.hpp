#ifndef TIME_HPP
#define TIME_HPP

using time_t = long long;
using time_delta_t = long long;

class Time {
private:
    time_t _time = 0;
    time_delta_t _timeDelta = 0;

public:
    time_t getTime() {
        return _time;
    }

    time_delta_t getTimeDelta() {
        return _timeDelta;
    }

    void updateTimeByDelta(time_delta_t timeDelta) {
        _timeDelta = timeDelta;
        _time += timeDelta;
    }
};

class Timer {
    time_t _time = 0;
    time_t _timestamp = 0;

public:
    time_t getTime() {
        return _time;
    }

    void updateTimeByDelta(time_delta_t timeDelta) {
        _time += timeDelta;
    }

    void reset() {
        _time = 0;
        _timestamp = 0;
    }

    void setTime(time_t time) {
        _time = time;
    }

    void setTimestamp(time_t time) {
        _timestamp = time;
    }

    bool timestampHit() {
        return _time >= _timestamp;
    }

    time_t getTimestamp() {
        return _timestamp;
    }
};

#endif // !TIME_HPP
