#pragma once

#include "stm32_clock_util.h"
#include "system_conf.h"
#include "system_info.h"
#include "util/math.h"

#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_system.h"

#include <limits>

namespace clk {

inline constexpr int sysclk_freq_hz {compute_frequency(compute_pll_params(desired_sysclk_freq_hz))};
inline constexpr int ahb_freq_hz {compute_bus_freq<ahb_prescaler>(sysclk_freq_hz)};
inline constexpr int apb1_freq_hz {compute_bus_freq<apb1_prescaler>(ahb_freq_hz)};
inline constexpr int apb2_freq_hz {compute_bus_freq<apb2_prescaler>(ahb_freq_hz)};

static_assert(sysclk_freq_hz <= 100000000);
static_assert(ahb_freq_hz <= 100000000);
static_assert(apb1_freq_hz <= 50000000);
static_assert(apb2_freq_hz <= 100000000);

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
        enable_HSE_switch_sys_clk();
    } else if constexpr (_clock_source == ClockSource::HSI) {
        enable_HSI_switch_sys_clk();
    } else if constexpr (_clock_source == ClockSource::PLL_HSI) {
        static constexpr PLLParams params {compute_pll_params(_desired_frequency)};

        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, params.pllm, params.plln, params.pllp);
        enable_PLL_switch_sys_clk();
    } else if constexpr (_clock_source == ClockSource::PLL_HSE) {
        static constexpr PLLParams params {compute_pll_params(_desired_frequency)};

        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, params.pllm, params.plln, params.pllp);

        LL_RCC_HSE_Enable();
        while(!LL_RCC_HSE_IsReady()) {
        }

        enable_PLL_switch_sys_clk();
    }

    static constexpr u32 ahb_prescaler_reg_val {convert_prescaler<Bus::AHB, ahb_prescaler>()};
    static constexpr u32 apb1_prescaler_reg_val {convert_prescaler<Bus::APB1, apb1_prescaler>()};
    static constexpr u32 apb2_prescaler_reg_val {convert_prescaler<Bus::APB2, apb2_prescaler>()};

    LL_RCC_SetAHBPrescaler(ahb_prescaler_reg_val);
    LL_RCC_SetAPB1Prescaler(apb1_prescaler_reg_val);
    LL_RCC_SetAPB2Prescaler(apb2_prescaler_reg_val);

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

