#pragma once

namespace uart {

enum class Mode {
    Blocking,
    Interrupt,
    DMA,
};

enum class Instance {
    _1,
    _2,
    _6,
};

enum class FrameBits {
    _8,
    _9,
};

} /* namespace uart */

