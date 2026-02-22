#include "coco/print/printr.hh"

#include "coco/drivers/uart/st/stm32f4_uart.hh"

namespace print {

namespace impl {
    void printr(coco::string_view Msg) {
        uart::write_blocking<uart::instance::_2, uart::data_bits::_8>(Msg);
    }
} /* namespace impl */

} /* namespace print */

