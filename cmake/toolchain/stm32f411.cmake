set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER  /opt/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb)
set(CMAKE_CXX_COMPILER /opt/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-eabi/bin/arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb)

set(CMAKE_C_FLAGS_INIT -ffreestanding)
set(CMAKE_CXX_FLAGS_INIT -ffreestanding)

set(CMAKE_EXE_LINKER_FLAGS_INIT -nostdlib)

