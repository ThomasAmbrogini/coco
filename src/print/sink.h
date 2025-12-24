#pragma once

#include "ros/string_view.h"

using write_fun = void (*)(ros::StringView data);

struct Sink {
    write_fun write {};
    bool valid {};
};

