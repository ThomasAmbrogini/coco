#pragma once

#include "stm32f4_clk_types.hh"
#include "stm32f4_clk_util.hh"
#include "coco/util/math.hh"
#include "coco/device/st/stm32f4_device.hh"

#include "vendor/st/ll/stm32f4xx_ll_pwr.h"
#include "vendor/st/ll/stm32f4xx_ll_system.h"

#include "board_config.hh"

#include <limits>

namespace clk {

namespace detail {

template<clock_config _ClockConfig>
constexpr clock_tree compute_actual_clock_tree() {
    static constexpr clock_tree ClockTree {_ClockConfig.ClockTree};

    if constexpr (_ClockConfig.ClockSource == clock_source::PLL_HSE) {
        static constexpr pll_params Params {compute_pll_params(_ClockConfig.ExternalClockFreqHz, ClockTree.SysclkFreqHz)};
        static constexpr int SysclkFreqHz {compute_frequency(_ClockConfig.ExternalClockFreqHz, Params)};

        return clock_tree {
            .SysclkFreqHz {SysclkFreqHz},
            .AHBFreqHz {SysclkFreqHz / _ClockConfig.ClockTree.APB1FreqHz},
            .APB1FreqHz {SysclkFreqHz / _ClockConfig.ClockTree.APB1FreqHz},
            .APB2FreqHz {SysclkFreqHz / _ClockConfig.ClockTree.APB2FreqHz},
        };
    } else if constexpr (_ClockConfig.ClockSource == clock_source::PLL_HSI) {
        static_assert(false, "Yet to implement");
        static constexpr pll_params Params {compute_pll_params(_ClockConfig.InternalClockFreqHz, ClockTree.SysclkFreqHz)};
        //TODO: implement.
        return clock_tree {};
    } else {
        static_assert(false, "Yet to implement");
    }
}

struct clock {
    static constexpr clock_config ClockConfig {GlobalDeviceInfo.ClockConfig};
    static constexpr clock_tree DesiredClockTree_ {GlobalDeviceInfo.ClockConfig.ClockTree};
    static constexpr clock_tree ActualClockTree_ {compute_actual_clock_tree<ClockConfig>()};
};

} /* namespace detail */

inline constexpr clock_tree ClockTree = detail::clock::ActualClockTree_;

/**
 * @details
 *    The system is supposed to be used statically and the frequency of the
 *    system is not supposed to change after initialization.
 */
void clock_configuration() {
    static constexpr int StartingCoreFreqHz {GlobalDeviceInfo.ClockConfig.InternalClockFreqHz};
    static constexpr clock_tree ClockTree {GlobalDeviceInfo.ClockConfig.ClockTree};

    static constexpr bool AceleratingFreq {ClockTree.SysclkFreqHz > StartingCoreFreqHz};
    if constexpr (AceleratingFreq) {
        static constexpr int NumWaitStates {compute_flash_latency<ClockTree.SysclkFreqHz>()};

        LL_FLASH_SetLatency(NumWaitStates);
        while(LL_FLASH_GetLatency() != NumWaitStates) {
        }
    }

    if constexpr (GlobalDeviceInfo.ClockConfig.ClockSource == clock_source::HSE) {
        enable_HSE_switch_sys_clk();
    } else if constexpr (GlobalDeviceInfo.ClockConfig.ClockSource == clock_source::HSI) {
        enable_HSI_switch_sys_clk();
    } else if constexpr (GlobalDeviceInfo.ClockConfig.ClockSource == clock_source::PLL_HSI) {
        static constexpr pll_params Params {compute_pll_params(GlobalDeviceInfo.ClockConfig.ExternalClockFreqHz, ClockTree.SysclkFreqHz)};

        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, Params.Pllm, Params.Plln, Params.Pllp);
        enable_PLL_switch_sys_clk();
    } else if constexpr (GlobalDeviceInfo.ClockConfig.ClockSource == clock_source::PLL_HSE) {
        static constexpr pll_params Params {compute_pll_params(GlobalDeviceInfo.ClockConfig.ExternalClockFreqHz, ClockTree.SysclkFreqHz)};

        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, Params.Pllm, Params.Plln, Params.Pllp);

        LL_RCC_HSE_Enable();
        while(!LL_RCC_HSE_IsReady()) {
        }

        enable_PLL_switch_sys_clk();
    }

    static constexpr u32 AHBPrescalerRegVal {get_prescaler<bus::AHB, ClockTree>()};
    static constexpr u32 APB1PrescalerRegVal {get_prescaler<bus::APB1, ClockTree>()};
    static constexpr u32 APB2PrescalerRegVal {get_prescaler<bus::APB2, ClockTree>()};

    LL_RCC_SetAHBPrescaler(AHBPrescalerRegVal);
    LL_RCC_SetAPB1Prescaler(APB1PrescalerRegVal);
    LL_RCC_SetAPB2Prescaler(APB2PrescalerRegVal);

    /* I am assuming that the required clock frequency for timers is the highest possible. */
    LL_RCC_SetTIMPrescaler(LL_RCC_TIM_PRESCALER_FOUR_TIMES);

    //TODO: is there a case where you should have two clock sources, what about
    //the feature to fallback in case the HSE fails?
    //TODO: if i am using the pll with HSE as source, and the HSE fails, can i
    //use the PLL with the HSI?
    if constexpr ((GlobalDeviceInfo.ClockConfig.ClockSource != clock_source::HSI) &&
                  (GlobalDeviceInfo.ClockConfig.ClockSource != clock_source::PLL_HSI)) {
        LL_RCC_HSI_Disable();
    }

    if constexpr (!AceleratingFreq) {
        static constexpr int NumWaitStates {compute_flash_latency<ClockTree.SysclkFreqHz>()};

        LL_FLASH_SetLatency(NumWaitStates);
        while(LL_FLASH_GetLatency()!= NumWaitStates) {
        }
    }
}

//TODO: I want to be able to know the frequeuncy of all the buses. (maybe even
//of the single peripherals?)
//TODO: I want to be able to retrieve the current configuration.
//TODO: I want to change the clock to the different peripherals of the system.
//TODO: I want to enable the clock for the different peripherals of the system.

} /* namespace clk */

