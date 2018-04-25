#ifndef DOUBLE_ENDED_SEQUENCE_HPP
#define DOUBLE_ENDED_SEQUENCE_HPP

#include <deque>

template<typename T>
class DoubleEndedSequence {
public:
    class SequenceStep;
    //class iterator;
    using iterator = typename std::deque<SequenceStep>::iterator;

private:
    std::deque<SequenceStep> _steps;

public:
    class SequenceStep {
        std::deque<T> _elements;
        time_t _startTime;

    public:
        SequenceStep(time_t startTime, std::initializer_list<T>&& list): _elements(list), _startTime(startTime) {}

        size_t size() {
            return _elements.size();
        }

        T& operator[](size_t index) {
            return _elements[index];
        }

        T& get(size_t index) {
            return _elements[index];
        }

        time_t getStartTime() {
            return _startTime;
        }
    };

    iterator begin();
    iterator end();

    /*class iterator {
        friend iterator DoubleEndedSequence<T>::begin();
        friend iterator DoubleEndedSequence<T>::end();

        typename std::deque<SequenceStep>::iterator _itr;

        iterator(const typename std::deque<SequenceStep>::iterator& i): _itr(i) {}

    public:
        SequenceStep* operator->() {
            return &(*_itr);
        }

        SequenceStep& operator*() {
            return *_itr;
        }

        bool operator==(const iterator& i) {
            return i._itr == _itr;
        }

        bool operator!=(const iterator& i) {
            return _itr != i._itr;
        }

        iterator& operator++() {
            ++_itr;
            return *this;
        }

        iterator& operator--() {
            --_itr;
            return *this;
        }

        iterator operator-(int offset) {
            return _itr - offset;
        }

        iterator operator+(int offset) {
            return _itr + offset;
        }
    };*/

    void addStepFront(const SequenceStep& step) {
        step.setStartTime(startTime);
        _steps.push_front(step);
    }
    
    void addStepFront(SequenceStep&& step) {
        _steps.push_front(step);
    }
    
    void addStepBack(const SequenceStep& step) {
        _steps.push_back(step);
    }
   
    void addStepBack(SequenceStep&& step) {
        _steps.push_back(step);
    }
};

template<typename T>
typename DoubleEndedSequence<T>::iterator DoubleEndedSequence<T>::begin() {
    return _steps.begin();
}

template<typename T>
typename DoubleEndedSequence<T>::iterator DoubleEndedSequence<T>::end() {
    return _steps.end();
}

#endif // !DOUBLE_ENDED_SEQUENCE_HPP