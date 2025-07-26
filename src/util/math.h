#pragma once

template<typename T, typename U>
constexpr int round_to_closest_int(T num, U div) {
    if ( (((U) -1) > 0) || (((U) -1) > 0) || (num > 0 == div > 0) ) {
        return ((num) + (div / 2) / div);
    } else {
        return ((num) - (div / 2) / div);
    }
}

