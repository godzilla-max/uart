/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* System Name  : Sample program for asynchronous communication using the SCI 
*              : that does not use the interrupt.
* File Name    : main.c
* Version      : Ver 1.00
* Device       : R5F563NBDDFC (RX63N Group)
* Abstract     : Serial control program
* Tool-Chain   : High-performance Embedded Workshop (Version 4.09.01.007)
*              : C/C++ Compiler Package for RX Family (V.1.02 Release 01)
* OS           : not use
* H/W Platform : Renesas Starter Kit for RX63N
* Description  : Asynchronous transmission and reception with SCI.
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.04.2015 1.00     First Release
*******************************************************************************/

/******************************************************************************
Includes <System Includes> , "Project Includes"
******************************************************************************/
#include <machine.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcu/rx63n/register_access/iodefine.h"
#include "r_init_clock.h"
#include "r_init_non_existent_port.h"
#include "r_init_stop_module.h"
#include "init_uart.h"
#include "sci.h"

#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* **** Buffer **** */
#define BUF_SIZE        (12)                /* Buffer size */
#define NULL_SIZE       (1)                 /* Null code size */

/******************************************************************************
Global variables and functions
******************************************************************************/
void main(void);

#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

/*******************************************************************************
Private variables and functions
*******************************************************************************/
#ifdef USE_ORIGINAL_CODE
static uint8_t rx_buf[BUF_SIZE];
static uint8_t tx_buf[] = "Hello world!";
#endif
static void port_init(void);
static void peripheral_init(void);
#ifdef USE_ORIGINAL_CODE
static void IRQ_Init(void);
#endif

/******************************************************************************
* Outline           : Main processing
* Include           : none
* Declaration       : void main(void)
* Description       : This function includes setting of no use effective module 
                      after reset, the initialization functions for the clock 
                      and non-existent portnon-existent ports. 
* Argument          : void
* Return Value      : void
******************************************************************************/
void init_uart(void)
{
#ifdef USE_ORIGINAL_CODE
    /* ---- Disable maskable interrupts ---- */
    clrpsw_i();
#endif
    /* ---- Initialize I/O ports ---- */
    port_init();
#ifdef USE_ORIGINAL_CODE
    /* ---- Stopping the peripherals which start operations  ---- */
    R_INIT_StopModule();

    /* ---- Initialization of the non-existent ports ---- */
    R_INIT_NonExistentPort();

    /* ---- Initialization of the clock ---- */
    R_INIT_Clock();
#endif
    /* ---- Initialize peripheral functions ---- */
    peripheral_init();
#ifdef USE_ORIGINAL_CODE
    /* ---- Enable maskable interrupts ---- */
    setpsw_i();

    while(1) 
    {
        if (SW_ON == SW3_REQ)
        {
            SW3_REQ = SW_OFF;
            SCI_B_TX_BUSY = 1;
            SCI_B_RX_BUSY = 1;
        }
        SCI_Receive(SCI_CH0, rx_buf, BUF_SIZE);
        SCI_Transmit(SCI_CH0, tx_buf, (sizeof(tx_buf) - NULL_SIZE));
    }
#endif
}

/*******************************************************************************
* Outline      : Initialization of I/O ports
* Header       : none
* Function Name: port_init
* Description  : Initialize I/O ports.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void port_init (void)
{

    /* ---- Initialize LEDs ---- */
    /* Start with LEDs off. */
    LED0 = LED_OFF;
    LED1 = LED_OFF;
    LED2 = LED_OFF;
    LED3 = LED_OFF;

    /* Set LED pins as outputs. */
    LED0_PDR = 1;
    LED1_PDR = 1;
    LED2_PDR = 1;
    LED3_PDR = 1;

}

/*******************************************************************************
* Outline      : Initialization of peripheral functions
* Header       : none
* Function Name: peripheral_init
* Description  : Initialize peripheral functions to use.
* Arguments    : none
* Return Value : none
*******************************************************************************/
static void peripheral_init (void)
{
#ifdef USE_ORIGINAL_CODE
    uint8_t cnt;
#endif
    /* ---- Initialize SCI ---- */
    SCI_Init(SCI_CH0);
#ifdef USE_ORIGINAL_CODE
    /* ---- Initialize IRQ ---- */
    IRQ_Init();

    /* ---- Initialize RAM ---- */
    for (cnt = 0; cnt < BUF_SIZE; cnt++)
    {
        rx_buf[cnt] = 0; 
    }
#endif
}

/*******************************************************************************
* Outline      : IRQ initial setting
* Header       : none
* Function Name: IRQ_init
* Description  : Configure IRQ15 initial setting.
* Arguments    : none
* Return Value : none
*******************************************************************************/
#ifdef USE_ORIGINAL_CODE
static void IRQ_Init(void)
{
    /* ---- IRQ15(SW3) ---- */
    /* IRQ15 interrupt request is disabled */
    IEN(ICU,IRQ15) = 0;

    /* IRQFLTE1 - IRQ Pin Digital Filter Enable Register 1
    b7 FLTEN15 - IRQ15 Digital Filter Enable - Digital filter is disabled */
    ICU.IRQFLTE1.BIT.FLTEN15 = 0;

    /* IRQFLTC1 - IRQ Pin Digital Filter Setting Register 1
    b15:b14 FCLKSEL15[1:0] - IRQ15 Digital Filter Sampling Clock - PCLKB/64 */
    ICU.IRQFLTC1.BIT.FCLKSEL15 = 3;

    /* PWPR - Write-Protect Register
    b7     B0WI     - PFSWE Bit Write Disable   - Writing to the PFSWE bit is enabled
    b6     PFSWE    - PFS Register Write Enable - Writing to the PFS register is enabled
    b5:b0  Reserved - These bits are read as 0. The write value should be 0. */
    MPC.PWPR.BIT.B0WI  = 0;
    MPC.PWPR.BIT.PFSWE = 1;

    /* PFS - Pin Function Control Register
    b6 ISEL - Interrupt Input Function Select - Used as IRQ15 input pin */
    MPC.P07PFS.BIT.ISEL = 1;

    /* Enable write protection */
    MPC.PWPR.BIT.PFSWE = 0;
    MPC.PWPR.BIT.B0WI  = 1;

    /* IRQCR15 - IRQ Control Register 15
    b7:b4 Reserved   - These bits are read as 0. The write value should be 0.
    b3:b2 IRQMD[1:0] - IRQ Detection Sense Select - Falling edge
    b1:b0 Reserved   - These bits are read as 0. The write value should be 0. */
    ICU.IRQCR[15].BYTE = 0x04;

    /* Clear IRQ15 interrupt request */
    IR(ICU, IRQ15) = 0;

    /* IRQFLTE1 - IRQ Pin Digital Filter Enable Register 1
    b7 FLTEN15 - IRQ15 Digital Filter Enable - Digital filter is enabled */
    ICU.IRQFLTE1.BIT.FLTEN15 = 1;
}
#endif

#ifdef __cplusplus
void abort(void)
{

}
#endif

/* End of File */
