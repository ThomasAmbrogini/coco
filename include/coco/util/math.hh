#pragma once

#include "type_traits.hh"

template<typename T, typename U = int, typename = enable_if_t<is_floating_point<T>::value>>
constexpr U round(T Num) {
    return static_cast<U>(Num + 0.5);
}

template<typename T, typename U>
constexpr int round_to_closest_int(T Num, U Div) {
    if ( (((U) -1) > 0) || (((U) -1) > 0) || ((Num > 0) == (Div > 0)) ) {
        return ((Num) + (Div / 2)) / Div;
    } else {
        return ((Num) - (Div / 2)) / Div;
    }
}

template<typename T>
constexpr T abs(T Value) {
    return (Value >= 0) ? Value : -Value;
}

