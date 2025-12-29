#include "drivers/uart/stm32_uart.h"
#include "print/ring_buffer.h"
#include "print/printr.h"
#include "ros/string_view.h"

namespace {

inline constexpr auto uart_write_blocking = uart::write<uart::Mode::Blocking, uart::Instance::_2, uart::FrameBits::_8>;
inline constexpr auto uart_write_interrupt = uart::write<uart::Mode::Interrupt, uart::Instance::_2, uart::FrameBits::_8>;

inline constexpr int print_data_size {10 * 1024};
inline constexpr int print_desc_size {64};
using PrintRingBuffer = print::RingBuffer<print_data_size, print_desc_size>;
PrintRingBuffer s_prb {};

inline constexpr auto print_store_log = print::store_log<print_data_size, print_desc_size>;
inline constexpr auto print_retrieve_log = print::retrieve_log<print_data_size, print_desc_size>;

}

namespace print::impl {

void printr(ros::StringView msg) {
    print_store_log(s_prb, msg);

    if (uart::impl::g_interrupt_data_msg.size() == 0)
    {
        ros::StringView data {print_retrieve_log(s_prb)};
        uart_write_interrupt(data);
    }
}

} /* namespace print::impl */

extern "C" void USART2_Handler() {
    const uint32_t data_register_empty {LL_USART_IsActiveFlag_TXE(USART2)};
    const uint32_t transmission_comp   {LL_USART_IsActiveFlag_TC(USART2)};
    const uint32_t it_txe_enabled      {LL_USART_IsEnabledIT_TXE(USART2)};
    const uint32_t it_tc_enabled       {LL_USART_IsEnabledIT_TC(USART2)};

    if (data_register_empty && it_txe_enabled) {
        const ros::StringView data {uart::impl::g_interrupt_data_msg};

        if (data.size() > 0)
        {
            //TODO: get a method to take the char.
            uart::write<uart::Instance::_2, uart::FrameBits::_8>(data.data()[0]);
        }

        uart::impl::g_interrupt_data_msg.drop_first();
        if (uart::impl::g_interrupt_data_msg.size() == 0)
        {
            LL_USART_DisableIT_TXE(USART2);
            LL_USART_EnableIT_TC(USART2);
        }
    }

    if (transmission_comp && it_tc_enabled) {
        //TODO: another way to check this.
        if (s_prb.desc_ring.head_id != s_prb.desc_ring.tail_id)
        {
            const ros::StringView data {print_retrieve_log(s_prb)};
            uart_write_interrupt(data);
            LL_USART_EnableIT_TXE(USART2);
        }

        LL_USART_DisableIT_TC(USART2);
    }
}
