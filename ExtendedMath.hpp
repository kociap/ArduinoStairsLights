#ifndef EXTENDED_MATH_HPP
#define EXTENDED_MATH_HPP

template<typename T>
constexpr bool isEven(T number) {
    return (number & 1) == 0;
}

template<typename T>
constexpr bool isOdd(T number) {
    return (number & 1) == 1;
}

template<typename T>
T abs(T number) {
    return (number >= 0 ? number : -number);
}

#endif // !EXTENDED_MATH_HPP

