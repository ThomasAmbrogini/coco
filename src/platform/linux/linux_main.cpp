#include "print/io_sink.h"
#include "print/print.h"

int main() {
    print::register_io_sink();

    printr("Hellooooo", sizeof("Hellooooo"));

    return 0;
}
