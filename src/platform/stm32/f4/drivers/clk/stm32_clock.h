#pragma once

#include "stm32_clock_util.h"
#include "system_conf.h"
#include "system_info.h"
#include "util/math.h"

#include "drivers/ll/stm32f4xx_ll_pwr.h"
#include "drivers/ll/stm32f4xx_ll_system.h"

#include <limits>

namespace clk {

inline constexpr int sysclk_freq_hz {compute_frequency(compute_pll_params(desired_sysclk_freq_hz))};
inline constexpr int ahb_freq_hz {compute_bus_freq<AhbPrescaler>(sysclk_freq_hz)};
inline constexpr int apb1_freq_hz {compute_bus_freq<Apb1Prescaler>(ahb_freq_hz)};
inline constexpr int apb2_freq_hz {compute_bus_freq<Apb2Prescaler>(ahb_freq_hz)};

static_assert(sysclk_freq_hz <= 100000000);
static_assert(ahb_freq_hz <= 100000000);
static_assert(apb1_freq_hz <= 50000000);
static_assert(apb2_freq_hz <= 100000000);

/**
 * @details
 *    The system is supposed to be used statically and the frequency of the
 *    system is not supposed to change after initialization.
 */
template<clock_source _clock_source, int _desired_frequency>
void clock_configuration() {
    static constexpr bool AceleratingFreq {_desired_frequency > starting_core_freq_hz};
    if constexpr (AceleratingFreq) {
        static constexpr int NumWaitStates {compute_flash_latency<sysclk_freq_hz>()};

        LL_FLASH_SetLatency(NumWaitStates);
        while(LL_FLASH_GetLatency() != NumWaitStates) {
        }
    }

    //This is a register which regulates the maximum voltage which can be achieved.
    //This belongs here because if the PLL is configured or not decides the values
    //which can be set in the VOS register. In the operating conditions it is
    //possible to see the voltages at which the CPU will run based on the voltage
    //scale.
    /* THESE BITS CAN ONLY BE MODIFIED WHEN THE PLL IS OFF. */
    if constexpr ((_clock_source == clock_source::PLL_HSI) || (_clock_source == clock_source::PLL_HSE)) {
        if constexpr (ahb_freq_hz <= 64000000) {
            LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);
        } else if constexpr (ahb_freq_hz <= 84000000) {
            LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE2);
        } else if constexpr (ahb_freq_hz <= 100000000) {
            LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
        }
    }

    if constexpr (_clock_source == clock_source::HSE) {
        enable_HSE_switch_sys_clk();
    } else if constexpr (_clock_source == clock_source::HSI) {
        enable_HSI_switch_sys_clk();
    } else if constexpr (_clock_source == clock_source::PLL_HSI) {
        static constexpr pll_params Params {compute_pll_params(_desired_frequency)};

        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, Params.Pllm, Params.Plln, Params.Pllp);
        enable_PLL_switch_sys_clk();
    } else if constexpr (_clock_source == clock_source::PLL_HSE) {
        static constexpr pll_params Params {compute_pll_params(_desired_frequency)};

        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, Params.Pllm, Params.Plln, Params.Pllp);

        LL_RCC_HSE_Enable();
        while(!LL_RCC_HSE_IsReady()) {
        }

        enable_PLL_switch_sys_clk();
    }

    static constexpr u32 AhbPrescalerRegVal {convert_prescaler<bus::AHB, AhbPrescaler>()};
    static constexpr u32 Apb1PrescalerRegVal {convert_prescaler<bus::APB1, Apb1Prescaler>()};
    static constexpr u32 Apb2PrescalerRegVal {convert_prescaler<bus::APB2, Apb2Prescaler>()};

    LL_RCC_SetAHBPrescaler(AhbPrescalerRegVal);
    LL_RCC_SetAPB1Prescaler(Apb1PrescalerRegVal);
    LL_RCC_SetAPB2Prescaler(Apb2PrescalerRegVal);

    /* I am assuming that the required clock frequency for timers is the highest possible. */
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_FOUR_TIMES);

    //TODO: is there a case where you should have two clock sources, what about
    //the feature to fallback in case the HSE fails?
    //TODO: if i am using the pll with HSE as source, and the HSE fails, can i
    //use the PLL with the HSI?
    if constexpr ((_clock_source != clock_source::HSI) && (_clock_source != clock_source::PLL_HSI)) {
        LL_RCC_HSI_Disable();
    }

    if constexpr (!AceleratingFreq) {
        static constexpr int NumWaitStates {compute_flash_latency<sysclk_freq_hz>()};

        LL_FLASH_SetLatency(NumWaitStates);
        while(LL_FLASH_GetLatency()!= NumWaitStates) {
        }
    }

    if constexpr (ClockSecuritySystemEnabled) {
        LL_RCC_HSE_EnableCSS();
    }
}

//TODO: I want to be able to know the frequeuncy of all the buses. (maybe even
//of the single peripherals?)
//TODO: I want to be able to retrieve the current configuration.
//TODO: I want to change the clock to the different peripherals of the system.
//TODO: I want to enable the clock for the different peripherals of the system.

} /* namespace clk */

