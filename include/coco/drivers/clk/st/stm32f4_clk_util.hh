#pragma once

#include "coco/types.hh"
#include "stm32f4_clk_types.hh"
#include "coco/util/math.hh"

#include "vendor/st/ll/stm32f4xx_ll_rcc.h"

#include <limits>

namespace clk {

inline constexpr int MaxVCOFreqHz {2000000};
inline constexpr int MinVCOFreqHz {1000000};

inline constexpr int MinPLLM = 2;
inline constexpr int MaxPLLM = 63;

inline constexpr int MinPLLN = RCC_PLLN_MIN_VALUE;
inline constexpr int MaxPLLN = RCC_PLLN_MAX_VALUE;

inline constexpr int MinPLLP = 2;
inline constexpr int MaxPLLP = 8;

/**
 * @brief Convert the pllp actual value to value to write to the register.
 */
constexpr int convert_pllp_to_reg_value(int Pllp) {
    return ((Pllp / 2) - 1) << RCC_PLLCFGR_PLLP_Pos;
}
static_assert(convert_pllp_to_reg_value(2) == 0);
static_assert(convert_pllp_to_reg_value(4) == (1 << RCC_PLLCFGR_PLLP_Pos));
static_assert(convert_pllp_to_reg_value(6) == (2 << RCC_PLLCFGR_PLLP_Pos));
static_assert(convert_pllp_to_reg_value(8) == (3 << RCC_PLLCFGR_PLLP_Pos));

constexpr int convert_pllp_reg_value(int PllpRegValue) {
    return (((PllpRegValue >> RCC_PLLCFGR_PLLP_Pos) + 1) * 2);
}
static_assert(convert_pllp_reg_value(0 << RCC_PLLCFGR_PLLP_Pos) == 2);
static_assert(convert_pllp_reg_value(1 << RCC_PLLCFGR_PLLP_Pos) == 4);
static_assert(convert_pllp_reg_value(2 << RCC_PLLCFGR_PLLP_Pos) == 6);
static_assert(convert_pllp_reg_value(3 << RCC_PLLCFGR_PLLP_Pos) == 8);

constexpr pll_params compute_pll_params(int ExternalOscFreqHz, int DesiredFreqHz) {
    pll_params Ret {};
    int PllmStart {ExternalOscFreqHz / MaxVCOFreqHz};
    int PllmEnd {ExternalOscFreqHz / MinVCOFreqHz};

    int MinErr = std::numeric_limits<int>::max();
    for (int Pllm = PllmStart; Pllm < PllmEnd; ++Pllm) {
        int VCOFreqHz = round_to_closest_int(ExternalOscFreqHz, Pllm);
        for (int Plln = MinPLLN; Plln < MaxPLLN; ++Plln) {
            for (int Pllp = MinPLLP; Pllp < MaxPLLP; Pllp+=2) {
                int OutputFrequency = round_to_closest_int(VCOFreqHz * Plln, Pllp);
                if (OutputFrequency == DesiredFreqHz) {
                    return pll_params{Pllm, Plln, convert_pllp_to_reg_value(Pllp)};
                }
                else {
                    int Err = abs(OutputFrequency - DesiredFreqHz);
                    if (Err < MinErr) {
                        MinErr = Err;
                        Ret = {.Pllm = Pllm, .Plln = Plln, .Pllp = convert_pllp_to_reg_value(Pllp)};
                    }
                }
            }
        }
    }

    return Ret;
}

constexpr int compute_frequency(int ExternalOscFreqHz, pll_params PllParams) {
    /*
     * output_frequency = vco_freq * plln / pllp;
     * vco_freq = input_freq / pllm;
     */
    return (ExternalOscFreqHz / PllParams.Pllm) * PllParams.Plln / convert_pllp_reg_value(PllParams.Pllp);
}

template<bus _Bus, prescaler _Prescaler>
consteval u32 convert_prescaler() {
    if constexpr (_Bus == bus::AHB) {
        static_assert(_Prescaler <= prescaler::div512, "AHB bus supports up to div512 prescaler");

        switch (_Prescaler) {
            case prescaler::div1: {
                return RCC_CFGR_HPRE_DIV1;
            } break;
            case prescaler::div2: {
                return RCC_CFGR_HPRE_DIV2;
            } break;
            case prescaler::div4: {
                return RCC_CFGR_HPRE_DIV4;
            } break;
            case prescaler::div8: {
                return RCC_CFGR_HPRE_DIV8;
            } break;
            case prescaler::div16: {
                return RCC_CFGR_HPRE_DIV16;
            } break;
            case prescaler::div64: {
                return RCC_CFGR_HPRE_DIV64;
            } break;
            case prescaler::div128: {
                return RCC_CFGR_HPRE_DIV128;
            } break;
            case prescaler::div256: {
                return RCC_CFGR_HPRE_DIV256;
            } break;
            case prescaler::div512: {
                return RCC_CFGR_HPRE_DIV256;
            } break;
            default: {
            } break;
        }
    } else if constexpr (_Bus == bus::APB1) {
        static_assert(_Prescaler <= prescaler::div16, "APB1 bus supports up to div16 prescaler");

        switch (_Prescaler) {
            case prescaler::div1: {
                return RCC_CFGR_PPRE1_DIV1;
            } break;
            case prescaler::div2: {
                return RCC_CFGR_PPRE1_DIV2;
            } break;
            case prescaler::div4: {
                return RCC_CFGR_PPRE1_DIV4;
            } break;
            case prescaler::div8: {
                return RCC_CFGR_PPRE1_DIV8;
            } break;
            case prescaler::div16: {
                return RCC_CFGR_PPRE1_DIV16;
            } break;
            case prescaler::div64:
            case prescaler::div128:
            case prescaler::div256:
            case prescaler::div512:
            default: {
            } break;
        }
    } else if constexpr (_Bus == bus::APB2) {
        static_assert(_Prescaler <= prescaler::div16, "APB2 bus supports up to div16 prescaler");

        switch (_Prescaler) {
            case prescaler::div1: {
                return RCC_CFGR_PPRE1_DIV1;
            } break;
            case prescaler::div2: {
                return RCC_CFGR_PPRE1_DIV2;
            } break;
            case prescaler::div4: {
                return RCC_CFGR_PPRE1_DIV4;
            } break;
            case prescaler::div8: {
                return RCC_CFGR_PPRE1_DIV8;
            } break;
            case prescaler::div16: {
                return RCC_CFGR_PPRE1_DIV16;
            } break;
            case prescaler::div64:
            case prescaler::div128:
            case prescaler::div256:
            case prescaler::div512:
            default: {
            } break;
        }
    }
}

template<bus _Bus, clock_tree _ClockTree>
consteval u32 get_prescaler() {
    if constexpr (_Bus == bus::AHB) {
        static constexpr int AHBDivider {_ClockTree.SysclkFreqHz / _ClockTree.AHBFreqHz};
        return convert_prescaler<_Bus, static_cast<prescaler>(AHBDivider)>();
    } else if constexpr (_Bus == bus::APB1) {
        static constexpr int APB1Divider {_ClockTree.SysclkFreqHz / _ClockTree.APB1FreqHz};
        return convert_prescaler<_Bus, static_cast<prescaler>(APB1Divider)>();
    } else if constexpr (_Bus == bus::APB2) {
        static constexpr int APB2Divider {_ClockTree.SysclkFreqHz / _ClockTree.APB2FreqHz};
        return convert_prescaler<_Bus, static_cast<prescaler>(APB2Divider)>();
    }
}

template<prescaler _Prescaler>
constexpr int compute_bus_freq(int ParentClkFreqHz) {
    return ParentClkFreqHz / static_cast<int>(_Prescaler);
}

template<int _HCLKFreqHz>
consteval int compute_flash_latency() {
    static_assert(_HCLKFreqHz > 0 && _HCLKFreqHz  <= 100000000, "The HCLK freq is too high");
    if constexpr (_HCLKFreqHz > 0 && _HCLKFreqHz <= 30000000) {
        return 0;
    } else if constexpr ((_HCLKFreqHz > 30000000) && (_HCLKFreqHz <= 64000000)) {
        return 1;
    } else if constexpr ((_HCLKFreqHz > 64000000) && (_HCLKFreqHz <= 90000000)) {
        return 2;
    } else if constexpr ((_HCLKFreqHz > 90000000) && (_HCLKFreqHz <= 100000000)) {
        return 3;
    }
}

inline void enable_HSE_switch_sys_clk() {
    if (!LL_RCC_HSE_IsOn()) {
        LL_RCC_HSE_Enable();
        while(!LL_RCC_HSE_IsReady()) {
        }
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE)
    {
    }
}

inline void enable_HSI_switch_sys_clk() {
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

inline void enable_PLL_switch_sys_clk() {
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

} /* namespace clk */

