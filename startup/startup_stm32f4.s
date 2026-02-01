.syntax unified
.cpu cortex-m4
.thumb

/**
 * Default handler for unimplemented interrupts.
 * Loops forever - can be overridden by user.
 */
.section .text.Default_Handler,"ax",%progbits
.global Default_Handler
.type Default_Handler, %function
Default_Handler:
    b Default_Handler
.size Default_Handler, .-Default_Handler

/**
 * Reset handler - entry point after reset.
 */
.section .text.Reset_Handler,"ax",%progbits
.weak    Reset_Handler
.type    Reset_Handler, %function
Reset_Handler:
    ldr sp, =__stack_start

init_data_section:
    mov r0, #0
    ldr r1, =__rom_data_start
    ldr r2, =__rom_data_end
    ldr r3, =__data_start

data_copy_loop:
    add r4, r1, r0
    cmp r4, r2
    beq init_bss_section
    ldr r5, [r1, r0]
    str r5, [r3, r0]
    add r0, r0, #4
    b data_copy_loop

init_bss_section:
    mov r0, #0
    mov r3, #0
    ldr r1, =__bss_start
    ldr r2, =__bss_end

zero_bss_loop:
    add r4, r1, r0
    cmp r4, r2
    beq start
    str r3, [r1, r0]
    add r0, r0, #4
    b zero_bss_loop

start:
    bl main
    b .
.size Reset_Handler, .-Reset_Handler

/**
 * Vector table - placed at start of flash.
 * Uses the largest STM32F4 variant (F446) for maximum compatibility.
 */
.section .isr_vector,"a",%progbits
.type g_pfnVectors, %object
.global g_pfnVectors
g_pfnVectors:
    /* Cortex-M4 system exceptions */
    .word __stack_start             /* 0:  Initial Stack Pointer */
    .word Reset_Handler             /* 1:  Reset Handler */
    .word NMI_Handler               /* 2:  NMI Handler */
    .word HardFault_Handler         /* 3:  Hard Fault Handler */
    .word MemManage_Handler         /* 4:  MPU Fault Handler */
    .word BusFault_Handler          /* 5:  Bus Fault Handler */
    .word UsageFault_Handler        /* 6:  Usage Fault Handler */
    .word 0                         /* 7:  Reserved */
    .word 0                         /* 8:  Reserved */
    .word 0                         /* 9:  Reserved */
    .word 0                         /* 10: Reserved */
    .word SVC_Handler               /* 11: SVCall Handler */
    .word DebugMon_Handler          /* 12: Debug Monitor Handler */
    .word 0                         /* 13: Reserved */
    .word PendSV_Handler            /* 14: PendSV Handler */
    .word SysTick_Handler           /* 15: SysTick Handler */

    /* STM32F4 peripheral interrupts */
    .word WWDG_IRQHandler                   /* 0:  Window Watchdog */
    .word PVD_IRQHandler                    /* 1:  PVD through EXTI Line */
    .word TAMP_STAMP_IRQHandler             /* 2:  Tamper and TimeStamp */
    .word RTC_WKUP_IRQHandler               /* 3:  RTC Wakeup */
    .word FLASH_IRQHandler                  /* 4:  Flash */
    .word RCC_IRQHandler                    /* 5:  RCC */
    .word EXTI0_IRQHandler                  /* 6:  EXTI Line 0 */
    .word EXTI1_IRQHandler                  /* 7:  EXTI Line 1 */
    .word EXTI2_IRQHandler                  /* 8:  EXTI Line 2 */
    .word EXTI3_IRQHandler                  /* 9:  EXTI Line 3 */
    .word EXTI4_IRQHandler                  /* 10: EXTI Line 4 */
    .word DMA1_Stream0_IRQHandler           /* 11: DMA1 Stream 0 */
    .word DMA1_Stream1_IRQHandler           /* 12: DMA1 Stream 1 */
    .word DMA1_Stream2_IRQHandler           /* 13: DMA1 Stream 2 */
    .word DMA1_Stream3_IRQHandler           /* 14: DMA1 Stream 3 */
    .word DMA1_Stream4_IRQHandler           /* 15: DMA1 Stream 4 */
    .word DMA1_Stream5_IRQHandler           /* 16: DMA1 Stream 5 */
    .word DMA1_Stream6_IRQHandler           /* 17: DMA1 Stream 6 */
    .word ADC_IRQHandler                    /* 18: ADC1, ADC2, ADC3 */
    .word CAN1_TX_IRQHandler                /* 19: CAN1 TX */
    .word CAN1_RX0_IRQHandler               /* 20: CAN1 RX0 */
    .word CAN1_RX1_IRQHandler               /* 21: CAN1 RX1 */
    .word CAN1_SCE_IRQHandler               /* 22: CAN1 SCE */
    .word EXTI9_5_IRQHandler                /* 23: EXTI Lines [9:5] */
    .word TIM1_BRK_TIM9_IRQHandler          /* 24: TIM1 Break and TIM9 */
    .word TIM1_UP_TIM10_IRQHandler          /* 25: TIM1 Update and TIM10 */
    .word TIM1_TRG_COM_TIM11_IRQHandler     /* 26: TIM1 Trigger/Commutation and TIM11 */
    .word TIM1_CC_IRQHandler                /* 27: TIM1 Capture Compare */
    .word TIM2_IRQHandler                   /* 28: TIM2 */
    .word TIM3_IRQHandler                   /* 29: TIM3 */
    .word TIM4_IRQHandler                   /* 30: TIM4 */
    .word I2C1_EV_IRQHandler                /* 31: I2C1 Event */
    .word I2C1_ER_IRQHandler                /* 32: I2C1 Error */
    .word I2C2_EV_IRQHandler                /* 33: I2C2 Event */
    .word I2C2_ER_IRQHandler                /* 34: I2C2 Error */
    .word SPI1_IRQHandler                   /* 35: SPI1 */
    .word SPI2_IRQHandler                   /* 36: SPI2 */
    .word USART1_IRQHandler                 /* 37: USART1 */
    .word USART2_IRQHandler                 /* 38: USART2 */
    .word USART3_IRQHandler                 /* 39: USART3 */
    .word EXTI15_10_IRQHandler              /* 40: EXTI Lines [15:10] */
    .word RTC_Alarm_IRQHandler              /* 41: RTC Alarm through EXTI */
    .word OTG_FS_WKUP_IRQHandler            /* 42: USB OTG FS Wakeup */
    .word TIM8_BRK_TIM12_IRQHandler         /* 43: TIM8 Break and TIM12 */
    .word TIM8_UP_TIM13_IRQHandler          /* 44: TIM8 Update and TIM13 */
    .word TIM8_TRG_COM_TIM14_IRQHandler     /* 45: TIM8 Trigger/Commutation and TIM14 */
    .word TIM8_CC_IRQHandler                /* 46: TIM8 Capture Compare */
    .word DMA1_Stream7_IRQHandler           /* 47: DMA1 Stream 7 */
    .word FMC_IRQHandler                    /* 48: FMC */
    .word SDIO_IRQHandler                   /* 49: SDIO */
    .word TIM5_IRQHandler                   /* 50: TIM5 */
    .word SPI3_IRQHandler                   /* 51: SPI3 */
    .word UART4_IRQHandler                  /* 52: UART4 */
    .word UART5_IRQHandler                  /* 53: UART5 */
    .word TIM6_DAC_IRQHandler               /* 54: TIM6 and DAC underrun */
    .word TIM7_IRQHandler                   /* 55: TIM7 */
    .word DMA2_Stream0_IRQHandler           /* 56: DMA2 Stream 0 */
    .word DMA2_Stream1_IRQHandler           /* 57: DMA2 Stream 1 */
    .word DMA2_Stream2_IRQHandler           /* 58: DMA2 Stream 2 */
    .word DMA2_Stream3_IRQHandler           /* 59: DMA2 Stream 3 */
    .word DMA2_Stream4_IRQHandler           /* 60: DMA2 Stream 4 */
    .word 0                                 /* 61: Reserved */
    .word 0                                 /* 62: Reserved */
    .word CAN2_TX_IRQHandler                /* 63: CAN2 TX */
    .word CAN2_RX0_IRQHandler               /* 64: CAN2 RX0 */
    .word CAN2_RX1_IRQHandler               /* 65: CAN2 RX1 */
    .word CAN2_SCE_IRQHandler               /* 66: CAN2 SCE */
    .word OTG_FS_IRQHandler                 /* 67: USB OTG FS */
    .word DMA2_Stream5_IRQHandler           /* 68: DMA2 Stream 5 */
    .word DMA2_Stream6_IRQHandler           /* 69: DMA2 Stream 6 */
    .word DMA2_Stream7_IRQHandler           /* 70: DMA2 Stream 7 */
    .word USART6_IRQHandler                 /* 71: USART6 */
    .word I2C3_EV_IRQHandler                /* 72: I2C3 Event */
    .word I2C3_ER_IRQHandler                /* 73: I2C3 Error */
    .word OTG_HS_EP1_OUT_IRQHandler         /* 74: USB OTG HS EP1 Out */
    .word OTG_HS_EP1_IN_IRQHandler          /* 75: USB OTG HS EP1 In */
    .word OTG_HS_WKUP_IRQHandler            /* 76: USB OTG HS Wakeup */
    .word OTG_HS_IRQHandler                 /* 77: USB OTG HS */
    .word DCMI_IRQHandler                   /* 78: DCMI */
    .word 0                                 /* 79: Reserved */
    .word 0                                 /* 80: Reserved */
    .word FPU_IRQHandler                    /* 81: FPU */
    .word 0                                 /* 82: Reserved */
    .word 0                                 /* 83: Reserved */
    .word SPI4_IRQHandler                   /* 84: SPI4 */
    .word 0                                 /* 85: Reserved */
    .word 0                                 /* 86: Reserved */
    .word SAI1_IRQHandler                   /* 87: SAI1 */
    .word 0                                 /* 88: Reserved */
    .word 0                                 /* 89: Reserved */
    .word 0                                 /* 90: Reserved */
    .word SAI2_IRQHandler                   /* 91: SAI2 */
    .word QUADSPI_IRQHandler                /* 92: QuadSPI */
    .word CEC_IRQHandler                    /* 93: CEC */
    .word SPDIF_RX_IRQHandler               /* 94: SPDIF-RX */
    .word FMPI2C1_EV_IRQHandler             /* 95: FMPI2C1 Event */
    .word FMPI2C1_ER_IRQHandler             /* 96: FMPI2C1 Error */
.size g_pfnVectors, .-g_pfnVectors

/**
 * Weak aliases for all interrupt handlers.
 * Default to Default_Handler if not overridden.
 */

/* Cortex-M4 system exceptions */
.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler, Default_Handler

.weak MemManage_Handler
.thumb_set MemManage_Handler, Default_Handler

.weak BusFault_Handler
.thumb_set BusFault_Handler, Default_Handler

.weak UsageFault_Handler
.thumb_set UsageFault_Handler, Default_Handler

.weak SVC_Handler
.thumb_set SVC_Handler, Default_Handler

.weak DebugMon_Handler
.thumb_set DebugMon_Handler, Default_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler, Default_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler, Default_Handler

/* STM32F4 peripheral interrupts */
.weak WWDG_IRQHandler
.thumb_set WWDG_IRQHandler, Default_Handler

.weak PVD_IRQHandler
.thumb_set PVD_IRQHandler, Default_Handler

.weak TAMP_STAMP_IRQHandler
.thumb_set TAMP_STAMP_IRQHandler, Default_Handler

.weak RTC_WKUP_IRQHandler
.thumb_set RTC_WKUP_IRQHandler, Default_Handler

.weak FLASH_IRQHandler
.thumb_set FLASH_IRQHandler, Default_Handler

.weak RCC_IRQHandler
.thumb_set RCC_IRQHandler, Default_Handler

.weak EXTI0_IRQHandler
.thumb_set EXTI0_IRQHandler, Default_Handler

.weak EXTI1_IRQHandler
.thumb_set EXTI1_IRQHandler, Default_Handler

.weak EXTI2_IRQHandler
.thumb_set EXTI2_IRQHandler, Default_Handler

.weak EXTI3_IRQHandler
.thumb_set EXTI3_IRQHandler, Default_Handler

.weak EXTI4_IRQHandler
.thumb_set EXTI4_IRQHandler, Default_Handler

.weak DMA1_Stream0_IRQHandler
.thumb_set DMA1_Stream0_IRQHandler, Default_Handler

.weak DMA1_Stream1_IRQHandler
.thumb_set DMA1_Stream1_IRQHandler, Default_Handler

.weak DMA1_Stream2_IRQHandler
.thumb_set DMA1_Stream2_IRQHandler, Default_Handler

.weak DMA1_Stream3_IRQHandler
.thumb_set DMA1_Stream3_IRQHandler, Default_Handler

.weak DMA1_Stream4_IRQHandler
.thumb_set DMA1_Stream4_IRQHandler, Default_Handler

.weak DMA1_Stream5_IRQHandler
.thumb_set DMA1_Stream5_IRQHandler, Default_Handler

.weak DMA1_Stream6_IRQHandler
.thumb_set DMA1_Stream6_IRQHandler, Default_Handler

.weak ADC_IRQHandler
.thumb_set ADC_IRQHandler, Default_Handler

.weak CAN1_TX_IRQHandler
.thumb_set CAN1_TX_IRQHandler, Default_Handler

.weak CAN1_RX0_IRQHandler
.thumb_set CAN1_RX0_IRQHandler, Default_Handler

.weak CAN1_RX1_IRQHandler
.thumb_set CAN1_RX1_IRQHandler, Default_Handler

.weak CAN1_SCE_IRQHandler
.thumb_set CAN1_SCE_IRQHandler, Default_Handler

.weak EXTI9_5_IRQHandler
.thumb_set EXTI9_5_IRQHandler, Default_Handler

.weak TIM1_BRK_TIM9_IRQHandler
.thumb_set TIM1_BRK_TIM9_IRQHandler, Default_Handler

.weak TIM1_UP_TIM10_IRQHandler
.thumb_set TIM1_UP_TIM10_IRQHandler, Default_Handler

.weak TIM1_TRG_COM_TIM11_IRQHandler
.thumb_set TIM1_TRG_COM_TIM11_IRQHandler, Default_Handler

.weak TIM1_CC_IRQHandler
.thumb_set TIM1_CC_IRQHandler, Default_Handler

.weak TIM2_IRQHandler
.thumb_set TIM2_IRQHandler, Default_Handler

.weak TIM3_IRQHandler
.thumb_set TIM3_IRQHandler, Default_Handler

.weak TIM4_IRQHandler
.thumb_set TIM4_IRQHandler, Default_Handler

.weak I2C1_EV_IRQHandler
.thumb_set I2C1_EV_IRQHandler, Default_Handler

.weak I2C1_ER_IRQHandler
.thumb_set I2C1_ER_IRQHandler, Default_Handler

.weak I2C2_EV_IRQHandler
.thumb_set I2C2_EV_IRQHandler, Default_Handler

.weak I2C2_ER_IRQHandler
.thumb_set I2C2_ER_IRQHandler, Default_Handler

.weak SPI1_IRQHandler
.thumb_set SPI1_IRQHandler, Default_Handler

.weak SPI2_IRQHandler
.thumb_set SPI2_IRQHandler, Default_Handler

.weak USART1_IRQHandler
.thumb_set USART1_IRQHandler, Default_Handler

.weak USART2_IRQHandler
.thumb_set USART2_IRQHandler, Default_Handler

.weak USART3_IRQHandler
.thumb_set USART3_IRQHandler, Default_Handler

.weak EXTI15_10_IRQHandler
.thumb_set EXTI15_10_IRQHandler, Default_Handler

.weak RTC_Alarm_IRQHandler
.thumb_set RTC_Alarm_IRQHandler, Default_Handler

.weak OTG_FS_WKUP_IRQHandler
.thumb_set OTG_FS_WKUP_IRQHandler, Default_Handler

.weak TIM8_BRK_TIM12_IRQHandler
.thumb_set TIM8_BRK_TIM12_IRQHandler, Default_Handler

.weak TIM8_UP_TIM13_IRQHandler
.thumb_set TIM8_UP_TIM13_IRQHandler, Default_Handler

.weak TIM8_TRG_COM_TIM14_IRQHandler
.thumb_set TIM8_TRG_COM_TIM14_IRQHandler, Default_Handler

.weak TIM8_CC_IRQHandler
.thumb_set TIM8_CC_IRQHandler, Default_Handler

.weak DMA1_Stream7_IRQHandler
.thumb_set DMA1_Stream7_IRQHandler, Default_Handler

.weak FMC_IRQHandler
.thumb_set FMC_IRQHandler, Default_Handler

.weak SDIO_IRQHandler
.thumb_set SDIO_IRQHandler, Default_Handler

.weak TIM5_IRQHandler
.thumb_set TIM5_IRQHandler, Default_Handler

.weak SPI3_IRQHandler
.thumb_set SPI3_IRQHandler, Default_Handler

.weak UART4_IRQHandler
.thumb_set UART4_IRQHandler, Default_Handler

.weak UART5_IRQHandler
.thumb_set UART5_IRQHandler, Default_Handler

.weak TIM6_DAC_IRQHandler
.thumb_set TIM6_DAC_IRQHandler, Default_Handler

.weak TIM7_IRQHandler
.thumb_set TIM7_IRQHandler, Default_Handler

.weak DMA2_Stream0_IRQHandler
.thumb_set DMA2_Stream0_IRQHandler, Default_Handler

.weak DMA2_Stream1_IRQHandler
.thumb_set DMA2_Stream1_IRQHandler, Default_Handler

.weak DMA2_Stream2_IRQHandler
.thumb_set DMA2_Stream2_IRQHandler, Default_Handler

.weak DMA2_Stream3_IRQHandler
.thumb_set DMA2_Stream3_IRQHandler, Default_Handler

.weak DMA2_Stream4_IRQHandler
.thumb_set DMA2_Stream4_IRQHandler, Default_Handler

.weak CAN2_TX_IRQHandler
.thumb_set CAN2_TX_IRQHandler, Default_Handler

.weak CAN2_RX0_IRQHandler
.thumb_set CAN2_RX0_IRQHandler, Default_Handler

.weak CAN2_RX1_IRQHandler
.thumb_set CAN2_RX1_IRQHandler, Default_Handler

.weak CAN2_SCE_IRQHandler
.thumb_set CAN2_SCE_IRQHandler, Default_Handler

.weak OTG_FS_IRQHandler
.thumb_set OTG_FS_IRQHandler, Default_Handler

.weak DMA2_Stream5_IRQHandler
.thumb_set DMA2_Stream5_IRQHandler, Default_Handler

.weak DMA2_Stream6_IRQHandler
.thumb_set DMA2_Stream6_IRQHandler, Default_Handler

.weak DMA2_Stream7_IRQHandler
.thumb_set DMA2_Stream7_IRQHandler, Default_Handler

.weak USART6_IRQHandler
.thumb_set USART6_IRQHandler, Default_Handler

.weak I2C3_EV_IRQHandler
.thumb_set I2C3_EV_IRQHandler, Default_Handler

.weak I2C3_ER_IRQHandler
.thumb_set I2C3_ER_IRQHandler, Default_Handler

.weak OTG_HS_EP1_OUT_IRQHandler
.thumb_set OTG_HS_EP1_OUT_IRQHandler, Default_Handler

.weak OTG_HS_EP1_IN_IRQHandler
.thumb_set OTG_HS_EP1_IN_IRQHandler, Default_Handler

.weak OTG_HS_WKUP_IRQHandler
.thumb_set OTG_HS_WKUP_IRQHandler, Default_Handler

.weak OTG_HS_IRQHandler
.thumb_set OTG_HS_IRQHandler, Default_Handler

.weak DCMI_IRQHandler
.thumb_set DCMI_IRQHandler, Default_Handler

.weak FPU_IRQHandler
.thumb_set FPU_IRQHandler, Default_Handler

.weak SPI4_IRQHandler
.thumb_set SPI4_IRQHandler, Default_Handler

.weak SAI1_IRQHandler
.thumb_set SAI1_IRQHandler, Default_Handler

.weak SAI2_IRQHandler
.thumb_set SAI2_IRQHandler, Default_Handler

.weak QUADSPI_IRQHandler
.thumb_set QUADSPI_IRQHandler, Default_Handler

.weak CEC_IRQHandler
.thumb_set CEC_IRQHandler, Default_Handler

.weak SPDIF_RX_IRQHandler
.thumb_set SPDIF_RX_IRQHandler, Default_Handler

.weak FMPI2C1_EV_IRQHandler
.thumb_set FMPI2C1_EV_IRQHandler, Default_Handler

.weak FMPI2C1_ER_IRQHandler
.thumb_set FMPI2C1_ER_IRQHandler, Default_Handler
