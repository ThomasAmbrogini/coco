.syntax unified
.cpu cortex-m4
.thumb

.section .text.Reset_Handler
.weak    Reset_Handler
.type    Reset_Handler, %function
Reset_Handler:
    @This is not really necessary
    ldr sp, =__stack_start

init_data_section:
    mov r0, 0
    ldr r1, =__rom_data_start
    ldr r2, =__rom_data_end
    ldr r3, =__data_start

data_copy_loop:
    add r4, r1, r0
    cmp r4, r2
    beq init_bss_section
    ldr r5, [r1, r0]
    str r5, [r2, r0]
    add r0, r0, 4
    b data_copy_loop

init_bss_section:
    mov r0, 0
    mov r3, 0
    ldr r1, =__bss_start
    ldr r2, =__bss_end

zero_bss_loop:
    add r4, r1, r0
    cmp r4, r2
    beq start
    str r3, [r1, r0]
    add r0, r0, 4
    b zero_bss_loop

start:
    b main

