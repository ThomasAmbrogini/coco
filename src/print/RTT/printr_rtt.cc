#include "coco/print/printr.hh"

#include "src/RTT/SEGGER_RTT.h"

namespace print {
namespace impl {
    void printr(coco::string_view Msg) {
        (void)Msg;
        SEGGER_RTT_WriteString(0, "Hello World from SEGGER!\r\n");
    }

} /* namespace impl */
} /* namespace print */
