#pragma once

#include "assert.h"

#include <stddef.h>

namespace ros {

template<typename _T>
class View {
public:
    using ValueType    = _T;
    using Pointer      = ValueType*;
    using ConstPointer = const ValueType*;
    using SizeType     = int;

    constexpr View(ConstPointer data, SizeType size) noexcept : __data_(data),
                                                                __size_(size) {
        assert(__data_ != nullptr);
        assert(__size_ > 0);
    }

    constexpr SizeType size() const noexcept {
        return __size_;
    }

    //TODO: maybe i never need to direclty access the data.
    constexpr ConstPointer data() const noexcept {
        return __data_;
    }

private:
    ConstPointer __data_ {};
    SizeType __size_ {};
};

} /* namespace ros */

inline constexpr ros::View<char> operator""_sv(const char* str, size_t len) noexcept {
    return ros::View<char>(str, len);
}

