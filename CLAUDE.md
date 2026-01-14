# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Bare-metal C++23 firmware for creating an abstraction layer, without the
operating system and which can support different boards, linux desktop, etc.

Right now it is only been developed the print function used for device logging.

There is not standard library from C/C++ and some of the functionalities which
can be useful can be reimplemented manually.

## Build Commands

### Linux (host testing)
```bash
# Build
cmake --build build_linux --config Debug
```

### STM32 (cross-compile)
```bash
# Build
cmake --build build --config Debug
```

## Architecture

- `cmake/` - utils file for cmake and toolchain files.
- `src/` - contains the source code.
- `env.sh` - bash utility which creates some aliases specific to the project.

### Source

- `src/platform/` - contains the code related to the platform on which it is running.
- `src/print/` - Platform-agnostic logging via `printr_info()`, `printr_error()`, etc.
- `src/coco/` - Custom types which are similar to standard library ones.
- `src/util/` - Various utilities.

#### Platform

- `src/platform/linux/` - Linux host build for testing functionality on linux

- `src/platform/stm32/` - STM32 bare-metal implementation
  - `cmsis/` - ARM CMSIS-Core headers
  - `f4/` - STM32F4 family code (startup, linker scripts, drivers)
  - `f3/` - STM32F3 family code (startup, linker scripts, drivers)
  - `libc/` - Minimal libc runtime support for bare-metal

### Key Components

## Project options
- Build uses `-fno-exceptions`, `-fno-rtti`, `-fno-threadsafe-statics`

### CMake Configuration
- `COCO_PLATFORM`: Target platform (`linux` or `stm32`)
- `COCO_STM32_MICRO`: STM32 variant (currently `stm32f411`)

## Coding Conventions

### General
- Namespace `coco::` for custom types used throughout the codebase
- CMSIS and LL drivers are used directly (no HAL abstraction), with constexpr intermediate abstractions
- C++23 features: `consteval`, `constexpr` constructors, concepts-ready

### Naming Conventions

| Element | Style | Example |
|---------|-------|---------|
| Functions | snake_case | `compute_pll_params()`, `get_time_ms()` |
| Classes/Structs/Enums | snake_case | `string_view`, `ring_buffer`, `clock_source` |
| Type aliases | snake_case | `value_type`, `const_pointer`, `iterator_type` |
| Variables | PascalCase | `Data`, `StartTick`, `NumWaitStates` |
| Private members | PascalCase_ | `Data_`, `Size_`, `Current_` |
| Function-like constexpr variables | snake_case | `printr_info`, `uart_write_interrupt` |
| Trait members (::value) | snake_case | `is_floating_point::value` |
| Template type parameters | _T, _It | `template<typename _T>` |
| Template value parameters | _PascalCase | `template<int _Size>` |
| Enum values | PascalCase | `clock_source::PLL_HSE`, `level::Info` |
| Inline constexpr variables | PascalCase | `PrintLevel`, `DesiredSysclkFreqHz` |
