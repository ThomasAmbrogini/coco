#pragma once

#include "stm32f4_clk_types.h"
#include "stm32f4_clk_util.h"
#include "util/math.h"

#include "device/st/stm32f4_device.h"
#include "vendor/st/ll/stm32f4xx_ll_pwr.h"
#include "vendor/st/ll/stm32f4xx_ll_system.h"

#include <limits>

namespace clk {

/**
 * @details
 *    The system is supposed to be used statically and the frequency of the
 *    system is not supposed to change after initialization.
 */
template<device_info _DeviceInfo>
void clock_configuration() {
    static constexpr int StartingCoreFreqHz {_DeviceInfo.ClockConfig.ClockTree.StartingCoreFreqHz};
    static constexpr clock_tree ClockTree {_DeviceInfo.ClockConfig.ClockTree};

    static constexpr bool AceleratingFreq {ClockTree.SysclkFreqHz > StartingCoreFreqHz};
    if constexpr (AceleratingFreq) {
        static constexpr int NumWaitStates {compute_flash_latency<ClockTree.SysclkFreqHz>()};

        LL_FLASH_SetLatency(NumWaitStates);
        while(LL_FLASH_GetLatency() != NumWaitStates) {
        }
    }

    if constexpr (_DeviceInfo.ClockConfiguration.ClockSource == clock_source::HSE) {
        enable_HSE_switch_sys_clk();
    } else if constexpr (_DeviceInfo.ClockConfiguration.ClockSource == clock_source::HSI) {
        enable_HSI_switch_sys_clk();
    } else if constexpr (_DeviceInfo.ClockConfiguration.ClockSource == clock_source::PLL_HSI) {
        static constexpr pll_params Params {compute_pll_params(ClockTree.SysclkFreqHz)};

        LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, Params.Pllm, Params.Plln, Params.Pllp);
        enable_PLL_switch_sys_clk();
    } else if constexpr (_DeviceInfo.ClockConfiguration.ClockSource == clock_source::PLL_HSE) {
        static constexpr pll_params Params {compute_pll_params(ClockTree.SysclkFreqHz)};

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
    if constexpr ((_DeviceInfo.ClockConfiguration.ClockSource != clock_source::HSI) &&
                  (_DeviceInfo.ClockConfiguration.ClockSource != clock_source::PLL_HSI)) {
        LL_RCC_HSI_Disable();
    }

    if constexpr (!AceleratingFreq) {
        static constexpr int NumWaitStates {compute_flash_latency<ClockTree.SysclkFreqHz>()};

        LL_FLASH_SetLatency(NumWaitStates);
        while(LL_FLASH_GetLatency()!= NumWaitStates) {
        }
    }

    //TODO: Clock security system.
}

//TODO: I want to be able to know the frequeuncy of all the buses. (maybe even
//of the single peripherals?)
//TODO: I want to be able to retrieve the current configuration.
//TODO: I want to change the clock to the different peripherals of the system.
//TODO: I want to enable the clock for the different peripherals of the system.

} /* namespace clk */

