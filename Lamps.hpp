#ifndef LAMPS_HPP
#define LAMPS_HPP

#include "BasicTypes.hpp"

namespace Lamps {

    //pin_index_t FIRST = 2;
    //pin_index_t SECOND = 3;
    //pin_index_t THIRD = 4;
    //pin_index_t FOURTH = 5;
    //pin_index_t FIFTH = 6;
    //pin_index_t SIXTH = 7;
    //pin_index_t SEVENTH = 8;
    //pin_index_t EIGHTH = 9;
    //pin_index_t NINETH = 10;
    //pin_index_t TENTH = 11;
    //pin_index_t ELEVENTH = 12;
    //pin_index_t TWELFTH = 13;
    //pin_index_t THIRTEENTH = 44;
    //pin_index_t FOURTEENTH = 45;
    //pin_index_t FIFTEENTH = 46;
    pin_index_t LAMP_1 = 2;
    pin_index_t LAMP_2 = 3;
    pin_index_t LAMP_3 = 4;
    pin_index_t LAMP_4 = 5;
    pin_index_t LAMP_5 = 6;
    pin_index_t LAMP_6 = 7;
    pin_index_t LAMP_7 = 8;
    pin_index_t LAMP_8 = 9;
    pin_index_t LAMP_9 = 10;
    pin_index_t LAMP_10 = 11;
    pin_index_t LAMP_11 = 12;
    pin_index_t LAMP_12 = 13;
    pin_index_t LAMP_13 = 44;
    pin_index_t LAMP_14 = 45;
    pin_index_t LAMP_15 = 46;

    pin_index_t NUMBER_OF_LAMPS = 15;

    pin_index_t get(int index) {
        pin_index_t lamps[] = {
            2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 44, 45, 46
        };

        return lamps[index];
    }
};

#endif // !LAMPS_HPP
