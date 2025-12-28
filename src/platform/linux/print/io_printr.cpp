#include "print/printr.h"

#include <iostream>

namespace print {
namespace impl {

void printr(ros::StringView msg) {
    std::cout << msg.data() << std::endl;
}

} /* namespace impl */
} /* namespace print */

