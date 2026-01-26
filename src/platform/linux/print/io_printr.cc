#include "print/printr.h"

#include <iostream>

namespace print {
namespace impl {

void printr(coco::string_view Msg) {
    std::cout << Msg.data() << std::endl;
}

} /* namespace impl */
} /* namespace print */

