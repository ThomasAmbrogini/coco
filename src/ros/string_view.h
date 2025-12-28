#pragma once

#include "assert.h"
#include "view.h"

#include <stddef.h>

consteval int consteval_strlen(const char* str) {
    int idx {0};

    while(str[idx] != '\0') {
        ++idx;
    }

    return idx;
}

namespace ros {

class StringView : public View<char> {
public:
    consteval StringView() = default;

    consteval StringView(ConstPointer data) noexcept {
        int size {consteval_strlen(data)};
        __data_ = data;
        __size_ = size;
    }

    constexpr StringView(ConstPointer data, SizeType size) : View<char>(data, size) {
    }
};

} /* namespace ros */

inline constexpr ros::StringView operator""_sv(const char* str, size_t len) noexcept {
    return ros::StringView(str, len);
}

