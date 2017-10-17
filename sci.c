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
* File Name    : sci.c
* Version      : Ver 1.00
* Device       : R5F563NBDDFC (RX63N Group)
* Abstract     : Serial control program
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

/*******************************************************************************
Includes <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "mcu/rx63n/register_access/iodefine.h"
#include "init_uart.h"
#include "sci.h"
#include "sci_cfg.h"

//#include "typedefine.h"
#ifdef __cplusplus
//#include <ios>                        // Remove the comment when you use ios
//_SINT ios_base::Init::init_cnt;       // Remove the comment when you use ios
#endif

/*******************************************************************************
Macro definitions
*******************************************************************************/
/* SSR - Serial Status Register
b7:b6  Reserved - The read value is undefined. The write value should be 1.
b5     ORER     - Overrun Error Flag    - An overrun error has occurred
b4     FER      - Framing Error Flag    - A framing error has occurred
b3     PER      - Parity Error Flag     - A parity error has occurred */
#define SSR_ERROR_FLAGS     (0x38)

/* **** SCI state **** */
sci_state_t  sci_state = {0x00};

/*******************************************************************************
Private variables and functions
*******************************************************************************/
/* **** SCI transmit/receive buffer **** */
static const uint8_t * pbuf_tx;             /* Pointer to transmit buffer */
static       uint8_t   tx_cnt;              /* Transmit counter */

static       uint8_t * pbuf_rx;             /* Pointer to receive buffer */
static       uint8_t   rx_cnt;              /* Receive counter */

/* **** SCI routine **** */
static void sci_recv_data_full(uint8_t sci_ch);
static void sci_recv_error(uint8_t sci_ch);
static void sci_tran_buffer_empty(uint8_t sci_ch);
static void sci_tran_end(uint8_t sci_ch);

/*******************************************************************************
* Outline      : User I/F function (Initialization of SCI)
* Header       : sci.h
* Function Name: SCI_Init
* Description  : Initialize SCI.
* Arguments    : sci_ch -
*                   Used SCI channel number
* Return Value : none
*******************************************************************************/
void SCI_Init (uint8_t sci_ch)
{

    const sci_ch_iodefine_t *sci_regs = &sci_def[sci_ch];

    /* ---- Initialization of SCI ---- */

    /* PRCR - Protect Register
    b15:b8 PRKEY    - PRC Key Code  - A5h (The write value should be A5h to permission writing PRCi bit)
    b7:b4  Reserved - The write value should be 0.
    b1     PRC1     - Protect Bit 1 - Write enabled */
    SYSTEM.PRCR.WORD = 0xA502;

    /* The module stop state of SCIn is canceled */
    *sci_regs->mstp &= ~sci_regs->stop_mask;

    /* Enable write protection */
    SYSTEM.PRCR.WORD = 0xA500;

    /* SCR - Serial Control Register
    b7     TIE  - Transmit Interrupt Enable     - A TXI interrupt request is disabled
    b6     RIE  - Receive Interrupt Enable      - RXI and ERI interrupt requests are disabled
    b5     TE   - Transmit Enable               - Serial transmission is disabled
    b4     RE   - Receive Enable                - Serial reception is disabled
    b2     TEIE - Transmit End Interrupt Enable - A TEI interrupt request is disabled */
    sci_regs->regs->SCR.BYTE = 0x00;

    while (0x00 != (sci_regs->regs->SCR.BYTE & 0xF0))
    {
        /* Confirm that bit is actually 0 */
    }

    /* ---- Set the I/O port functions ---- */

    /* Set port output data - High level */
    *sci_regs->txd_port_od |= sci_regs->txd_bit;

    /* Set port direction - TXDn is output port, RXDn is input port */
    *sci_regs->txd_port_d |= sci_regs->txd_bit;
    *sci_regs->rxd_port_d &= ~(sci_regs->rxd_bit);

    /* Set port mode - Use pin as general I/O port */
    *sci_regs->txd_port_md &= ~(sci_regs->txd_bit);
    *sci_regs->rxd_port_md &= ~(sci_regs->rxd_bit);

    /* PWPR - Write-Protect Register
    b7     B0WI     - PFSWE Bit Write Disable   - Writing to the PFSWE bit is enabled
    b6     PFSWE    - PFS Register Write Enable - Writing to the PFS register is enabled
    b5:b0  Reserved - These bits are read as 0. The write value should be 0. */
    MPC.PWPR.BIT.B0WI  = 0;
    MPC.PWPR.BIT.PFSWE = 1;

    /* PFS - Pin Function Control Register
    b3:b0  PSEL - Pin Function Select - RXDn, TXDn */
    *sci_regs->txd_pdr_md = sci_regs->txd_mpc_set;
    *sci_regs->rxd_pdr_md = sci_regs->rxd_mpc_set;

    /* Enable write protection */
    MPC.PWPR.BIT.PFSWE = 0;
    MPC.PWPR.BIT.B0WI  = 1;

    /* Use pin as I/O port for peripheral functions */
    *sci_regs->rxd_port_md |= sci_regs->rxd_bit;

    /* ---- Initialization of SCI ---- */

    /* Select an On-chip baud rate generator to the clock source */
    sci_regs->regs->SCR.BIT.CKE = 0;

    /* SMR - Serial Mode Register
    b7     CM   - Communications Mode   - Asynchronous mode
    b6     CHR  - Character Length      - Selects 8 bits as the data length
    b5     PE   - Parity Enable         - When transmitting : Parity bit addition is not performed
                                          When receiving    : Parity bit checking is not performed
    b3     STOP - Stop Bit Length       - 1 stop bits
    b2     MP   - Multi-Processor Mode  - Multi-processor communications function is disabled
    b1:b0  CKS  - Clock Select          - PCLK clock (n = 0) */
    sci_regs->regs->SMR.BYTE = 0x00;

    /* SCMR - Smart Card Mode Register
    b6:b4  Reserved - The write value should be 1.
    b3     SDIR     - Transmitted/Received Data Transfer Direction - Transfer with LSB-first
    b2     SINV     - Transmitted/Received Data Invert  - TDR contents are transmitted as they are. 
                                                          Receive data is stored as it is in RDR.
    b1     Reserved - The write value should be 1.
    b0     SMIF     - Smart Card Interface Mode Select  - Serial communications interface mode */
    sci_regs->regs->SCMR.BYTE = 0xF2;

    /* SEMR - Serial Extended Mode Register
    b7:b6  Reserved - The write value should be 0.
    b5     NFEN     - Digital Noise Filter Function Enable  - Noise cancellation function 
                                                              for the RXDn input signal is disabled.
    b4     ABCS     - Asynchronous Mode Base Clock Select   - Selects 16 base clock cycles for 1-bit period
    b3:b1  Reserved - The write value should be 0. */
    sci_regs->regs->SEMR.BYTE = 0x00;

    /* BRR - Bit Rate Register
    Bit Rate: (96MHz/(64*2^(-1)*1000000bps))-1=2 */
    sci_regs->regs->BRR = 2;

    /* ---- Initialization of SCI interrupt ---- */
    /* Interrupt request is cleared (Edge interrupt) */
    *sci_regs->ir_rxi = 0;
    *sci_regs->ir_txi = 0;
}

/*******************************************************************************
* Outline      : User I/F function (SCI reception start)
* Header       : sci.h
* Function Name: SCI_Receive
* Description  : Start SCI reception.
* Arguments    : sci_ch -
*                   Used SCI channel number
*                pbuf -
*                   Pointer to where to store the received data
*                num -
*                   Number of receive byte
* Return Value : none
*******************************************************************************/
void SCI_Receive (uint8_t sci_ch, uint8_t * pbuf, uint8_t num)
{

    const sci_ch_iodefine_t *sci_regs = &sci_def[sci_ch];

    /* **** Check the reception state **** */
    if (1 == SCI_B_RX_BUSY)
    {
        /* ---- Ready ---- */

        /* ---- Set the RAM ---- */
        SCI_B_RX_BUSY    = 2;
        SCI_B_RX_ORER    = 0;
        SCI_B_RX_FER     = 0;
        SCI_B_RX_PER     = 0;
        pbuf_rx          = pbuf;
        rx_cnt           = num;

        /* ---- Enable serial reception ---- */
        /* RXI and ERI interrupt requests are enabled */
        sci_regs->regs->SCR.BIT.RIE = 1;

        /* ---- Serial reception is enabled ---- */
        /* Setting the RE bit allows RXDn to be used. */
        sci_regs->regs->SCR.BIT.RE  = 1;

    }
    else if (2 == SCI_B_RX_BUSY)
    {
        /* ---- Busy ---- */

        /* Receive data full function */
        sci_recv_data_full(sci_ch);

    }
    else
    {
        /* ---- Stop ---- */
        SCI_B_RX_BUSY = 0;

    }
}

/*******************************************************************************
* Outline      : User I/F function (SCI transmission start)
* Header       : sci.h
* Function Name: SCI_Transmit
* Description  : Start SCI transmission.
* Arguments    : sci_ch -
*                   Used SCI channel number
*                pbuf -
*                   Pointer to where to store the transmit data
*                num  -
*                   Number of transmit byte
* Return Value : none
*******************************************************************************/
void SCI_Transmit (uint8_t sci_ch, const uint8_t * pbuf, uint8_t num)
{

    const sci_ch_iodefine_t *sci_regs = &sci_def[sci_ch];

    /* **** Check the reception state **** */
    if (1 == SCI_B_TX_BUSY)
    {
        /* ---- Ready ---- */

        /* ---- Set the RAM ---- */
        SCI_B_TX_BUSY   = 2;
        pbuf_tx         = pbuf;
        tx_cnt          = num;

        /* ---- Enable serial transmission ---- */
        /* A TXI interrupt request is enabled */
        sci_regs->regs->SCR.BIT.TIE = 1;

        /* ---- Serial transmission is enabled ---- */
        /* Setting the TE bit allows TXDn to be used. */
        sci_regs->regs->SCR.BIT.TE = 1;

        /* ---- Use pin as I/O port for peripheral functions ---- */
        *sci_regs->txd_port_md |= sci_regs->txd_bit;

    }
    else if (2 == SCI_B_TX_BUSY)
    {
        /* ---- Busy ---- */

        /* Transmit buffer empty function */
        sci_tran_buffer_empty(sci_ch);

        /* Transmit end function */
        sci_tran_end(sci_ch);

    }
    else
    {
        /* ---- Stop ---- */

        SCI_B_TX_BUSY = 0;

    }
}

/*******************************************************************************
* Outline      : Receive data full interrupt
* Header       : none
* Function Name: sci_recv_data_full
* Description  : Called in RXI interrupt processing. Store the received data.
*                When received the last data, disable serial reception and
*                Call the Callback function (SCI reception end).
* Arguments    : sci_ch -
*                   Used SCI channel number
* Return Value : none
*******************************************************************************/
static void sci_recv_data_full (uint8_t sci_ch)
{
    const sci_ch_iodefine_t *sci_regs = &sci_def[sci_ch];

    /* ---- Error processing ---- */
    /* ---- Overrun error ---- */
    if (1 == (sci_regs->regs->SSR.BIT.ORER))
    {
        SCI_B_RX_ORER = 1;
        sci_recv_error(sci_ch);
    }
    /* ---- Framing error ---- */
    else if (1 == (sci_regs->regs->SSR.BIT.FER))
    {
        SCI_B_RX_FER = 1;
        sci_recv_error(sci_ch);
    }
    /* ---- Parity error ---- */
    else if (1 == (sci_regs->regs->SSR.BIT.PER))
    {
        SCI_B_RX_PER = 1;
        sci_recv_error(sci_ch);
    }
    else
    {

        /* Check RXI interrupt request */
        if (1 == (*sci_regs->ir_rxi))
        {
            /* RXI interrupt request is cleared */
            *sci_regs->ir_rxi = 0;

            /* ---- Store the received data ---- */
            *pbuf_rx = sci_regs->regs->RDR;
            pbuf_rx++;                      /* Update of the receive buffer pointer */
            rx_cnt--;                       /* Downcount of the reception counter */

            /* ---- When received the last data ---- */
            if (0 == rx_cnt)
            {
                /* ---- Disable serial reception ---- */
                sci_regs->regs->SCR.BIT.RE = 0;

                while (0 != (sci_regs->regs->SCR.BIT.RE))
                {
                    /* Confirm that bit is actually 0 */
                }

                /* ---- Disable ERI and RXI interrupt requests ---- */
                sci_regs->regs->SCR.BIT.RIE = 0;

                while (0 != (sci_regs->regs->SCR.BIT.RIE))
                {
                    /* Confirm that bit is actually 0 */
                }

                /* RXI interrupt request is cleared */
                *sci_regs->ir_rxi = 0;

                /* ---- Clear the reception busy flag ---- */
                SCI_B_RX_BUSY = 0;

                LED1 = ~LED1;               /* LED1 ON (SCI reception end) */
            }
        }
    }
}

/*******************************************************************************
* Outline      : Receive error function
* Header       : none
* Function Name: sci_recv_error
* Description  : Called in ERI interrupt processing. Disable serial reception.
*                Call the Callback function (SCI receive error).
* Arguments    : sci_ch -
*                   Used SCI channel number
* Return Value : none
*******************************************************************************/
static void sci_recv_error (uint8_t sci_ch)
{

    const sci_ch_iodefine_t *sci_regs = &sci_def[sci_ch];
    volatile uint8_t dummy;

    /* Dummy-read the RDR register */
    dummy = sci_regs->regs->RDR;

    /* ---- Disable serial reception ---- */
    sci_regs->regs->SCR.BIT.RE = 0;

    while (0 != (sci_regs->regs->SCR.BIT.RE))
    {
        /* Confirm that bit is actually 0 */
    }

    /* ---- Clear the error flag ---- */
    sci_regs->regs->SSR.BYTE = (sci_regs->regs->SSR.BYTE & ~SSR_ERROR_FLAGS) | 0xC0;

    while (0x00 != (sci_regs->regs->SSR.BYTE & SSR_ERROR_FLAGS))
    {
        /* ---- Clear the error flag ---- */
        sci_regs->regs->SSR.BYTE = (sci_regs->regs->SSR.BYTE & ~SSR_ERROR_FLAGS) | 0xC0;
    }

    /* ---- Disable ERI and RXI interrupt requests flag ---- */
    sci_regs->regs->SCR.BIT.RIE = 0;

    while (0 != (sci_regs->regs->SCR.BIT.RIE))
    {
        /* Confirm that bit is actually 0 */
    }

    /* RXI interrupt request is cleared */
    *sci_regs->ir_rxi = 0;

    /* ---- Clear the reception busy flag ---- */
    SCI_B_RX_BUSY = 0;

    LED2 = LED_ON;                          /* LED2 ON (SCI receive error) */

}

/*******************************************************************************
* Outline      : Transmit buffer empty interrupt
* Header       : none
* Function Name: sci_tran_buffer_empty
* Description  : Called in TXI interrupt processing. Write transmit data.
*                When transmit the last data, Disable TXI and enable TEI interrupt request.
* Arguments    : sci_ch -
*                   Used SCI channel number
* Return Value : none
*******************************************************************************/
static void sci_tran_buffer_empty (uint8_t sci_ch)
{
    const sci_ch_iodefine_t *sci_regs = &sci_def[sci_ch];

    /* Check TXI interrupt request */
    if (1== (*sci_regs->ir_txi))
    {
        /* TXI interrupt request is cleared */
        *sci_regs->ir_txi = 0;

        /* ---- Write transmit data to TDR register ---- */
        sci_regs->regs->TDR = *pbuf_tx;

        if (0 == (sci_regs->regs->SSR.BIT.TEND))
        {
            /* Dummy read the SSR register */
        }

        pbuf_tx++;                      /* Update of the transmit buffer pointer */
        tx_cnt--;                       /* Downcount of the transmission counter */

        /* ---- When transmit the last data ---- */
        if (0 == tx_cnt)
        {
            /* ---- Disable TXI interrupt request ---- */
            sci_regs->regs->SCR.BIT.TIE = 0;

            while (0 != (sci_regs->regs->SCR.BIT.TIE))
            {
                /* Confirm that bit is actually 0 */
            }

            /* ---- TXI interrupt request is cleared ---- */
            *sci_regs->ir_txi = 0;

            /* ---- Enable TEI interrupt request ---- */
            sci_regs->regs->SCR.BIT.TEIE = 1;
        }
    }
}

/*******************************************************************************
* Outline      : Transmit end interrupt
* Header       : none
* Function Name: sci_tran_end
* Description  : Called in TEI interrupt processing. Disable serial transmission.
*                Call the Callback function (SCI transmission end).
* Arguments    : sci_ch -
*                   Used SCI channel number
* Return Value : none
*******************************************************************************/
static void sci_tran_end (uint8_t sci_ch)
{
    const sci_ch_iodefine_t *sci_regs = &sci_def[sci_ch];

    /* Check TEI interrupt request */
    if (1== (*sci_regs->ir_tei))
    {
        /* ---- Use pin as general I/O port ---- */
        *sci_regs->txd_port_md &= ~(sci_regs->txd_bit);

        /* ---- Disable serial transmission ---- */

        /* Serial transmission is disabled */
        sci_regs->regs->SCR.BIT.TE = 0;

        /* A TXI interrupt request is disabled */
        sci_regs->regs->SCR.BIT.TIE = 0;

        /* ---- Disable TEI interrupt request ---- */
        sci_regs->regs->SCR.BIT.TEIE = 0;

        while (0 != (sci_regs->regs->SCR.BIT.TEIE))
        {
            /* Confirm that bit is actually 0 */
        }

        /* ---- Clear the transmission busy flag ---- */
        SCI_B_TX_BUSY = 0;

        LED0 = ~LED0;                       /* LED0 ON (SCI transmission end) */
    }
}

/* End of File */
