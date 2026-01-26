# CLAUDE.md

## Project Overview

C++23 hardware abstraction layer where the user is in charge of including the
correct driver files.

There is not standard library from C/C++ and some of the functionalities which
can be useful can be reimplemented manually.

## Architecture

- `cmake/` - utils file for cmake and toolchain files.
- `include\` - contains the actual code that can be imported by the users.
- `src/` - contains the source code.
- `examples/` - contains some example code for some boards.
- `env.sh` - bash utility which creates some aliases specific to the project.

### Key Components

## Project options
- Build uses `-fno-exceptions`, `-fno-rtti`, `-fno-threadsafe-statics`

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
