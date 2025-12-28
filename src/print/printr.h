#pragma once

#include "ros/string_view.h"

namespace print {

namespace impl {
    void printr(ros::StringView msg);
}

enum struct Level {
    Debug,
    Info,
    Warning,
    Error,
};

} /* namespace print */

//TODO: in some configuration file.
inline constexpr print::Level print_level {print::Level::Info};

template<print::Level _level>
void printr(ros::StringView msg) {
    if constexpr (_level >= print_level) {
        print::impl::printr(msg);
    }
}

inline constexpr auto printr_error = printr<print::Level::Error>;
inline constexpr auto printr_warn  = printr<print::Level::Warning>;
inline constexpr auto printr_info  = printr<print::Level::Info>;
inline constexpr auto printr_debug = printr<print::Level::Debug>;

