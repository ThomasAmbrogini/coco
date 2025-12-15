#pragma once

#include "system_info.h"
#include "system_types.h"
#include "util/math.h"

#include "drivers/ll/stm32f4xx_ll_rcc.h"

#include <limits>

namespace clk {

/**
 * @brief Convert the pllp actual value to value to write to the register.
 */
constexpr int convert_pllp_to_reg_value(int pllp) {
    return ((pllp / 2) - 1) << RCC_PLLCFGR_PLLP_Pos;
}
static_assert(convert_pllp_to_reg_value(2) == 0);
static_assert(convert_pllp_to_reg_value(4) == (1 << RCC_PLLCFGR_PLLP_Pos));
static_assert(convert_pllp_to_reg_value(6) == (2 << RCC_PLLCFGR_PLLP_Pos));
static_assert(convert_pllp_to_reg_value(8) == (3 << RCC_PLLCFGR_PLLP_Pos));

constexpr int convert_pllp_reg_value(int pllp_reg_value) {
    return (((pllp_reg_value >> RCC_PLLCFGR_PLLP_Pos) + 1) * 2);
}
static_assert(convert_pllp_reg_value(0 << RCC_PLLCFGR_PLLP_Pos) == 2);
static_assert(convert_pllp_reg_value(1 << RCC_PLLCFGR_PLLP_Pos) == 4);
static_assert(convert_pllp_reg_value(2 << RCC_PLLCFGR_PLLP_Pos) == 6);
static_assert(convert_pllp_reg_value(3 << RCC_PLLCFGR_PLLP_Pos) == 8);

//TODO: the oscillator should be a variable.
constexpr PLLParams compute_pll_params(int desired_freq_hz) {
    PLLParams ret {};
    constexpr int pllm_start {external_osc_freq_hz / max_vco_freq};
    constexpr int pllm_end {external_osc_freq_hz / min_vco_freq};

    int min_err = std::numeric_limits<int>::max();
    for (int pllm = pllm_start; pllm < pllm_end; ++pllm) {
        int vco_freq = round_to_closest_int(external_osc_freq_hz, pllm);
        for (int plln = min_plln; plln < max_plln; ++plln) {
            for (int pllp = min_pllp; pllp < max_pllp; pllp+=2) {
                int output_frequency = round_to_closest_int(vco_freq * plln, pllp);
                if (output_frequency == desired_freq_hz) {
                    return PLLParams{pllm, plln, convert_pllp_to_reg_value(pllp)};
                }
                else {
                    int err = abs(output_frequency - desired_freq_hz);
                    if (err < min_err) {
                        min_err = err;
                        ret = {.pllm = pllm, .plln = plln, .pllp = convert_pllp_to_reg_value(pllp)};
                    }
                }
            }
        }
    }

    return ret;
}

constexpr int compute_frequency(PLLParams pll_params) {
    /*
     * output_frequency = vco_freq * plln / pllp;
     * vco_freq = input_freq / pllm;
     */
    return (external_osc_freq_hz / pll_params.pllm) * pll_params.plln / convert_pllp_reg_value(pll_params.pllp);
}

template<Bus _bus, Prescaler _prescaler>
consteval u32 convert_prescaler() {
    if constexpr (_bus == Bus::AHB) {
        static_assert(_prescaler <= Prescaler::div512, "AHB bus supports up to div512 prescaler");

        switch (_prescaler) {
            case Prescaler::div1: {
                return RCC_CFGR_HPRE_DIV1;
            } break;
            case Prescaler::div2: {
                return RCC_CFGR_HPRE_DIV2;
            } break;
            case Prescaler::div4: {
                return RCC_CFGR_HPRE_DIV4;
            } break;
            case Prescaler::div8: {
                return RCC_CFGR_HPRE_DIV8;
            } break;
            case Prescaler::div16: {
                return RCC_CFGR_HPRE_DIV16;
            } break;
            case Prescaler::div64: {
                return RCC_CFGR_HPRE_DIV64;
            } break;
            case Prescaler::div128: {
                return RCC_CFGR_HPRE_DIV128;
            } break;
            case Prescaler::div256: {
                return RCC_CFGR_HPRE_DIV256;
            } break;
            case Prescaler::div512: {
                return RCC_CFGR_HPRE_DIV256;
            } break;
            default: {
            } break;
        }
    } else if constexpr (_bus == Bus::APB1) {
        static_assert(_prescaler <= Prescaler::div16, "APB1 bus supports up to div16 prescaler");

        switch (_prescaler) {
            case Prescaler::div1: {
                return RCC_CFGR_PPRE1_DIV1;
            } break;
            case Prescaler::div2: {
                return RCC_CFGR_PPRE1_DIV2;
            } break;
            case Prescaler::div4: {
                return RCC_CFGR_PPRE1_DIV4;
            } break;
            case Prescaler::div8: {
                return RCC_CFGR_PPRE1_DIV8;
            } break;
            case Prescaler::div16: {
                return RCC_CFGR_PPRE1_DIV16;
            } break;
            case Prescaler::div64:
            case Prescaler::div128:
            case Prescaler::div256:
            case Prescaler::div512:
            default: {
            } break;
        }
    } else if constexpr (_bus == Bus::APB2) {
        static_assert(_prescaler <= Prescaler::div16, "APB2 bus supports up to div16 prescaler");

        switch (_prescaler) {
            case Prescaler::div1: {
                return RCC_CFGR_PPRE1_DIV1;
            } break;
            case Prescaler::div2: {
                return RCC_CFGR_PPRE1_DIV2;
            } break;
            case Prescaler::div4: {
                return RCC_CFGR_PPRE1_DIV4;
            } break;
            case Prescaler::div8: {
                return RCC_CFGR_PPRE1_DIV8;
            } break;
            case Prescaler::div16: {
                return RCC_CFGR_PPRE1_DIV16;
            } break;
            case Prescaler::div64:
            case Prescaler::div128:
            case Prescaler::div256:
            case Prescaler::div512:
            default: {
            } break;
        }
    }
}

template<Prescaler _prescaler>
constexpr int compute_bus_freq(int parent_clk_freq_hz) {
    return parent_clk_freq_hz / static_cast<int>(_prescaler);
}

template<int _hclk_freq_hz>
consteval int compute_flash_latency() {
    static_assert(_hclk_freq_hz > 0 && _hclk_freq_hz  <= 100000000, "The HCLK freq is too high");
    if constexpr (_hclk_freq_hz > 0 && _hclk_freq_hz <= 30000000) {
        return 0;
    } else if constexpr ((_hclk_freq_hz > 30000000) && (_hclk_freq_hz <= 64000000)) {
        return 1;
    } else if constexpr ((_hclk_freq_hz > 64000000) && (_hclk_freq_hz <= 90000000)) {
        return 2;
    } else if constexpr ((_hclk_freq_hz > 90000000) && (_hclk_freq_hz <= 100000000)) {
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

