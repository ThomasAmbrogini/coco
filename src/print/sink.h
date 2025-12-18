#pragma once

using write_fun = void (*)(const char* msg, int size);

struct Sink {
    write_fun write {};
    bool valid {};
};

