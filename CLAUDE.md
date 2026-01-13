# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Bare-metal C++23 firmware for creating an abstraction layer, without the
operating system and which can support different boards, linux desktop.

Right now it is only been developed the print function used for device logging.

There is not standard library from C/C++ and some of the functionalities which
can be useful can be reimplemented manually.

## Build Commands

### Linux (host testing)
```bash
# Configure
cmake -B build_linux -DROS_PLATFORM=linux -S .

# Build
cmake --build build_linux
```

### STM32 (cross-compile)
```bash
# Add ARM toolchain to PATH first
source env.sh  # or: export PATH=$PATH:/opt/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin/

# Configure
cmake --toolchain cmake/toolchain/stm32f411.cmake -B build -DROS_PLATFORM=stm32 -DROS_STM32_MICRO=stm32f411 -S .

# Build
cmake --build build
```

## Architecture

- `cmake/` - utils file for cmake and toolchain files.
- `src/` - contains the source code.
- `env.sh` - bash utility which creates some aliases specific to the project.

### Source

- `src/platform/` - contains the code related to the platform on which it is running.
- `src/print/` - Platform-agnostic logging via `printr_info()`, `printr_error()`, etc.
- `src/ros/` - Custom types which are similar to standard library ones.
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
- `ROS_PLATFORM`: Target platform (`linux` or `stm32`)
- `ROS_STM32_MICRO`: STM32 variant (currently `stm32f411`)

## Coding Conventions
- Namespace `ros::` for custom types which can be used thorughout the codebase.
- CMSIS and LL drivers are used directly (no HAL abstraction), but an
  intermediate abstraction is created appositely in a constexpr way.
- C++23 features: `consteval`, `constexpr` constructors, concepts-ready

