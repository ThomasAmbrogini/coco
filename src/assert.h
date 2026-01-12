#pragma once

inline constexpr void assert([[maybe_unused]] bool expression) {
#ifdef DEBUG
    if (!expression) {
        __builtin_trap();
    }
#endif
}

