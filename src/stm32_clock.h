#pragma once

#include "stm32_clock_util.h"
#include "system_conf.h"
#include "system_info.h"
#include "util/math.h"

#include "stm32f4xx.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_utils.h"

#include <limits>

namespace clk {

inline constexpr int sysclk_freq_hz {compute_frequency(compute_pll_params(desired_sysclk_freq_hz))};
inline constexpr int ahb_freq_hz {compute_bus_freq<Bus::AHB, ahb_prescaler>(sysclk_freq_hz)};
inline constexpr int apb1_freq_hz {compute_bus_freq<Bus::APB1, apb1_prescaler>(sysclk_freq_hz)};
inline constexpr int apb2_freq_hz {compute_bus_freq<Bus::APB2, apb2_prescaler>(sysclk_freq_hz)};

static inline void configure_HSE() {
    /* hse_ready is not placed to true until the HSE is not turned on. */
    if (!LL_RCC_HSE_IsOn()) {
        LL_RCC_HSE_Enable();
        /*
         * I can switch to a clock only when it is read, but it seems that even if
         * i do it before it is actually ready, it will wait until it is ready before
         * changing the system clock to the specified source.
         */
        while(!LL_RCC_HSE_IsReady()) {
        }

        /* hse on is true in here. */
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE)
    {
    }
}

static inline void configure_HSI() {
    if (!LL_RCC_HSI_IsReady()) {
        LL_RCC_HSI_Enable();
        while (!LL_RCC_HSI_IsReady()) {
        }
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
    {
    }
}

static inline void configure_PLL() {
    if (!LL_RCC_PLL_IsReady()) {
        LL_RCC_PLL_Enable();
        while (!LL_RCC_PLL_IsReady()) {
        }
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
    {
    }
}

/**
 * @details
 *    The system is supposed to be used statically and the frequency of the
 *    system is not supposed to change after initialization.
 */
template<ClockSource _clock_source, int _desired_frequency>
void clock_configuration() {
    static constexpr bool acelerating_freq {_desired_frequency > starting_core_freq_hz};
    if constexpr (acelerating_freq) {
        static constexpr int num_wait_states {compute_flash_latency<sysclk_freq_hz>()};

        LL_FLASH_SetLatency(num_wait_states);
        while(LL_FLASH_GetLatency()!= num_wait_states) {
        }
    }

    //This is a register which regulates the maximum voltage which can be achieved.
    //This belongs here because if the PLL is configured or not decides the values
    //which can be set in the VOS register. In the operating conditions it is
    //possible to see the voltages at which the CPU will run based on the voltage
    //scale.
    /* THESE BITS CAN ONLY BE MODIFIED WHEN THE PLL IS OFF. */
    if constexpr ((_clock_source == ClockSource::PLL_HSI) || (_clock_source == ClockSource::PLL_HSE)) {
        if constexpr (ahb_freq_hz <= 64000000) {
            LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
        } else if constexpr (ahb_freq_hz <= 84000000) {
            LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
        } else if constexpr (ahb_freq_hz <= 100000000) {
            LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
        }
    }

    if constexpr (_clock_source == ClockSource::HSE) {
        configure_HSE();
    } else if constexpr (_clock_source == ClockSource::HSI) {
        configure_HSI();
    } else if constexpr (_clock_source == ClockSource::PLL_HSI) {
        static constexpr PLLParams params {compute_pll_params(_desired_frequency)};

        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, params.pllm, params.plln, params.pllp);
        configure_PLL();
    } else if constexpr (_clock_source == ClockSource::PLL_HSE) {
        static constexpr PLLParams params {compute_pll_params(_desired_frequency)};

        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, params.pllm, params.plln, params.pllp);

        LL_RCC_HSE_Enable();
        while(!LL_RCC_HSE_IsReady()) {
        }

        configure_PLL();
    }

    static constexpr u32 ahb_prescaler {convert_prescaler<Bus::AHB, Prescaler::div1>()};
    static constexpr u32 apb1_prescaler {convert_prescaler<Bus::APB1, Prescaler::div1>()};
    static constexpr u32 apb2_prescaler {convert_prescaler<Bus::APB1, Prescaler::div1>()};

    LL_RCC_SetAHBPrescaler(ahb_prescaler);
    LL_RCC_SetAPB1Prescaler(apb1_prescaler);
    LL_RCC_SetAPB2Prescaler(apb2_prescaler);

    /* I am assuming that the required clock frequency for timers is the highest possible. */
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_FOUR_TIMES);

    //TODO: is there a case where you should have two clock sources, what about
    //the feature to fallback in case the HSE fails?
    //TODO: if i am using the pll with HSE as source, and the HSE fails, can i
    //use the PLL with the HSI?
    if constexpr ((_clock_source != ClockSource::HSI) && (_clock_source != ClockSource::PLL_HSI)) {
        LL_RCC_HSI_Disable();
    }

    if constexpr (!acelerating_freq) {
        static constexpr int num_wait_states {compute_flash_latency<sysclk_freq_hz>()};

        LL_FLASH_SetLatency(num_wait_states);
        while(LL_FLASH_GetLatency()!= num_wait_states) {
        }
    }

    if constexpr (clock_security_system_enabled) {
        LL_RCC_HSE_EnableCSS();
    }
}

//TODO: I want to be able to know the frequeuncy of all the buses. (maybe even
//of the single peripherals?)
//TODO: I want to be able to retrieve the current configuration.
//TODO: I want to change the clock to the different peripherals of the system.
//TODO: I want to enable the clock for the different peripherals of the system.

} /* namespace clk */

