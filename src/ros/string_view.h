#pragma once

#include "assert.h"
#include "view.h"

#include <stddef.h>

consteval int consteval_strlen(const char* Str) {
    int Idx {0};

    while(Str[Idx] != '\0') {
        ++Idx;
    }

    return Idx;
}

namespace ros {

class string_view : public view<char> {
public:
    consteval string_view() = default;

    consteval string_view(const_pointer Data) noexcept {
        int Size {consteval_strlen(Data)};
        Data_ = Data;
        Size_ = Size;
    }

    constexpr string_view(const_pointer Data, size_type Size) : view<char>(Data, Size) {
    }
};

} /* namespace ros */

inline constexpr ros::string_view operator""_sv(const char* Str, size_t Len) noexcept {
    return ros::string_view(Str, Len);
}

