set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER  arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb)

# Should this things actually be in here? They do depend on the application though
set(CMAKE_C_FLAGS_INIT -ffreestanding)
set(CMAKE_CXX_FLAGS_INIT -ffreestanding)

set(CMAKE_EXE_LINKER_FLAGS_INIT -nostdlib)

