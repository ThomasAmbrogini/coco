#pragma once

#include "print/sink.h"
#include "print/print.h"

#include <iostream>

namespace print {

namespace details {

void io_write(const char* msg, int size) {
    std::cout << msg << std::endl;
}

} /* namespace details */

void register_io_sink() {
    Sink io_sink {
        .write = details::io_write,
        .valid = true,
    };

    register_sink(io_sink);
}

} /* namespace print */

