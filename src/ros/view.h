#pragma once

#include "assert.h"
#include "ros/iterator.h"

#include <stddef.h>

namespace ros {

template<typename _T>
class View {
public:
    using ValueType     = _T;
    using Pointer       = ValueType*;
    using ConstPointer  = const ValueType*;
    using SizeType      = int;
    using ConstIterator = BoundedIterator<ConstPointer>;

    consteval View() = default;

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

    constexpr bool drop_first() noexcept {
        if (__size_ > 0)
        {
            ++__data_;
            --__size_;
            return true;
        }

        return false;
    }

    constexpr ConstIterator begin() const noexcept {
        return cbegin();
    }

    constexpr ConstIterator end() const noexcept {
        return cend();
    }

    constexpr ConstIterator cbegin() const noexcept {
        return ConstIterator(__data_, __data_ + __size_);
    }

    constexpr ConstIterator cend() const noexcept {
        return ConstIterator(__data_ + __size_, __data_, __data_ + __size_);
    }

protected:
    ConstPointer __data_ {};
    SizeType __size_ {};
};

} /* namespace ros */

