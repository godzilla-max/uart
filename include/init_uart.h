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
* File Name    : main.h
* Version      : Ver 1.00
* Device       : R5F563NBDDFC (RX63N Group)
* Abstract     : Header of serial control program
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

/* Guards against multiple inclusion */
#ifndef MAIN_H
#define MAIN_H

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* **** LEDs **** */
/* ==== LED0 (SCI transmission end) ==== */
#define LED0            (PORTA.PODR.BIT.B0)     /* P03: Output data store bit */
#define LED0_PDR        (PORTA.PDR.BIT.B0)      /* P03: I/O select bit */
/* ==== LED1 (SCI reception end) ==== */
#define LED1            (PORTA.PODR.BIT.B1)     /* P05: Output data store bit */
#define LED1_PDR        (PORTA.PDR.BIT.B1)      /* P05: I/O select bit */
/* ==== LED2 (SCI receive error) ==== */
#define LED2            (PORTA.PODR.BIT.B2)     /* P10: Output data store bit */
#define LED2_PDR        (PORTA.PDR.BIT.B2)      /* P10: I/O select bit */
/* ==== LED3                     ==== */
#define LED3            (PORTA.PODR.BIT.B6)     /* P11: Output data store bit */
#define LED3_PDR        (PORTA.PDR.BIT.B6)      /* P11: I/O select bit */
/* ==== LEDs output data ==== */
#define LED_ON          (0)                     /* LED on */
#define LED_OFF         (1)                     /* LED off */

/* **** Switch **** */
/* ==== Switch input ==== */
#define SW3_REQ         (IR(ICU, IRQ15))        /* P07/IRQ15 */
/* ==== Switch input data ==== */
#define SW_ON           (1)
#define SW_OFF          (0)

void init_uart(void);
#endif /* MAIN_H */

/* End of File */
