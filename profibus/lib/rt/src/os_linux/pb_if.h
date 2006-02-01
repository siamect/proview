/* 
 * Proview   $Id: pb_if.h,v 1.2 2006-02-01 08:35:51 claes Exp $
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
/**********************************************************************************/
/*                                                                                */
/*                                  SOFTING GmbH                                  */
/*                            Richard-Reitzner-Allee 6                            */
/*                                 D-85540 Haar                                   */
/*                          Phone: (++49)-(0)89-45656-0                           */
/*                          Fax:   (++49)-(0)89-45656-399                         */
/*                                                                                */
/*                    Copyright (C) SOFTING GmbH 1995-1999                        */
/*                                                                                */
/**********************************************************************************/
/**********************************************************************************/
/*                   PROFIBUS APPLICATION PROGRAM INTERFACE                       */
/*                                                                                */
/*  Filename    : PB_IF.H                                                         */
/*  Version     : 5.22.0.00.release                                               */
/*  Date        : 26-February-1999                                                */
/*  Author      : SOFTING-BG2                                                     */
/*                                                                                */
/*  Description : This file contains the global function declarations,            */
/*                defines and types of the PROFIBUS Communication Interface       */
/*                                                                                */
/*  CHANGE_NOTES                                                                  */
/*                                                                                */
/*  date      name      change                                                    */
/*  ------------------------------------------------------------------------      */
/*  18.03.97  BG2-BOE   use compiler switch PB_API_FUNC_NOT_USED                  */
/*                      to switch off PAPI-functions                              */
/*  22.07.97  BG2-BOE   remove layer identifier SM7 and SM7_USR                   */
/*  22.07.97  BG2-BOE   set #pragma pack(2) and reset to #pragma pack()           */
/*  24.09.97  BG2-BOE   add new identifiers for data description interface        */
/*                      - ID_SERIAL_DEVICE_NUMBER                                 */
/*  24.07.97  BG2-BOE   add new function prototypes                               */
/*                      - profi_get_serial_device_number()                        */
/*                      - profi_get_add_error()                                   */
/*  10.02.99  BG2-BOE   remove WIN32 function prototypes                          */
/**********************************************************************************/
/**********************************************************************************/
/**********************************************************************************/

#ifndef __PB_IF__
#define __PB_IF__

/*
#pragma warning (disable : 4103)
#ifdef WIN32
#pragma pack(push,2)
#else
#pragma pack(2)
#endif
#pragma warning (default : 4103)
*/

/**********************************************************************************/
/* INTERFACE DEFINES                                                              */
/**********************************************************************************/
/* --- contants for IRQ values -(only used for 16BIT-Interface)                   */
#define REQ_IRQ                  0xF0                  /* IND/CON IRQ is received */
#define ACK_IRQ                  0x0F                      /* acknowledge REQ/RES */
#define DP_SLAVE_IO_REQ_IRQ      0xE0              /* MASTER: DP-SLAVE-IO IND/REQ */
#define DP_SLAVE_IO_ACK_IRQ      0x0E  /* MASTER: DP-SLAVE-IO acknowldege IND/REQ */
#define DP_DATA_STOP_REQ_IRQ     0xD0  /* MASTER: data transfer stop REQ received */
#define DP_DATA_STOP_ACK_IRQ     0x0D   /* MASTER: data transfer stop Acknowldege */
#define EXCEPTION_REQ_IRQ        0xB0                        /* Exception Request */
#define EXCEPTION_ACK_IRQ        0x0B                    /* Exception Acknowledge */


/* --- contants to receive an indication or confirmation ------------------------ */
#define CON_IND_RECEIVED    0x0001              /* ind. or con. has been received */
#define NO_CON_IND_RECEIVED 0x0000                   /* nothing has been received */

/* --- values > CON_IND_RECEIVED are error returns (see pb_err.h)  -------------- */

/* --- OK return value ---------------------------------------------------------- */
#define E_OK                0                                        /* no errors */
/* --- values != E_OK are error returns (see pb_err.h) -------------------------- */

/* --- service result ----------------------------------------------------------- */
#define NEG                 0x01                  /* result for negative response */
#define POS                 0x00                  /* result for positive response */

/* --- service priority --------------------------------------------------------- */
#define LOW                 0x00                                 /* high priority */
#define HIGH                0x01                                  /* low priority */


/* --- service primitives ------------------------------------------------------- */
#define REQ                 0x00                             /* request primitive */
#define CON                 0x01                        /* confirmation primitive */
#define IND                 0x02                          /* indication primitive */
#define RES                 0x03                            /* response primitive */
#define RET                 0x04      /* return primitive not used by application */

/* --- layer identifiers -------------------------------------------------------- */
#define USR                 0x00                               /* identifier USER */
#define FMS                 0x01                                /* identifier FMS */
#define LLI                 0x02                                /* identifier LLI */
#define FDL                 0x03                                /* identifier FDL */
#define FM7                 0x04                                /* identifier FM7 */
#define FMB                 0x05                                /* identifier FMB */
#define FMS_USR             0x06                           /* identifier FMS-USER */
#define FM7_USR             0x07                           /* identifier FM7-USER */
#define FMB_USR             0x08                           /* identifier FMB-USER */
#define DP                  0x0B                                 /* identifier DP */
#define DP_USR              0x0C                            /* identifier DP-USER */
#define FDLIF               0x0D                              /* identifier FDLIF */
#define FDLIF_USR           0x0E                         /* identifier FDLIF-USER */

#define PAPI                0xF0               /* identifier Application-Layer-IF */

#if (PB_VER < 500)
#define FMA7                FM7                                /* identifier FMA7 */
#define FMA2                FMB                                /* identifier FMA2 */
#define FMA7_USR            FM7_USR                       /* identifier FMA7-USER */
#endif


/* --- identifiers for data description interface ------------------------------- */
/* --- 0x00 - 0x3F used by FMS -- (no identifiers defined) ---------------------- */

/* --- 0x40 - 0x7F reserved by management -- (no identifiers defined) ----------- */

/* --- 0x80 - 0x9F reserved by DP ----------------------------------------------- */
/* --- DP-MASTER ---------------------------------------------------------------- */
#define ID_DP_SLAVE_IO_IMAGE         0x80     /* MASTER: image for slave io datas */
#define ID_DP_STATUS_IMAGE           0x81       /* MASTER: image for status datas */

/* --- 0xA0 - 0xEF reserved for future use --- (no identifiers defined) --------- */

/* --- 0xF0 - 0xFF reserved for diagonsis  -------------------------------------- */
#define ID_EXCEPTION_IMAGE           0xF0             /* image for exeption datas */
#define ID_FW_VERS_IMAGE             0xF1           /* image for firmware version */
#define ID_SERIAL_DEVICE_NUMBER      0xF2      /* image for serial device number  */


/* --- idebtifier for end of data description list ------------------------------ */
#define EOF_DATA_DESCR_LIST          0x00         /* end of data description list */


/**********************************************************************************/
/* PROFI SERVICE DESCRIPTION BLOCK                                                */
/**********************************************************************************/

typedef struct _T_PROFI_SERVICE_DESCR
{
  USIGN16     comm_ref;                                /* communication reference */
  USIGN8      layer;                                          /* layer identifier */
  USIGN8      service;                                      /* service identifier */
  USIGN8      primitive;                                     /* service primitive */
  INT8        invoke_id;                                             /* invoke id */
  INT16       result;                              /* service result (POS or NEG) */
} T_PROFI_SERVICE_DESCR;

#endif

