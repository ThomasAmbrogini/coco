#pragma once

#include "assert.h"
#include "ros/types.h"
#include "util/type_displayer.h"

namespace ros {

template<typename _T>
struct remove_cv {
    using type = _T;
};

template<typename _T>
struct remove_cv<const _T> {
    using type = _T;
};

template<typename _T>
struct remove_cv<volatile _T> {
    using type = _T;
};

template<typename _T>
struct remove_cv<const volatile _T> {
    using type = _T;
};

template<typename _T>
struct iterator_traits {
    static_assert(false, "iterator_traits only works for pointers right now.");
};

template<typename _T>
struct iterator_traits<_T*> {
    using difference_type = ptrdiff_t;
    using value_type      = _T;
    using pointer         = value_type*;
    using reference       = value_type&;
};

template<typename _It>
class bounded_iterator {
public:
    using iterator_type   = _It;
    using value_type      = iterator_traits<iterator_type>::value_type;
    using difference_type = iterator_traits<iterator_type>::difference_type;
    using pointer         = iterator_traits<iterator_type>::pointer;
    using reference       = iterator_traits<iterator_type>::reference;

    constexpr bounded_iterator(iterator_type Current, iterator_type Start, iterator_type End) : Current_(Current),
                                                                                                Start_(Start),
                                                                                                End_(End) {
        assert(Current_ >= Start_);
        assert(Current_ < End_);
    }

    constexpr bounded_iterator(iterator_type Start, iterator_type End) : Current_(Start),
                                                                         Start_(Start),
                                                                         End_(End) {
        assert(Start_ < End_);
    }

    constexpr bounded_iterator operator++() {
        assert(Current_ != End_);
        ++Current_;
    }

    constexpr reference operator*() {
        assert(Current_ != End_);
        *Current_;
    }

private:
    iterator_type Current_;
    iterator_type Start_;
    iterator_type End_;
    type_displayer<reference> Type_;
};

} /* namespace ros */

