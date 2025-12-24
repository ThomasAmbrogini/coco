#pragma once

inline constexpr void assert(bool expression) {
#ifdef DEBUG
    if (!expression) {
#ifdef LINUX_PLATFORM
        *(int*) 0 = 0;
#else
        __asm volatile("bkpt #0");
#endif
    }
#endif
}

