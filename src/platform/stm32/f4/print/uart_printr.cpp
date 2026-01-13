#include "drivers/uart/stm32_uart.h"
#include "print/ring_buffer.h"
#include "print/printr.h"
#include "ros/string_view.h"

namespace {

inline constexpr auto uart_write_blocking = uart::write<uart::mode::Blocking, uart::instance::_2, uart::frame_bits::_8>;
inline constexpr auto uart_write_interrupt = uart::write<uart::mode::Interrupt, uart::instance::_2, uart::frame_bits::_8>;

inline constexpr int PrintDataSize {10 * 1024};
inline constexpr int PrintDescSize {64};
using print_ring_buffer = print::ring_buffer<PrintDataSize, PrintDescSize>;
print_ring_buffer SPrb {};

inline constexpr auto print_store_log = print::store_log<PrintDataSize, PrintDescSize>;
inline constexpr auto print_retrieve_log = print::retrieve_log<PrintDataSize, PrintDescSize>;

}

namespace print::impl {

void printr(ros::string_view Msg) {
    print_store_log(SPrb, Msg);

    if (uart::impl::GInterruptDataMsg.size() == 0)
    {
        ros::string_view Data {print_retrieve_log(SPrb)};
        uart_write_interrupt(Data);
    }
}

} /* namespace print::impl */

extern "C" void USART2_Handler() {
    const uint32_t DataRegisterEmpty {LL_USART_IsActiveFlag_TXE(USART2)};
    const uint32_t TransmissionComp   {LL_USART_IsActiveFlag_TC(USART2)};
    const uint32_t ItTxeEnabled      {LL_USART_IsEnabledIT_TXE(USART2)};
    const uint32_t ItTcEnabled       {LL_USART_IsEnabledIT_TC(USART2)};

    if (DataRegisterEmpty && ItTxeEnabled) {
        const ros::string_view Data {uart::impl::GInterruptDataMsg};

        if (Data.size() > 0)
        {
            //TODO: get a method to take the char.
            uart::write<uart::instance::_2, uart::frame_bits::_8>(Data.data()[0]);
        }

        uart::impl::GInterruptDataMsg.drop_first();
        if (uart::impl::GInterruptDataMsg.size() == 0)
        {
            LL_USART_DisableIT_TXE(USART2);
            LL_USART_EnableIT_TC(USART2);
        }
    }

    if (TransmissionComp && ItTcEnabled) {
        //TODO: another way to check this.
        if (SPrb.DescRing.HeadId != SPrb.DescRing.TailId)
        {
            const ros::string_view Data {print_retrieve_log(SPrb)};
            uart_write_interrupt(Data);
            LL_USART_EnableIT_TXE(USART2);
        }

        LL_USART_DisableIT_TC(USART2);
    }
}
