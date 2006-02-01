/* 
 * Proview   $Id: pb_conf.h,v 1.2 2006-02-01 08:35:51 claes Exp $
 * Copyright (C) SOFTING GmbH 1995-1999.
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*****************************************************************************/
/*                                                                           */
/*                              SOFTING GmbH                                 */
/*                        Richard-Reitzner-Allee 6                           */
/*                              D-85540 Haar                                 */
/*                      Phone: (++49)-(0)89-45656-0                          */
/*                      Fax:   (++49)-(0)89-45656-399                        */
/*                                                                           */
/*                     Copyright (C) SOFTING GmbH 1995-1999                  */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*           PROFIBUS CONFIGURATION AND IMPLEMENTATION DEFINITIONS           */
/*                                                                           */
/*  Filename    : PB_CONF.H                                                  */
/*  Version     : 5.22.0.00.release                                          */
/*  Date        : 26-February-1999                                           */
/*  Author      : SOFTING-BG2                                                */
/*                                                                           */
/*  Description : This file contains the PROFIBUS configuration and          */
/*                implementation definitions                                 */
/*                                                                           */
/*  CHANGE_NOTES                                                             */
/*                                                                           */
/*  date      name      change                                               */
/*  -----------------------------------------------------------------------  */
/*  18.03.97  BG2-BOE   modify compiler switches                             */
/*  18.03.97  BG2-BOE   rename #define PAGE_SIZE to #define DPR_PAGE_SIZE    */
/*  18.03.97  BG2-BOE   rename #define PB_VER 500 to #define PB_VER 511      */
/*  19.06.97  BG2-BOE   correct macro _NAME_LENGTH                           */
/*  22.07.97  BG2-BOE   remove #pragma pack(2)                               */
/*  22.07.97  BG2-BOE   modify #define CMI_TIMEOUT                           */
/*  23.07.97  BG2-BOE   add #define LOCK_UNLOCK_TIMEOUT                      */
/*  23.07.97  BG2-BOE   add defines for supported services                   */
/*  15.09.97  BG2-BOE   add new IRQ identifiers                              */
/*  16.09.97  BG2-BOE   add new define EXPORT                                */
/*  06.02.98  BG2-BOE   set MAX_COMREF 48 to MAX_COMREF 64                   */
/*  06.02.98  BG2-BOE   set #define CMI_V400 0xA1                            */
/*  06.02.98  BG2-BOE   set #define PB_VER 520 to #define PB_VER 521         */
/*  26.02.99  BG2-BOE   set #define PB_VER 521 to #define PB_VER 522         */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#ifndef __PB_CONF__
#define __PB_CONF__

/*****************************************************************************/
/* COMPILER SWITCHES FOR COMPABLITY                                          */
/*****************************************************************************/


#ifndef PB_VER
/* --- for new applications running with PROFIBUS V500 --------------------- */
/* --- NOTE: For applications developed with PROFIBUS V4XX invoke compiler   */
/*          with compiler switch "/DPB_VER=400"                              */
#define PB_VER  522
#endif


/*****************************************************************************/
/* COMPILER SWITCHES DEPENDING ON OPERATING SYSTEM                           */
/*****************************************************************************/

#undef FAR
#undef HUGE

#if defined (WIN32) || defined (_WIN32)                /* compiling as WIN32 */
   #undef  DOS16
   #undef  WIN32
   #undef  EXPORT
   #define WIN32
   #define EXPORT  __export
   #define FAR
   #define HUGE
   #define CALL_CONV  APIENTRY                /* call convention using WIN32 */
   #ifdef PB_API_FUNC_NOT_USED
      #undef  CALL_CONV
      #define CALL_CONV                      /* call convention using NT-DDK */
   #endif
#else                                                  /* compiling as WIN16 */
   #if defined (WIN_DLL) || defined (_WINDOWS) || defined (_WINDLL)
       #undef  DOS16
       #undef  WIN16
       #define WIN16
   #endif

   #if defined (WIN16) || defined (_WIN16)
       #undef  DOS16
       #undef  WIN16
       #undef  EXPORT
       #define WIN16
       #define EXPORT  __export
       #define FAR  __far
       #define HUGE __huge
       #define CALL_CONV FAR pascal         /* calling convention using WIN16 */
   #endif
#endif


#if !defined (WIN16) && !defined (WIN32)                /* compiling as DOS16 */
  #undef  DOS16
  #undef  EXPORT
  #define DOS16
  #define EXPORT
  #define FAR  __far
  #define HUGE __huge
  #define CALL_CONV
#endif

/*****************************************************************************/
/*              SUPPORTED SERVICES                                           */
/*****************************************************************************/
#define FMS_SERVICES_SUPPORTED
#define DP_SERVICES_SUPPORTED


/*****************************************************************************/
/*              HW  Implementation Constants                                 */
/*****************************************************************************/
/* --- DPR access mode ---------------------------------------------------- */
#define USING_8BIT                      1
#define USING_16BIT_LA                  2
#define USING_16BIT_SA                  3
#define DPR_ACCESS_MODE                 USING_16BIT_SA

/* --- DPR page size ------------------------------------------------------ */
#define USING_64K_PAGE                  0
#define USING_16K_PAGES                 1
#define DPR_PAGE_SIZE                   USING_16K_PAGES

/* --- IRQ line to host --------------------------------------------------- */
#define NO_INT                          0x00           /* no PC-IRQ is used */
#define INT_5                           0x10                    /* PC-IRQ 5 */
#define INT_10                          0x20                   /* PC-IRQ 10 */
#define INT_11                          0x30                   /* PC-IRQ 11 */
#define INT_12                          0x40                   /* PC-IRQ 12 */
#define INT_15                          0x50                   /* PC-IRQ 15 */

#define NO_INT_TO_HOST                  NO_INT
#define H1_INT_5                        INT_5
#define H1_INT_10                       INT_10
#define H1_INT_11                       INT_11
#define H1_INT_12                       INT_12
#define H1_INT_15                       INT_15

#define INT_TO_HOST                     NO_INT


/*****************************************************************************/
/*              CMI Implementation Constants                                 */
/*****************************************************************************/

/* --- timeouts to wait for controller response ---------------------------- */
#define CMI_TIMEOUT           (03*19)           /* CMI timeout in 55ms units */
#define LOCK_UNLOCK_TIMEOUT        2 /* Lock/Unlock Data image in 55ms units */

/* --- host identifiers ---------------------------------------------------- */
#define CMI_V410                     0xA1    /* host driver CMI version 4.10 */
#define HOST_TYPE                    CMI_V410

/* --- controller identifier ----------------------------------------------- */
#define ID_PROFI_BOARD               0xCA                      /* PROFIboard */
#define ID_PROFI_104                 0xCA                        /* PROFI104 */
#define ID_PROFI_CARD                0xBA                       /* PROFIcard */
#define TYPE_PCMCIA                  0xBA                   /* old PROFIcard */

#define CONTROLLER_TYPE              ID_PROFI_BOARD

/* --- polling or interrupt mode --------------------------------------------*/
#define REQ_ACK_BY_POLLING             0     /* request and ackn. by polling */
#define REQ_BY_IR_ACK_BY_POLLING       1  /* request by IR, ackn. by polling */
#define REQ_BY_POLLING_ACK_BY_IR       2  /* request by polling, ackn. by IR */
#define REQ_ACK_BY_IR                  3    /* request and acknowledge by IR */

#define POLL_INT_MODE                REQ_ACK_BY_POLLING

/* --- address mode -------------------------------------------------------- */
#define ABS_32BIT_MOTOROLA             0  /* linear absolute 32 bit Motorola */
#define ABS_16BIT_MOTOROLA             1  /* linear absolute 16 bit Motorola */
#define REL_32BIT_MOTOROLA             2  /* linear relative 32 bit Motorola */
#define REL_16BIT_MOTOROLA             3  /* linear relative 16 bit Motorola */

#define ABS_32BIT_INTEL                8     /* linear absolute 32 bit Intel */
#define ABS_16BIT_INTEL                9     /* linear absolute 16 bit Intel */
#define REL_32BIT_INTEL               10     /* linear relative 32 bit Intel */
#define REL_16BIT_INTEL               11     /* linear relative 32 bit Intel */
#define SEGMENT_OFFSET_INTEL          12            /* SEGEMENT/OFFSET Intel */

#define ADDRESS_SWAP_MODE            SEGMENT_OFFSET_INTEL


/*****************************************************************************/
/*                        Implementation Constants                           */
/*                                                                           */
/* The constants given below define the sizes of various data structures in  */
/* the protocol software and thus influence memory consumption.              */
/*                                                                           */
/* NOTE: Do not change the following constants without recompiling the       */
/*       the protocol software on the communication controller               */
/*****************************************************************************/

#define VERSION_STRING_LENGTH        100  /* length of version string buffer */

/* -- constants of internal sizes of byte arrays --------------------------- */

#define VFD_STRING_LENGTH             32     /* max length of the VFD string */
#define IDENT_STRING_LENGTH           32   /* max length of the Ident string */

#define ACCESS_NAME_LENGTH            32    /* max length for name adressing */
#define OBJECT_NAME_LENGTH            32        /* max length of object name */
#define EXTENSION_LENGTH              32   /* max length of object extension */
#define EXECUTION_ARGUMENT_LENGTH     32     /* max length of exec. argument */
#define ERROR_DESCR_LENGTH            32     /* max length of error descript.*/
#define CRL_SYMBOL_LENGTH             32    /* max length of crl symbol name */
#define CRL_EXTENSION_LENGTH           2      /* max length of crl extension */

#if (PB_VER < 500)
#define KBL_SYMBOL_LENGTH             CRL_SYMBOL_LENGTH
#define KBL_EXTENSION_LENGTH          CRL_EXTENSION_LENGTH
#endif

#define MAX_FMS_PDU_LENGTH      241    /* max size of the FMS/FM7-PDU-Buffer */
#define MAX_VAR_LIST_ELEMENTS    50   /* max count of variable list elements */
#define MAX_DOM_LIST_ELEMENTS    50     /* max count of domain list elements */
#define MAX_VAR_RECORD_ELEMENTS  10          /* max count of record elements */

#define MAX_COMREF            64   /* max supported communication references */
#define MAX_VFD                5                       /* max supported VFDs */

#if (PB_VER < 500)
#define MAX_KBL_LEN            MAX_COMREF              /* max entries in CRL */
#define MAX_PARA_LOC_SERVICES          5   /* max parallel local FMS-Services*/
#endif

/*****************************************************************************/
/* USEFUL MACROS                                                             */
/*****************************************************************************/

/*****************************************************************************/
/* MACRO TO CALCULATE MAX_xxxx_NAME_LENGTH                                   */
/*                                                                           */
/* This macro calculates the internal sizes of byte arrays in a way that the */
/* desired alignment on byte, word or long word boundaries is achieved.      */
/* The alignment is specified by the constant ALIGNMENT (e. g. longword = 4) */
/*                                                                           */
/*****************************************************************************/

#define ALIGNMENT                  0x02        /* alignment on word boundary */

#define _NAME_LENGTH(length) ((length) + ALIGNMENT - ((length) % ALIGNMENT))

#endif

