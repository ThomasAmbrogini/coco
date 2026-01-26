#pragma once

#include "assert.h"
#include "coco/iterator.hh"
#include "coco/types.hh"

namespace coco {

template<typename _T>
class view {
public:
    using value_type      = _T;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using size_type       = int;
    using const_iterator  = bounded_iterator<const_pointer>;

    consteval view() = default;

    constexpr view(const_pointer Data, size_type Size) noexcept : Data_(Data),
                                                                  Size_(Size) {
        assert(Data_ != nullptr);
        assert(Size_ > 0);
    }

    constexpr size_type size() const noexcept {
        return Size_;
    }

    //TODO: maybe i never need to direclty access the data.
    constexpr const_pointer data() const noexcept {
        return Data_;
    }

    constexpr bool drop_first() noexcept {
        if (Size_ > 0)
        {
            ++Data_;
            --Size_;
            return true;
        }

        return false;
    }

    constexpr const_iterator begin() const noexcept {
        return cbegin();
    }

    constexpr const_iterator end() const noexcept {
        return cend();
    }

    constexpr const_iterator cbegin() const noexcept {
        return const_iterator(Data_, Data_ + Size_);
    }

    constexpr const_iterator cend() const noexcept {
        return const_iterator(Data_ + Size_, Data_, Data_ + Size_);
    }

protected:
    const_pointer Data_ {};
    size_type Size_ {};
};

} /* namespace coco */

