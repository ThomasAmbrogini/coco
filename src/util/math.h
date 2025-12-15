#pragma once

#include "type_traits.h"

template<typename T, typename U = int, typename = enable_if_t<is_floating_point<T>::value>>
constexpr U round(T num) {
    return static_cast<U>(num + 0.5);
}

template<typename T, typename U>
constexpr int round_to_closest_int(T num, U div) {
    if ( (((U) -1) > 0) || (((U) -1) > 0) || ((num > 0) == (div > 0)) ) {
        return ((num) + (div / 2)) / div;
    } else {
        return ((num) - (div / 2)) / div;
    }
}

template<typename T>
constexpr T abs(T value) {
    return (value >= 0) ? value : -value;
}

