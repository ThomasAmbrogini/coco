#include "coco/print/printr.hh"

#include "coco/drivers/uart/st/stm32f4_uart.hh"

#include "board_config.hh"

namespace print {

namespace impl {
    void printr(coco::string_view Msg) {
        static constexpr uart::instance PrintUartInstance {uart::instance::_2};
        uart::write_blocking<PrintUartInstance, config::GlobalDeviceInfo.UartConfig[static_cast<int>(PrintUartInstance)].DataBits>(Msg);
    }
} /* namespace impl */

} /* namespace print */

