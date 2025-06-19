#pragma once

#include <stdlib.h>
#include <string.h>

namespace coco {

class String {
public:
    String(const char* c_like_string) {
        m_size = strlen(c_like_string);
        underlying_string = (char*) malloc(m_size);
        strncpy(underlying_string, c_like_string, strlen(c_like_string));
    }

    ~String() {
        free(underlying_string);
    }

    const char* c_str() {
        return underlying_string;
    }

    int size() {
        return m_size;
    }

    char get(int index) {
        return underlying_string[index];
    }

    void modify(int index, char new_char) {
        underlying_string[index] = new_char;
    }

private:
    char* underlying_string;
    int m_size;
};

} /* namespace coco */

