#pragma once

#include "assert.h"
#include "util/type_displayer.h"

namespace ros {

template<typename _T>
struct remove_cv {
    using Type = _T;
};

template<typename _T>
struct remove_cv<const _T> {
    using Type = _T;
};

template<typename _T>
struct remove_cv<volatile _T> {
    using Type = _T;
};

template<typename _T>
struct remove_cv<const volatile _T> {
    using Type = _T;
};

template<typename _T>
struct iterator_traits {
    static_assert(false, "iterator_traits only works for pointers right now.");
};

template<typename _T>
struct iterator_traits<_T*> {
    using DifferenceType = std::ptrdiff_t;
    using ValueType      = _T;
    using Pointer        = ValueType*;
    using Reference      = ValueType&;
};

template<typename _It>
class BoundedIterator {
public:
    using IteratorType   = _It;
    using ValueType      = iterator_traits<IteratorType>::ValueType;
    using DifferenceType = iterator_traits<IteratorType>::DifferenceType;
    using Pointer        = iterator_traits<IteratorType>::Pointer;
    using Reference      = iterator_traits<IteratorType>::Reference;

    constexpr BoundedIterator(IteratorType current, IteratorType start, IteratorType end) : __current_(current),
                                                                                            __start_(start),
                                                                                            __end_(end) {
        assert(__current_ >= __start_);
        assert(__current_ < __end_);
    }

    constexpr BoundedIterator(IteratorType start, IteratorType end) : __current_(start),
                                                                      __start_(start),
                                                                      __end_(end) {
        assert(__start_ < __end_);
    }

    constexpr BoundedIterator operator++() {
        assert(__current_ != __end_);
        ++__current_;
    }

    constexpr Reference operator*() {
        assert(__current_ != __end_);
        *__current_;
    }

private:
    IteratorType __current_;
    IteratorType __start_;
    IteratorType __end_;
    TypeDisplayer<Reference> type;
};

} /* namespace ros */

