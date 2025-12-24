#include "print/io_sink.h"
#include "print/print.h"
#include "ros/view.h"
#include "ros/string_view.h"

int main() {
    print::register_io_sink();

    ros::View<char> view;

    printr("Helloo");

    return 0;
}
