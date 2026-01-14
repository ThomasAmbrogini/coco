#pragma once

#include "coco/string_view.h"

namespace print {

namespace impl {
    void printr(coco::string_view Msg);
}

enum struct level {
    Debug,
    Info,
    Warning,
    Error,
};

} /* namespace print */

//TODO: in some configuration file.
inline constexpr print::level PrintLevel {print::level::Info};

template<print::level _Level>
void printr(coco::string_view Msg) {
    if constexpr (_Level >= PrintLevel) {
        print::impl::printr(Msg);
    }
}

inline constexpr auto printr_error = printr<print::level::Error>;
inline constexpr auto printr_warn  = printr<print::level::Warning>;
inline constexpr auto printr_info  = printr<print::level::Info>;
inline constexpr auto printr_debug = printr<print::level::Debug>;

