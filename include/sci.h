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
* File Name    : sci.h
* Version      : Ver 1.00
* Device       : R5F563NBDDFC (RX63N Group)
* Abstract     : Header of serial control program
* Tool-Chain   : High-performance Embedded Workshop (Version 4.09.01.007),
*                C/C++ Compiler Package for RX Family (V.1.02 Release 01)
* OS           : not use
* H/W Platform : Renesas Starter Kit for RX63N
* Description  : Asynchronous transmission and reception with SCI.
* Limitation   : none
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 01.04.2015 1.00     First Release
*******************************************************************************/
#include <stdint.h>

/* Guards against multiple inclusion */
#ifndef SCI_H
#define SCI_H

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SCI_CH0     (0)         /* SCI channnel number */
#define SCI_CH1     (1)
#define SCI_CH2     (2)
#define SCI_CH3     (3)
#define SCI_CH4     (4)
#define SCI_CH5     (5)
#define SCI_CH6     (6)
#define SCI_CH7     (7)
#define SCI_CH8     (8)
#define SCI_CH9     (9)
#define SCI_CH10    (10)
#define SCI_CH11    (11)
#define SCI_CH12    (12)

/*******************************************************************************
Typedef definitions
*******************************************************************************/
/*******************************************************************************
Macro definitions
*******************************************************************************/

/* **** Configuration of SCIn **** */
typedef struct sci_ch_iodefine    /* Data and information for each channel */
{
    /* Register struct. SCI */
    volatile  __evenaccess struct st_sci12 *regs;           /* base ptr to ch registers */

    /* Module stop Register and bit */
    volatile  __evenaccess uint32_t        *mstp;           /* ptr to mstp register     */
    uint32_t                                stop_mask;      /* mstp mask to disable ch  */

    /* Interrupt Register */
    volatile  __evenaccess uint8_t         *ir_rxi;         /* ptr to RXI IR register   */
    volatile  __evenaccess uint8_t         *ir_txi;         /* ptr to TXI IR register   */
    volatile  __evenaccess uint8_t         *ir_tei;         /* ptr to TEI IR register   */

    /* Port (TXD) */
    volatile  __evenaccess uint8_t         *txd_port_od;    /* ptr to TXD data register */
    volatile  __evenaccess uint8_t         *txd_port_d;     /* ptr to TXD register      */
    volatile  __evenaccess uint8_t         *txd_port_md;    /* ptr to TXD Mode register */
    uint32_t                                txd_bit;        /* TXD PORT group bit       */
    volatile  __evenaccess uint8_t         *txd_pdr_md;     /* ptr to TXD Mode register */
    uint32_t                                txd_mpc_set;    /* TXD MPC set data         */

    /* Port (RXD) */

    volatile  __evenaccess uint8_t         *rxd_port_d;     /* ptr to RXD register      */
    volatile  __evenaccess uint8_t         *rxd_port_md;    /* ptr to RXD Mode register */
    uint32_t                                rxd_bit;        /* RXD PORT group bit       */
    volatile  __evenaccess uint8_t         *rxd_pdr_md;     /* ptr to RXD Mode register */
    uint32_t                                rxd_mpc_set;    /* RXD MPC set data         */

}sci_ch_iodefine_t;

/* **** SCI state **** */
#pragma bit_order left
#pragma unpack
typedef union
{
    uint8_t byte;
    struct
    {
        uint8_t b_tx_busy:2;    /* Transmission busy flag   0: Transmission is ready    1: Transmission is start 
                                                            2: Transmission is busy */
        uint8_t b_rx_busy:2;    /* Reception busy flag      0: Reception is ready       1: Reception is start 
                                                            2: Reception is busy */
        uint8_t b_rx_orer:1;    /* Overrun error flag       0: No error                 1: Overrun error */
        uint8_t b_rx_fer :1;    /* Framing error flag       0: No error                 1: Framing error */
        uint8_t b_rx_per :1;    /* Parity error flag        0: No error                 1: A parity error */
        uint8_t          :1;    /* not use */
    } bit;
} sci_state_t;
#pragma packoption
#pragma bit_order

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
void    SCI_Init    (uint8_t sci_ch);
void    SCI_Receive (uint8_t sci_ch, uint8_t * pbuf, uint8_t num);
void    SCI_Transmit(uint8_t sci_ch, const uint8_t * pbuf, uint8_t num);

extern sci_state_t  sci_state;
#define SCI_B_TX_BUSY   (sci_state.bit.b_tx_busy)   /* Transmission busy flag */
#define SCI_B_RX_BUSY   (sci_state.bit.b_rx_busy)   /* Reception busy flag */
#define SCI_B_RX_ORER   (sci_state.bit.b_rx_orer)   /* Overrun error flag */
#define SCI_B_RX_FER    (sci_state.bit.b_rx_fer)    /* Framing error flag */
#define SCI_B_RX_PER    (sci_state.bit.b_rx_per)    /* Parity error flag */

#endif /* SCI_H */

/* End of File */
