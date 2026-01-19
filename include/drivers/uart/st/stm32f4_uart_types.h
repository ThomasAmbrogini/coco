#pragma once

namespace uart {

enum class mode {
    Blocking,
    Interrupt,
    DMA,
};

enum class instance {
    _1,
    _2,
    _6,
};

enum class frame_bits {
    _8,
    _9,
};

} /* namespace uart */

