#pragma once

template<bool _condition>
struct enable_if;

template<>
struct enable_if<true> {
    using type = void;
};

template<bool _condition>
using enable_if_t = enable_if<_condition>::type;

template<typename T>
struct is_floating_point {
    static constexpr bool value {false};
};

template<>
struct is_floating_point<float> {
    static constexpr bool value {true};
};

template<>
struct is_floating_point<double> {
    static constexpr bool value {true};
};

template<typename T>
inline constexpr bool is_floating_point_v {is_floating_point<T>::value};

