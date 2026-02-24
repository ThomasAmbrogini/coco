#pragma once

namespace uart {

enum class instance {
    _1,
    _2,
    _3,
    _6,
};

enum class data_bits {
    _8,
    _9,
};

enum class stop_bits {
    _1,
    _2,
};

struct uart_config {
    data_bits DataBits;
    stop_bits StopBits;
    int DesiredBaudRate;
};

} /* namespace uart */

