============================
Clock Management Specification
============================

.. contents:: Table of Contents
   :depth: 3
   :local:

1. Introduction
===============

1.1 Purpose
-----------

This specification defines the clock management subsystem for the coco embedded
firmware framework. The clock management system provides a unified, type-safe,
and compile-time validated interface for configuring system clocks across
different embedded platforms.

1.2 Design Philosophy
---------------------

The clock management system is built on the principle of **compile-time first**:

- **All configurations computed at compile time**: Clock frequencies, prescalers,
  and PLL parameters are computed using ``constexpr`` and ``consteval`` functions.
  No runtime computation overhead exists for clock configuration.

- **Compile-time validation**: Invalid configurations are caught at compile time
  using ``static_assert``. This prevents deployment of firmware with incorrect
  clock settings.

- **Frequency-based API**: Users specify desired frequencies in Hz. The system
  automatically computes the required prescalers and PLL parameters.

- **Zero runtime overhead**: All validation and computation occurs during
  compilation. The resulting binary contains only the final register values.

2. Requirements
===============

2.1 Functional Requirements
---------------------------

FR-001
    The system shall support configuration of the system clock (SYSCLK) from
    multiple sources: HSI, HSE, PLL with HSI, and PLL with HSE.

FR-002
    The system shall support configuration of bus clocks (AHB, APB1, APB2) with
    automatic prescaler computation based on desired frequencies.

FR-003
    The system shall expose actual computed frequencies as ``constexpr`` values
    for use by peripheral drivers.

FR-004
    The system shall support enabling and disabling peripheral clocks.

FR-005
    The system shall support Clock Security System (CSS) configuration for
    HSE failure detection.

2.2 Compile-Time Validation Requirements
----------------------------------------

VR-001
    The system shall reject at compile time any bus frequency configuration that
    exceeds hardware limits (e.g., APB1 ≤ 50MHz for STM32F4).

VR-002
    The system shall reject at compile time any PLL configuration that violates
    VCO frequency range constraints.

VR-003
    The system shall reject at compile time any prescaler value that is not
    achievable by the hardware.

VR-004
    The system shall reject at compile time any PLLM, PLLN, or PLLP value
    outside the valid range.

VR-005
    The system shall validate that the configured SYSCLK frequency does not
    exceed the maximum core frequency.

2.3 Platform Support Requirements
---------------------------------

PR-001
    The system shall support STM32F4 family microcontrollers as the reference
    implementation.

PR-002
    The architecture shall allow extension to other platforms (STM32F3, ESP32,
    etc.) without modification to the core interface.

PR-003
    Each platform implementation shall be self-contained within its respective
    platform directory.

3. Architecture
===============

3.1 Layer Overview
------------------

The clock management system follows a layered architecture::

    ┌─────────────────────────────────────────────────────────────┐
    │  Application                                                │
    ├─────────────────────────────────────────────────────────────┤
    │  Option A: Direct include      │  Option B: coco interface  │
    │  #include "stm32_clock.h"      │  clk::init()               │
    ├────────────────────────────────┴────────────────────────────┤
    │  Family-specific implementation                             │
    │  (stm32_clock.h, stm32_clock_util.h, etc.)                  │
    ├─────────────────────────────────────────────────────────────┤
    │  Board configuration (system_conf.h, system_info.h)         │
    │  - DesiredSysclkFreqHz                                      │
    │  - Clock source selection                                   │
    │  - Bus prescalers                                           │
    │  - Hardware constraints                                     │
    └─────────────────────────────────────────────────────────────┘

3.2 Family-Specific Implementation
----------------------------------

Each microcontroller family has its own implementation that handles:

- PLL parameter computation specific to the family's PLL structure
- Register-level configuration using LL (Low-Level) drivers
- Family-specific constraints and validation
- Flash latency configuration based on frequency

**File structure for STM32F4**::

    src/platform/stm32/f4/
    ├── drivers/clk/
    │   ├── stm32_clock.h       # Main clock configuration API
    │   └── stm32_clock_util.h  # Utility functions and helpers
    ├── system_conf.h           # User configuration (frequencies, prescalers)
    ├── system_info.h           # Hardware constraints (max frequencies, PLL limits)
    └── system_types.h          # Type definitions (enums, structs)

3.3 Simple Interface (coco)
---------------------------

The ``coco/clk/clk.h`` header provides a simplified, platform-agnostic interface::

    namespace clk {
        // Initialize clock system using board configuration
        int init();

        // Query clock tree configuration (compile-time)
        consteval clock_tree get_clock_tree();

        // Set specific frequency (template parameter)
        template<int freq_hz>
        int set_frequency();
    }

This interface reads configuration from the board's ``system_conf.h`` and
delegates to the appropriate family-specific implementation.

3.4 Board Configuration
-----------------------

Board configuration is split into two files:

**system_info.h** - Hardware-specific constants (read-only)::

    namespace clk {
        inline constexpr int ExternalOscFreqHz = 8000000;   // Board's external crystal
        inline constexpr int InternalOscFreqHz = 16000000;  // Internal RC oscillator
        inline constexpr int MaxCoreFreqHz     = 100000000; // Maximum SYSCLK
        inline constexpr int MaxAHBFreqHz      = 100000000; // Maximum AHB frequency
        inline constexpr int MaxAPB1FreqHz     = 50000000;  // Maximum APB1 frequency
        inline constexpr int MaxAPB2FreqHz     = 100000000; // Maximum APB2 frequency

        // PLL constraints
        inline constexpr int MinPLLM = 2;
        inline constexpr int MaxPLLM = 63;
        inline constexpr int MinPLLN = 50;
        inline constexpr int MaxPLLN = 432;
        inline constexpr int MinPLLP = 2;
        inline constexpr int MaxPLLP = 8;
        inline constexpr int MinVCOFreq = 1000000;
        inline constexpr int MaxVCOFreq = 2000000;
    }

**system_conf.h** - User-configurable settings::

    namespace clk {
        inline constexpr int DesiredSysclkFreqHz = 100000000;

        inline constexpr prescaler AhbPrescaler  = prescaler::div1;
        inline constexpr prescaler Apb1Prescaler = prescaler::div2;
        inline constexpr prescaler Apb2Prescaler = prescaler::div1;

        inline constexpr bool ClockSecuritySystemEnabled = true;
    }

4. API Specification
====================

4.1 Clock Source Configuration
------------------------------

**Enumeration: clock_source**

Defines the available clock sources for SYSCLK::

    enum class clock_source {
        HSI,      // High-Speed Internal oscillator
        HSE,      // High-Speed External oscillator
        PLL_HSI,  // PLL with HSI as input
        PLL_HSE,  // PLL with HSE as input
    };

4.2 System Clock (SYSCLK)
-------------------------

**Configuration Function**::

    template<clock_source _ClockSource, int _DesiredFrequency>
    void clock_configuration();

This function configures the complete clock tree:

1. Configures flash latency (before or after frequency change as appropriate)
2. Sets voltage scaling based on target frequency
3. Configures PLL parameters (if PLL source selected)
4. Enables clock source and switches SYSCLK
5. Configures bus prescalers
6. Optionally enables Clock Security System

**Usage Example**::

    // Configure for 100 MHz using PLL with external oscillator
    clk::clock_configuration<clk::clock_source::PLL_HSE, 100000000>();

4.3 Bus Clocks (AHB, APB1, APB2)
--------------------------------

**Enumeration: bus**::

    enum class bus {
        AHB,   // Advanced High-performance Bus
        APB1,  // Advanced Peripheral Bus 1 (low-speed)
        APB2,  // Advanced Peripheral Bus 2 (high-speed)
    };

**Enumeration: prescaler**::

    enum class prescaler {
        div1   = 1,
        div2   = 2,
        div4   = 4,
        div8   = 8,
        div16  = 16,
        div64  = 64,   // AHB only
        div128 = 128,  // AHB only
        div256 = 256,  // AHB only
        div512 = 512,  // AHB only
    };

**Prescaler Constraints**:

- AHB: Supports div1 through div512
- APB1: Supports div1 through div16
- APB2: Supports div1 through div16

**Computed Frequencies** (exposed as constexpr)::

    namespace clk {
        inline constexpr int SysclkFreqHz;  // Actual SYSCLK frequency
        inline constexpr int AHBFreqHz;     // Actual AHB frequency
        inline constexpr int APB1FreqHz;    // Actual APB1 frequency
        inline constexpr int APB2FreqHz;    // Actual APB2 frequency
    }

4.4 Peripheral Clocks
---------------------

**Information Structure**::

    struct information {
        int SysclkFreqHz;
        int AHBFreqHz;
        int APB1FreqHz;
        int APB2FreqHz;
    };

**Global Clock Information**::

    inline constexpr information GInformation {compute_buses_freq()};

Peripheral drivers can query clock frequencies at compile time::

    // In a UART driver
    static constexpr int UartClk = clk::APB2FreqHz;
    static constexpr int BaudDivisor = UartClk / DesiredBaudRate;

4.5 Compile-Time Queries
------------------------

**PLL Parameters Computation**::

    constexpr pll_params compute_pll_params(int DesiredFreqHz);

Returns the optimal PLL configuration for the desired frequency::

    struct pll_params {
        int Pllm;  // Input divider (2-63)
        int Plln;  // Multiplier (50-432)
        int Pllp;  // Output divider (register value)
    };

**Frequency Computation**::

    constexpr int compute_frequency(pll_params PllParams);

Computes the actual output frequency for given PLL parameters.

**Bus Frequency Computation**::

    template<prescaler _Prescaler>
    constexpr int compute_bus_freq(int ParentClkFreqHz);

Computes bus frequency based on parent clock and prescaler.

**Flash Latency Computation**::

    template<int _HCLKFreqHz>
    consteval int compute_flash_latency();

Returns the required flash wait states for the given HCLK frequency.

5. Compile-Time Validation
==========================

5.1 Frequency Limit Checks
--------------------------

All frequency limits are validated at compile time using ``static_assert``::

    static_assert(SysclkFreqHz <= 100000000,
        "SYSCLK exceeds maximum frequency of 100 MHz");

    static_assert(AHBFreqHz <= 100000000,
        "AHB exceeds maximum frequency of 100 MHz");

    static_assert(APB1FreqHz <= 50000000,
        "APB1 exceeds maximum frequency of 50 MHz");

    static_assert(APB2FreqHz <= 100000000,
        "APB2 exceeds maximum frequency of 100 MHz");

5.2 PLL Constraint Checks
-------------------------

PLL parameter ranges are validated within ``compute_pll_params``::

    // VCO input frequency constraints
    constexpr int PllmStart = ExternalOscFreqHz / MaxVCOFreq;  // Minimum PLLM
    constexpr int PllmEnd   = ExternalOscFreqHz / MinVCOFreq;  // Maximum PLLM

    // PLLN range: 50-432
    for (int Plln = MinPLLN; Plln < MaxPLLN; ++Plln) { ... }

    // PLLP values: 2, 4, 6, 8
    for (int Pllp = MinPLLP; Pllp < MaxPLLP; Pllp += 2) { ... }

5.3 Prescaler Achievability
---------------------------

Prescaler values are validated using ``static_assert`` within the conversion
functions::

    template<bus _Bus, prescaler _Prescaler>
    consteval u32 convert_prescaler() {
        if constexpr (_Bus == bus::AHB) {
            static_assert(_Prescaler <= prescaler::div512,
                "AHB bus supports up to div512 prescaler");
            // ...
        } else if constexpr (_Bus == bus::APB1) {
            static_assert(_Prescaler <= prescaler::div16,
                "APB1 bus supports up to div16 prescaler");
            // ...
        }
    }

5.4 Configuration File Validation
---------------------------------

User configuration is validated in ``system_conf.h``::

    inline constexpr int DesiredSysclkFreqHz = 100000000;
    static_assert(DesiredSysclkFreqHz <= 100000000,
        "The maximum frequency for the core is 100MHz");

    inline constexpr prescaler AhbPrescaler = prescaler::div1;
    static_assert(AhbPrescaler <= prescaler::div512,
        "The maximum prescaler for AHB is div512");

    inline constexpr prescaler Apb1Prescaler = prescaler::div2;
    static_assert(Apb1Prescaler <= prescaler::div16,
        "The maximum prescaler for APB1 is div16");

6. Configuration File Format
============================

6.1 Board Configuration (system_conf.h)
---------------------------------------

The board configuration file contains user-modifiable clock settings::

    #pragma once

    #include "system_types.h"

    namespace clk {

    // Desired system clock frequency in Hz
    inline constexpr int DesiredSysclkFreqHz = 100000000;
    static_assert(DesiredSysclkFreqHz <= 100000000,
        "The maximum frequency for the core is 100MHz");

    // Bus prescaler configuration
    inline constexpr prescaler AhbPrescaler  = prescaler::div1;
    inline constexpr prescaler Apb1Prescaler = prescaler::div2;
    inline constexpr prescaler Apb2Prescaler = prescaler::div1;

    // Enable Clock Security System for HSE failure detection
    inline constexpr bool ClockSecuritySystemEnabled = true;

    } /* namespace clk */

6.2 Hardware Information (system_info.h)
----------------------------------------

Hardware-specific constraints that should not be modified by users::

    #pragma once

    namespace clk {

    // Oscillator frequencies (board-specific)
    inline constexpr int ExternalOscFreqHz = 8000000;   // External crystal
    inline constexpr int InternalOscFreqHz = 16000000;  // Internal RC

    // Frequency at startup (before configuration)
    inline constexpr int StartingCoreFreqHz = InternalOscFreqHz;

    // Maximum frequencies (chip-specific)
    inline constexpr int MaxCoreFreqHz = 100000000;
    inline constexpr int MaxAHBFreqHz  = 100000000;
    inline constexpr int MaxAPB1FreqHz = 50000000;
    inline constexpr int MaxAPB2FreqHz = 100000000;

    // PLL parameter constraints (chip-specific)
    inline constexpr int MinPLLM = 2;
    inline constexpr int MaxPLLM = 63;
    inline constexpr int MinPLLN = 50;
    inline constexpr int MaxPLLN = 432;
    inline constexpr int MinPLLP = 2;
    inline constexpr int MaxPLLP = 8;

    // VCO frequency constraints
    inline constexpr int MinVCOFreq = 1000000;
    inline constexpr int MaxVCOFreq = 2000000;

    } /* namespace clk */

6.3 Type Definitions (system_types.h)
-------------------------------------

Shared type definitions used across the clock subsystem::

    #pragma once

    namespace clk {

    enum class prescaler {
        div1   = 1,
        div2   = 2,
        div4   = 4,
        div8   = 8,
        div16  = 16,
        div64  = 64,
        div128 = 128,
        div256 = 256,
        div512 = 512,
    };

    struct pll_params {
        int Pllm;
        int Plln;
        int Pllp;
    };

    enum class bus {
        AHB,
        APB1,
        APB2,
    };

    enum class clock_source {
        HSI,
        HSE,
        PLL_HSI,
        PLL_HSE,
    };

    struct information {
        int SysclkFreqHz;
        int AHBFreqHz;
        int APB1FreqHz;
        int APB2FreqHz;
    };

    } /* namespace clk */

7. Usage Examples
=================

7.1 Direct Include Usage
------------------------

For applications requiring full control over clock configuration::

    #include "drivers/clk/stm32_clock.h"

    int main() {
        // Configure clock using compile-time parameters
        clk::clock_configuration<clk::clock_source::PLL_HSE,
                                 clk::DesiredSysclkFreqHz>();

        // Use computed frequencies in peripheral configuration
        static_assert(clk::APB1FreqHz == 50000000,
            "Expected APB1 to be 50 MHz");

        // Access global clock information
        constexpr auto ClkInfo = clk::GInformation;
        // ClkInfo.SysclkFreqHz, ClkInfo.AHBFreqHz, etc.

        while (true) {
            // Application code
        }
    }

7.2 Simple Interface Usage
--------------------------

For applications using the simplified coco interface::

    #include "coco/clk/clk.h"

    int main() {
        // Initialize clock system using board configuration
        clk::init();

        // Query clock tree at compile time
        constexpr auto Tree = clk::get_clock_tree();

        while (true) {
            // Application code
        }
    }

7.3 Peripheral Driver Usage
---------------------------

Peripheral drivers can query clock frequencies at compile time::

    #include "drivers/clk/stm32_clock.h"

    namespace uart {

    template<int _BaudRate>
    void configure_uart1() {
        // UART1 is on APB2
        static constexpr int UartClk = clk::APB2FreqHz;
        static constexpr int Divisor = UartClk / _BaudRate;

        static_assert(UartClk % _BaudRate == 0,
            "Cannot achieve exact baud rate with current APB2 frequency");

        // Configure UART with computed divisor
        // ...
    }

    } /* namespace uart */

7.4 Conditional Compilation Based on Frequency
----------------------------------------------

Code can be conditionally compiled based on clock configuration::

    #include "drivers/clk/stm32_clock.h"

    void configure_peripheral() {
        if constexpr (clk::AHBFreqHz > 84000000) {
            // High-speed configuration
            // Enable additional prefetch, caches, etc.
        } else {
            // Standard configuration
        }
    }

8. Implementation Notes
=======================

8.1 STM32F4 Family
------------------

The STM32F4 implementation is the reference implementation and includes:

**Flash Latency Configuration**

Flash wait states must be configured before increasing frequency and after
decreasing frequency::

    template<int _HCLKFreqHz>
    consteval int compute_flash_latency() {
        if constexpr (_HCLKFreqHz <= 30000000) {
            return 0;  // 0 wait states
        } else if constexpr (_HCLKFreqHz <= 64000000) {
            return 1;  // 1 wait state
        } else if constexpr (_HCLKFreqHz <= 90000000) {
            return 2;  // 2 wait states
        } else {
            return 3;  // 3 wait states
        }
    }

**Voltage Scaling**

The STM32F4 requires appropriate voltage scaling based on frequency::

    - Scale 3 (low power):  AHB ≤ 64 MHz
    - Scale 2 (medium):     AHB ≤ 84 MHz
    - Scale 1 (high):       AHB ≤ 100 MHz

Voltage scaling must be configured before enabling the PLL.

**PLL Configuration**

The PLL output frequency is computed as::

    f_VCO = f_input / PLLM * PLLN
    f_out = f_VCO / PLLP

Where:
- f_input: HSI (16 MHz) or HSE (board-specific, typically 8-25 MHz)
- PLLM: 2-63 (divides input to 1-2 MHz VCO input)
- PLLN: 50-432 (VCO multiplier)
- PLLP: 2, 4, 6, or 8 (output divider)

8.2 Future Platforms
--------------------

When adding support for new platforms:

1. Create a new directory under ``src/platform/<platform>/``

2. Implement the following files:
   - ``drivers/clk/<platform>_clock.h`` - Main clock configuration
   - ``drivers/clk/<platform>_clock_util.h`` - Utility functions
   - ``system_conf.h`` - User configuration
   - ``system_info.h`` - Hardware constraints
   - ``system_types.h`` - Type definitions (may reuse common types)

3. Ensure all frequency limits are validated with ``static_assert``

4. Expose computed frequencies as ``constexpr`` values

5. Implement the ``coco/clk/clk.h`` interface for the platform

**Example structure for ESP32**::

    src/platform/esp32/
    ├── drivers/clk/
    │   ├── esp32_clock.h
    │   └── esp32_clock_util.h
    ├── system_conf.h
    ├── system_info.h
    └── system_types.h

Appendix A: STM32F4 Constraint Reference
========================================

+----------------+------------------+------------------+
| Parameter      | Minimum          | Maximum          |
+================+==================+==================+
| SYSCLK         | -                | 100 MHz          |
+----------------+------------------+------------------+
| AHB            | -                | 100 MHz          |
+----------------+------------------+------------------+
| APB1           | -                | 50 MHz           |
+----------------+------------------+------------------+
| APB2           | -                | 100 MHz          |
+----------------+------------------+------------------+
| PLLM           | 2                | 63               |
+----------------+------------------+------------------+
| PLLN           | 50               | 432              |
+----------------+------------------+------------------+
| PLLP           | 2                | 8 (step 2)       |
+----------------+------------------+------------------+
| VCO input      | 1 MHz            | 2 MHz            |
+----------------+------------------+------------------+
| VCO output     | 100 MHz          | 432 MHz          |
+----------------+------------------+------------------+

Appendix B: Prescaler Values
============================

**AHB Prescaler**

+------------+----------------+
| Prescaler  | Division Ratio |
+============+================+
| div1       | 1              |
+------------+----------------+
| div2       | 2              |
+------------+----------------+
| div4       | 4              |
+------------+----------------+
| div8       | 8              |
+------------+----------------+
| div16      | 16             |
+------------+----------------+
| div64      | 64             |
+------------+----------------+
| div128     | 128            |
+------------+----------------+
| div256     | 256            |
+------------+----------------+
| div512     | 512            |
+------------+----------------+

**APB1/APB2 Prescaler**

+------------+----------------+
| Prescaler  | Division Ratio |
+============+================+
| div1       | 1              |
+------------+----------------+
| div2       | 2              |
+------------+----------------+
| div4       | 4              |
+------------+----------------+
| div8       | 8              |
+------------+----------------+
| div16      | 16             |
+------------+----------------+
