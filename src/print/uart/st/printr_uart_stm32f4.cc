#include "coco/print/printr.hh"

#include "coco/drivers/uart/st/stm32f4_uart.hh"

#include "board_config.hh"

namespace print {

namespace impl {
    void printr(coco::string_view Msg) {
        static constexpr uart::instance PrintUartInstance {uart::instance::_2};
        static constexpr uart::uart_config UartConfig {config::GlobalDeviceInfo.UartConfig[static_cast<int>(PrintUartInstance)]};
        uart::write_blocking<PrintUartInstance, UartConfig.DataBits>(Msg);
    }
} /* namespace impl */

} /* namespace print */

