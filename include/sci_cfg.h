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
* File Name    : sci_cfg.h
* Version      : Ver 1.00
* Device       : R5F563NBDDFC (RX63N Group)
* Abstract     : Configuration of serial control program
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

/* Guards against multiple inclusion */
#ifndef SCI_CFG_H
#define SCI_CFG_H

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define SCI0_PSEL           (0)     /* 0 : TXD0 - P20, RXD0 - P21
                                       1 : TXD0 - P32, RXD0 - P33 */
#define SCI1_PSEL           (0)     /* 0 : TXD1 - P16, RXD1 - P15
                                       1 : TXD1 - P26, RXD1 - P30
                                       2 : TXD1 - PF0, RXD1 - PF2 */
#define SCI2_PSEL           (0)     /* 0 : TXD2 - P13, RXD2 - P12
                                       1 : TXD2 - P50, RXD2 - P52 */
#define SCI3_PSEL           (0)     /* 0 : TXD3 - P17, RXD3 - P16
                                       1 : TXD3 - P23, RXD3 - P25 */
#define SCI5_PSEL           (0)     /* 0 : TXD5 - PA4, RXD5 - PA2
                                       1 : TXD5 - PA4, RXD5 - PA3
                                       2 : TXD5 - PC3, RXD5 - PC2 */
#define SCI6_PSEL           (0)     /* 0 : TXD6 - P00, RXD6 - P01
                                       1 : TXD6 - P32, RXD6 - P33
                                       2 : TXD6 - PB1, RXD6 - PB0 */

/* SCI0 */
const sci_ch_iodefine_t sci_def[13] = {

    {(volatile __evenaccess struct st_sci12 *)&SCI0,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRB.LONG, 0x80000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI0_RXI0].BYTE,
     &ICU.IR[IR_SCI0_TXI0].BYTE,
     &ICU.IR[IR_SCI0_TEI0].BYTE,

#if SCI0_PSEL == 0
     /* Port (TXD) - P20 */
     &PORT2.PODR.BYTE, &PORT2.PDR.BYTE, &PORT2.PMR.BYTE, (1<<0),
     &MPC.P20PFS.BYTE, 0x0A,

     /* Port (RXD) - P21 */
     &PORT2.PDR.BYTE, &PORT2.PMR.BYTE, (1<<1),
     &MPC.P21PFS.BYTE, 0x0A

#else
     /* Port (TXD) - P32 */
     &PORT3.PODR.BYTE, &PORT3.PDR.BYTE, &PORT3.PMR.BYTE, (1<<2),
     &MPC.P32PFS.BYTE, 0x0B,

     /* Port (RXD) - P33 */
     &PORT3.PDR.BYTE, &PORT3.PMR.BYTE, (1<<3),
     &MPC.P33PFS.BYTE, 0x0B
#endif
    },

/* SCI1 */
    {(volatile __evenaccess struct st_sci12 *)&SCI1,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRB.LONG, 0x40000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI1_RXI1].BYTE,
     &ICU.IR[IR_SCI1_TXI1].BYTE,
     &ICU.IR[IR_SCI1_TEI1].BYTE,

#if SCI1_PSEL == 0
     /* Port (TXD) - P16 */
     &PORT1.PODR.BYTE, &PORT1.PDR.BYTE, &PORT1.PMR.BYTE, (1<<6),
     &MPC.P16PFS.BYTE, 0x0A,

     /* Port (RXD) - P15 */
     &PORT1.PDR.BYTE, &PORT1.PMR.BYTE, (1<<5),
     &MPC.P15PFS.BYTE, 0x0A

#elif SCI1_PSEL == 1
     /* Port (TXD) - P26 */
     &PORT2.PODR.BYTE, &PORT2.PDR.BYTE, &PORT2.PMR.BYTE, (1<<6),
     &MPC.P26PFS.BYTE, 0x0A,

     /* Port (RXD) - P30 */
     &PORT3.PDR.BYTE, &PORT3.PMR.BYTE, (1<<0),
     &MPC.P30PFS.BYTE, 0x0A

#else
     /* Port (TXD) - PF0 */
     &PORTF.PODR.BYTE, &PORTF.PDR.BYTE, &PORTF.PMR.BYTE, (1<<0),
     &MPC.PF0PFS.BYTE, 0x0A,

     /* Port (RXD) - PF2 */
     &PORTF.PDR.BYTE, &PORTF.PMR.BYTE, (1<<2),
     &MPC.PF2PFS.BYTE, 0x0A
#endif
    },

    /* SCI2 */
    {(volatile __evenaccess struct st_sci12 *)&SCI2,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRB.LONG, 0x20000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI2_RXI2].BYTE,
     &ICU.IR[IR_SCI2_TXI2].BYTE,
     &ICU.IR[IR_SCI2_TEI2].BYTE,

#if SCI2_PSEL == 0
     /* Port (TXD) - P13 */
     &PORT1.PODR.BYTE, &PORT1.PDR.BYTE, &PORT1.PMR.BYTE, (1<<3),
     &MPC.P13PFS.BYTE, 0x0A,

     /* Port (RXD) - P12 */
     &PORT1.PDR.BYTE, &PORT1.PMR.BYTE, (1<<2),
     &MPC.P12PFS.BYTE, 0x0A

#else
     /* Port (TXD) - P50 */
     &PORT5.PODR.BYTE, &PORT5.PDR.BYTE, &PORT5.PMR.BYTE, (1<<0),
     &MPC.P50PFS.BYTE, 0x0A,

     /* Port (RXD) - P52 */
     &PORT5.PDR.BYTE, &PORT5.PMR.BYTE, (1<<2),
     &MPC.P52PFS.BYTE, 0x0A
#endif

    },

    /* SCI3 */
    {(volatile __evenaccess struct st_sci12 *)&SCI3,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRB.LONG, 0x10000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI3_RXI3].BYTE,
     &ICU.IR[IR_SCI3_TXI3].BYTE,
     &ICU.IR[IR_SCI3_TEI3].BYTE,

#if SCI3_PSEL == 0
     /* Port (TXD) - P17 */
     &PORT1.PODR.BYTE, &PORT1.PDR.BYTE, &PORT1.PMR.BYTE, (1<<7),
     &MPC.P17PFS.BYTE, 0x0B,

     /* Port (RXD) - P16 */
     &PORT1.PDR.BYTE, &PORT1.PMR.BYTE, (1<<6),
     &MPC.P16PFS.BYTE, 0x0B

#else
     /* Port (TXD) - P23 */
     &PORT2.PODR.BYTE, &PORT2.PDR.BYTE, &PORT2.PMR.BYTE, (1<<3),
     &MPC.P23PFS.BYTE, 0x0A,

     /* Port (RXD) - P25 */
     &PORT2.PDR.BYTE, &PORT2.PMR.BYTE, (1<<5),
     &MPC.P25PFS.BYTE, 0x0A
#endif
    },

    /* SCI4 */
    {(volatile __evenaccess struct st_sci12 *)&SCI4,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRB.LONG, 0x08000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI4_RXI4].BYTE,
     &ICU.IR[IR_SCI4_TXI4].BYTE,
     &ICU.IR[IR_SCI4_TEI4].BYTE,

     /* Port (TXD) - PB1 */
     &PORTB.PODR.BYTE, &PORTB.PDR.BYTE, &PORTB.PMR.BYTE, (1<<1),
     &MPC.PB1PFS.BYTE, 0x0A,

     /* Port (RXD) - PB0 */
     &PORTB.PDR.BYTE, &PORTB.PMR.BYTE, (1<<0),
     &MPC.PB0PFS.BYTE, 0x0A
    },

    /* SCI5 */
    {(volatile __evenaccess struct st_sci12 *)&SCI5,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRB.LONG, 0x04000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI5_RXI5].BYTE,
     &ICU.IR[IR_SCI5_TXI5].BYTE,
     &ICU.IR[IR_SCI5_TEI5].BYTE,

#if SCI5_PSEL == 0
     /* Port (TXD) - PA4 */
     &PORTA.PODR.BYTE, &PORTA.PDR.BYTE, &PORTA.PMR.BYTE, (1<<4),
     &MPC.PA4PFS.BYTE, 0x0A,

     /* Port (RXD) - PA2 */
     &PORTA.PDR.BYTE, &PORTA.PMR.BYTE, (1<<2),
     &MPC.PA2PFS.BYTE, 0x0A

#elif SCI5_PSEL == 1
     /* Port (TXD) - PA4 */
     &PORTA.PODR.BYTE, &PORTA.PDR.BYTE, &PORTA.PMR.BYTE, (1<<4),
     &MPC.PA4PFS.BYTE, 0x0A,

     /* Port (RXD) - PA3 */
     &PORTA.PDR.BYTE, &PORTA.PMR.BYTE, (1<<3),
     &MPC.PA3PFS.BYTE, 0x0A

#else
     /* Port (TXD) - PC3 */
     &PORTC.PODR.BYTE, &PORTC.PDR.BYTE, &PORTC.PMR.BYTE, (1<<3),
     &MPC.PC3PFS.BYTE, 0x0A,

     /* Port (RXD) - PC2 */
     &PORTC.PDR.BYTE, &PORTC.PMR.BYTE, (1<<2),
     &MPC.PC2PFS.BYTE, 0x0A
#endif
    },

    /* SCI6 */
    {(volatile __evenaccess struct st_sci12 *)&SCI6,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRB.LONG, 0x02000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI6_RXI6].BYTE,
     &ICU.IR[IR_SCI6_TXI6].BYTE,
     &ICU.IR[IR_SCI6_TEI6].BYTE,

#if SCI6_PSEL == 0
     /* Port (TXD) - P00 */
     &PORT0.PODR.BYTE, &PORT0.PDR.BYTE, &PORT0.PMR.BYTE, (1<<0),
     &MPC.P00PFS.BYTE, 0x0A,

     /* Port (RXD) - P01 */
     &PORT0.PDR.BYTE, &PORT0.PMR.BYTE, (1<<1),
     &MPC.P01PFS.BYTE, 0x0A

#elif SCI6_PSEL == 1
     /* Port (TXD) - P32 */
     &PORT3.PODR.BYTE, &PORT3.PDR.BYTE, &PORT3.PMR.BYTE, (1<<2),
     &MPC.P32PFS.BYTE, 0x0A,

     /* Port (RXD) - P33 */
     &PORT3.PDR.BYTE, &PORT3.PMR.BYTE, (1<<3),
     &MPC.P33PFS.BYTE, 0x0A

#else
     /* Port (TXD) - PB1 */
     &PORTB.PODR.BYTE, &PORTB.PDR.BYTE, &PORTB.PMR.BYTE, (1<<1),
     &MPC.PB1PFS.BYTE, 0x0B,

     /* Port (RXD) - PB0 */
     &PORTB.PDR.BYTE, &PORTB.PMR.BYTE, (1<<0),
     &MPC.PB0PFS.BYTE, 0x0B
#endif
    },

    /* SCI7 */
    {(volatile __evenaccess struct st_sci12 *)&SCI7,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRB.LONG, 0x01000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI7_RXI7].BYTE,
     &ICU.IR[IR_SCI7_TXI7].BYTE,
     &ICU.IR[IR_SCI7_TEI7].BYTE,

     /* Port (TXD) - P90 */
     &PORT9.PODR.BYTE, &PORT9.PDR.BYTE, &PORT9.PMR.BYTE, (1<<0),
     &MPC.P90PFS.BYTE, 0x0A,

     /* Port (RXD) - P92 */
     &PORT9.PDR.BYTE, &PORT9.PMR.BYTE, (1<<2),
     &MPC.P92PFS.BYTE, 0x0A
    },

    /* SCI8 */
    {(volatile __evenaccess struct st_sci12 *)&SCI8,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRC.LONG, 0x08000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI8_RXI8].BYTE,
     &ICU.IR[IR_SCI8_TXI8].BYTE,
     &ICU.IR[IR_SCI8_TEI8].BYTE,

     /* Port (TXD) - PC7 */
     &PORTC.PODR.BYTE, &PORTC.PDR.BYTE, &PORTC.PMR.BYTE, (1<<7),
     &MPC.PC7PFS.BYTE, 0x0A,

     /* Port (RXD) - PC6 */
     &PORTC.PDR.BYTE, &PORTC.PMR.BYTE, (1<<6),
     &MPC.PC6PFS.BYTE, 0x0A
    },

    /* SCI9 */
    {(volatile __evenaccess struct st_sci12 *)&SCI9,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRC.LONG, 0x04000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI9_RXI9].BYTE,
     &ICU.IR[IR_SCI9_TXI9].BYTE,
     &ICU.IR[IR_SCI9_TEI9].BYTE,

     /* Port (TXD) - PB7 */
     &PORTB.PODR.BYTE, &PORTB.PDR.BYTE, &PORTB.PMR.BYTE, (1<<7),
     &MPC.PB7PFS.BYTE, 0x0A,

     /* Port (RXD) - PB6 */
     &PORTB.PDR.BYTE, &PORTB.PMR.BYTE, (1<<6),
     &MPC.PB6PFS.BYTE, 0x0A
     },

    /* SCI10 */
    {(volatile __evenaccess struct st_sci12 *)&SCI10,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRC.LONG, 0x02000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI10_RXI10].BYTE,
     &ICU.IR[IR_SCI10_TXI10].BYTE,
     &ICU.IR[IR_SCI10_TEI10].BYTE,

     /* Port (TXD) - P82 */
     &PORT8.PODR.BYTE, &PORT8.PDR.BYTE, &PORT8.PMR.BYTE, (1<<2),
     &MPC.P82PFS.BYTE, 0x0A,

     /* Port (RXD) - P81 */
     &PORT8.PDR.BYTE, &PORT8.PMR.BYTE, (1<<1),
     &MPC.P81PFS.BYTE, 0x0A
    },

    /* SCI11 */
    {(volatile __evenaccess struct st_sci12 *)&SCI11,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRC.LONG, 0x01000000,

     /* Interrupt Register */
     &ICU.IR[IR_SCI11_RXI11].BYTE,
     &ICU.IR[IR_SCI11_TXI11].BYTE,
     &ICU.IR[IR_SCI11_TEI11].BYTE,

     /* Port (TXD) - P77 */
     &PORT7.PODR.BYTE, &PORT7.PDR.BYTE, &PORT7.PMR.BYTE, (1<<7),
     &MPC.P77PFS.BYTE, 0x0A,

     /* Port (RXD) - P76 */
     &PORT7.PDR.BYTE, &PORT7.PMR.BYTE, (1<<6),
     &MPC.P76PFS.BYTE, 0x0A
    },

    /* SCI12 */
    {(volatile __evenaccess struct st_sci12 *)&SCI12,
     /* Module stop Register and bit */
     &SYSTEM.MSTPCRB.LONG, 0x00000010,

     /* Interrupt Register */
     &ICU.IR[IR_SCI12_RXI12].BYTE,
     &ICU.IR[IR_SCI12_TXI12].BYTE,
     &ICU.IR[IR_SCI12_TEI12].BYTE,

     /* Port (TXD) - PE1 */
     &PORTE.PODR.BYTE, &PORTE.PDR.BYTE, &PORTE.PMR.BYTE, (1<<1),
     &MPC.PE1PFS.BYTE, 0x0C,

     /* Port (RXD) - PE2 */
     &PORTE.PDR.BYTE, &PORTE.PMR.BYTE, (1<<2),
     &MPC.PE2PFS.BYTE, 0x0C
    }
};

#endif /* SCI_CFG_H */

/* End of File */
