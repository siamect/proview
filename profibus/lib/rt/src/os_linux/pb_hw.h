/*****************************************************************************/
/*                                                                           */
/*                              SOFTING GmbH                                 */
/*                        Richard-Reitzner-Allee 6                           */
/*                              D-85540 Haar                                 */
/*                      Phone: (++49)-(0)89-45656-0                          */
/*                      Fax:   (++49)-(0)89-45656-399                        */
/*                                                                           */
/*                  Copyright (C) SOFTING GmbH 1995-1999                     */
/*                            All Rights Reserved                            */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                         PROFIBoard SPECIFIC DEFINES                       */
/*  Filename    : PB_HW.H                                                    */
/*  Version     : 5.22.0.00.release                                          */
/*  Date        : 26-February-1999                                           */
/*  Author      : SOFTING-BG2                                                */
/*                                                                           */
/*  Description : This file contains the PROFIboard specific defines         */
/*                                                                           */
/*  CHANGE_NOTES                                                             */
/*                                                                           */
/*  date      name      change                                               */
/*  -----------------------------------------------------------------------  */
/*  03.03.98  BOE       new/modify #defines for DPRAM SIZES and IRQ-VAL      */
/*                                 offset in DPR                             */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#ifndef __PB_HW__
#define __PB_HW__

/* --- macros to access DPR ----------------------------------------------- */
#define _GET_DPR_BYTE(var)             var
#define _SET_DPR_BYTE(out_var,in_var)  out_var = in_var
#define _GET_DPR_WORD(var)             var
#define _SET_DPR_WORD(out_var,in_var)  out_var = in_var
#define _SET_CHIP_SELECT_ADDR(addr)

/* --- DPRAM SIZES / IRQ-VAL offset in DPR --------------------------------- */
#define DPRAM_SIZE_PROFIBOARD         0x4000                  /* 16 KB DPRAM */
#define DPRAM_SIZE_PROFI_104          0x4000                  /* 16 KB DPRAM */

#define DPRAM_H_TO_C_IRQ_OFFSET       0x2   /* IRQ memory cells at DPRAM end */
#define DPRAM_C_TO_H_IRQ_OFFSET       0x4

/* --- IRQ-VALUE ----------------------------------------------------------- */
#define RST_IRQ_VALUE              0xAA                             /* Reset */

#define REQ_IRQ_VALUE              0xF0                   /* SND/RCV Request */
#define ACK_IRQ_VALUE              0x0F               /* SND/RCV Acknowledge */
#define DP_SLAVE_IO_REQ_IRQ_VALUE  0xE0               /* DP-SLAVE-IO Request */
#define DP_SLAVE_IO_ACK_IRQ_VALUE  0x0E           /* DP-SLAVE-IO Acknowldege */
#define DP_DATA_STOP_REQ_IRQ_VALUE 0xD0        /* data transfer stop Request */
#define DP_DATA_STOP_ACK_IRQ_VALUE 0x0D    /* data transfer stop Acknowldege */


/* --- Read Registers in the I/O Area of the PC ---------------------------- */
#define IDENT_1      (IO_BASE_ADD + 0)              /* Identification Byte 1 */
#define IDENT_2      (IO_BASE_ADD + 1)              /* Identification Byte 2 */
#define IDENT_3      (IO_BASE_ADD + 2)              /* Identification Byte 3 */
#define IDENT_4      (IO_BASE_ADD + 3)              /* Identification Byte 4 */

/* --- Write Registers in the I/O Area of the PC --------------------------- */
#define BOARD_CTRL   (IO_BASE_ADD + 0)             /* Board Control Register */
#define BASE_ADD     (IO_BASE_ADD + 1)        /* DPRAM Base Address Register */
#define PAGE_SEL     (IO_BASE_ADD + 2)               /* Page Select Register */
#define CP_IRQ       (IO_BASE_ADD + 3)                 /* Interrupt Register */


/* -- Pindefinition of the Board Control Register -------------------------- */
#define C165_RESET      0x01                /* Reset of the C165 (low active)*/
#define EN_DPRAM        0x02             /* Enable DPRAM for PC (high active)*/
#define SIZE_16_64      0x04               /* DPRAM size 1:16 kByte 0:64kByte*/
                                                           /* 64 kByte (low) */
#define BASE_A14        0x08              /* Address low within the 64 kByte */
#define BASE_A15        0x10             /* Address high within the 64 kByte */
#define DPR_WIDTH_8_16  0x20                   /* DPRAM width 0:16Bit 1:8Bit */
#define MEMCS16_LA_SA   0x40                 /* MEMCS16 generation 0:LA 1:SA */

/* --- Pindefinition of the DPRAM Base Adress Register --------------------- */
/* Bit 7: Base Address 23 (MSB) to                                           */
/* Bit 0: Base Address 16 (LSB)                                              */

/* --- Pindefinition of the Page Select Register --------------------------- */
#define PAGE_A0      0x01     /* Page Address low within the 64 kByte window */
#define PAGE_A1      0x02    /* Page Address high within the 64 kByte window */

#define PAGE_0       0x00                              /* Page 0  0KB - 16KB */
#define PAGE_1       0x01                              /* Page 1 16KB - 32KB */
#define PAGE_2       0x02                              /* Page 2 32KB - 48KB */
#define PAGE_3       0x03                              /* Page 3 48KB - 64KB */


/* --- Macros to Set or Clear Page ----------------------------------------- */
#define CLEAR_PAGE(reg)         \
         ((reg) & ~(PAGE_A0 | PAGE_A1))

#define SET_PAGE(reg, page)     \
        (CLEAR_PAGE(reg) | page)


/* --- Board I/O identifier ------------------------------------------------ */

#define HW_IO_ID_PROFIBOARD_0           0x96
#define HW_IO_ID_PROFIBOARD_1           0x26
#define HW_IO_ID_PROFIBOARD_2           0x6B

#define HW_IO_ID_PROFI104_0             0x2C
#define HW_IO_ID_PROFI104_1             0x12
#define HW_IO_ID_PROFI104_2             0x41

#endif

