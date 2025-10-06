#pragma once

#include "util/math.h"
#include "util/limits.h"
#include "util/type_traits.h"

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_usart.h"

namespace uart {

//TODO: i will start by doing everything for the USART2. This has to be expanded to all the others.
static USART_TypeDef* usart_reg = USART2;

static void gpio_pin_configuration();

template<float _usart_div>
constexpr uint16_t compute_brr_val() {
    static_assert(static_cast<int>(_usart_div) <= unsigned_bit_limits<12>::max());

    static constexpr uint16_t integral_part {static_cast<uint16_t>(_usart_div)};
    static constexpr float floating_part {_usart_div - integral_part};
    static constexpr int floating_part_fixed_point {round(floating_part * 16)};
    static_assert(floating_part_fixed_point <= unsigned_bit_limits<4>::max());

    return (integral_part << 4) | floating_part_fixed_point;
}

template<int _baud_rate, int _freq_clk_hz, int _oversampling>
constexpr uint16_t compute_brr_val() {
    static constexpr float usart_div = (static_cast<float>(_freq_clk_hz) / (_baud_rate * _oversampling));
    static_assert(usart_div <= unsigned_bit_limits<12>::max());
    static_assert(_baud_rate > (_freq_clk_hz / (_oversampling * unsigned_bit_limits<12>::max())), "The baud rate is lower than the minimum possible");
    return compute_brr_val<usart_div>();
}

static_assert(compute_brr_val<9600, 16000000, 16>() == compute_brr_val<104.1875f>());
static_assert(compute_brr_val<115200, 16000000, 16>() == compute_brr_val<8.6875f>());
static_assert(compute_brr_val<460800, 8000000, 16>() == compute_brr_val<1.0625f>());
static_assert(compute_brr_val<115200, 100000000, 16>() == compute_brr_val<54.253472f>());

//TODO: pass the instance as a template parameter.
//TODO: should i just pass the known values as template params?
//maybe it is better than using them directly from the global value.
void configuration() {
    //TODO: enable the clock of the right peripheral.
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
    //TODO: configure the right pin, pass the template arguments with the right parameters.
    gpio_pin_configuration();

    LL_USART_Enable(usart_reg);

    LL_USART_SetDataWidth(usart_reg, LL_USART_DATAWIDTH_8B);

    LL_USART_SetStopBitsLength(usart_reg, LL_USART_STOPBITS_1);

    //TODO: 4.Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
    //place. Configure the DMA register as explained in multibuffer communication.

    static constexpr int desired_baud_rate {9600};
    //TODO: change the value for the divider and the baud rate and place them in a system file.
    static constexpr uint16_t brr_reg_val {compute_brr_val<desired_baud_rate, clk::apb1_freq_hz, 16>()};
    usart_reg->BRR = brr_reg_val;

    //TODO: why is the set of this bit atomic?
    LL_USART_EnableDirectionTx(usart_reg);
}

void write(char single_data) {
    //TODO: timeout which returns a fail if the write can not be done.
    while(!LL_USART_IsActiveFlag_TXE(usart_reg)) {
    }
    LL_USART_TransmitData8(usart_reg, single_data);
}

void write(const char * data, int size) {
    //TODO: timeout which returns a fail if the write can not be done.
    for (int i = 0; i < size; ++i) {
        write(data[i]);
    }
}

void end_transmission() {
    while (LL_USART_IsActiveFlag_TC(usart_reg)) {
    }
}

//TODO: configure based on the timer instance.
void gpio_pin_configuration() {
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7);
}

} /* namespace uart */

