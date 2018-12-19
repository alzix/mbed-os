/*
 * Copyright (c) 2017-2018 Arm Limited
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __ARM_LTD_DEVICE_CFG_H__
#define __ARM_LTD_DEVICE_CFG_H__

/**
 * \file device_cfg.h
 * \brief Configuration file native driver re-targeting
 *
 * \details This file can be used to add native driver specific macro
 *          definitions to select which peripherals are available in the build.
 *
 * This is a default device configuration file with all peripherals enabled.
 */

/* ARM SCC */
#if defined(COMPONENT_SPE)
#define MUSCA_A1_SCC_S
#endif

#if defined(COMPONENT_NSPE)
#define MUSCA_A1_SCC_NS
#define MUSCA_A1_SCC_DEV   MUSCA_A1_SCC_DEV_NS
#endif

#if defined(COMPONENT_SPE)

/* ARM Memory Protection Controller (MPC) SIE 200 */
#define MPC_ISRAM0_S
#define MPC_ISRAM1_S
#define MPC_ISRAM2_S
#define MPC_ISRAM3_S
#define MPC_CODE_SRAM_S
#define MPC_CODE_SRAM_NS
#define MPC_QSPI_S
#define MPC_QSPI_NS

/* ARM Peripheral Protection Controllers (PPC) */
#define AHB_PPC0_S
#define AHB_PPCEXP0_S
#define AHB_PPCEXP1_S
#define AHB_PPCEXP2_S
#define AHB_PPCEXP3_S

#define APB_PPC0_S
#define APB_PPC1_S
#define APB_PPCEXP0_S
#define APB_PPCEXP1_S
#define APB_PPCEXP2_S
#define APB_PPCEXP3_S

#endif // defined(COMPONENT_SPE)

/*ARM UART Controller PL011*/
#if defined(COMPONENT_NSPE)
#define UART0_PL011_NS
#define UART0_PL011_DEV                 UART0_PL011_DEV_NS
#define uart0_tx_irq_handler            UARTTX0_Handler
#define uart0_rx_irq_handler            UARTRX0_Handler
#define uart0_rx_timeout_irq_handler    UART0_RxTimeout_IRQHandler
#endif

#if defined(COMPONENT_NSPE)
#define UART1_PL011_NS
#define UART1_PL011_DEV                 UART1_PL011_DEV_NS
#define uart1_tx_irq_handler            UARTTX1_Handler
#define uart1_rx_irq_handler            UARTRX1_Handler
#define uart1_rx_timeout_irq_handler    UART1_RxTimeout_IRQHandler
#endif

/* CMSDK Timers */
#if defined(COMPONENT_SPE)
#define CMSDK_TIMER0_S
#endif

#if defined(COMPONENT_NSPE)
#define CMSDK_TIMER0_NS
#define CMSDK_TIMER0_DEV  CMSDK_TIMER0_DEV_NS
#endif

#if defined(COMPONENT_SPE)
#define CMSDK_TIMER1_S
#endif

#if defined(COMPONENT_NSPE)
#define CMSDK_TIMER1_NS
#endif

#if defined(COMPONENT_SPE)

/* Cadence QSPI Flash Controller */
#define QSPI_IP6514E_S
#define QSPI_IP6514E_NS

/* MT25QL Flash memory library */
#define MT25QL_S
#define MT25QL_NS

#endif // defined(COMPONENT_SPE)

#if defined(COMPONENT_NSPE)
/* GP Timer */
#define GP_TIMER_NS
#define GP_TIMER_DEV           GP_TIMER_DEV_NS

#define GP_TIMER_ALARM0_IRQ    GpTimer0_IRQn
#define GP_TIMER_IRQ0_HANDLER  GpTimer0_IRQHandler
#define GP_TIMER_ALARM_NR      TIMER_GP_READ_ALARM_0
#define GP_TIMER_FREQ_HZ       32768UL /* System Ref Clock */
#define GP_TIMER_BIT_WIDTH     32U

/**
 * mbed usec high-resolution ticker configuration
 */
#define USEC_TIMER_DEV                   CMSDK_TIMER0_DEV_NS
#define usec_interval_irq_handler        TIMER0_IRQHandler
#define USEC_INTERVAL_IRQ                TIMER0_IRQn
/** Timer frequency is equal to SYSTEM_CLOCK, defined in system_cmsdk_musca.c */
#define TIMER_FREQ_HZ                    50000000U
/** The us Ticker uses CMSDK Timer, that does not have HW prescaler.
 *  The reported shift define is necessary for the software emulated
 *  prescaler behavior, so the ticker works as if it was ticking on a
 *  virtually slower frequency. The value 5 sets up the ticker to work
 *  properly in the specified frequency interval.
 */
#define USEC_REPORTED_SHIFT              6
#define USEC_REPORTED_FREQ_HZ            (TIMER_FREQ_HZ >> USEC_REPORTED_SHIFT)
#define USEC_REPORTED_BITS               (32 - USEC_REPORTED_SHIFT)

#define UART_DEFAULT_BAUD_RATE           9600U
#endif // COMPONENT_NSPE

#endif  /* __ARM_LTD_DEVICE_CFG_H__ */
