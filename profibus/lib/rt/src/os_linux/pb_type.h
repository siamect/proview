/*****************************************************************************/
/*                                                                           */
/*                              SOFTING GmbH                                 */
/*                        Richard-Reitzner-Allee 6                           */
/*                              D-85540 Haar                                 */
/*                      Phone: (++49)-(0)89-45656-0                          */
/*                      Fax:   (++49)-(0)89-45656-399                        */
/*                                                                           */
/*                    Copyright (C) SOFTING GmbH 1995-1999                   */
/*                            All Rights Reserved                            */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                          PROFIBUS BASIC TYPES                             */
/*                                                                           */
/*  Filename    : PB_TYPE.H                                                  */
/*  Version     : 5.22.0.00.release                                          */
/*  Date        : 26-February-1999                                           */
/*  Author      : SOFTING-BG2                                                */
/*                                                                           */
/*  Description : This file contains the PROFIBUS basic types                */
/*                                                                           */
/*  CHANGE_NOTES                                                             */
/*                                                                           */
/*  date      name      change                                               */
/*  -----------------------------------------------------------------------  */
/*  18.03.97  BG2-BOE   include header file 'pb_conf.h'                      */
/*  18.03.97  BG2-BOE   remove #define FAR and #define HUGE                  */
/*  02.02.99  BG2-BOE   define INT32 as int using WIN32                      */
/*                      (compatibility to MSVC60)                            */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#ifndef __PB_TYPE__
#define __PB_TYPE__

#include "pb_conf.h"                        /* PAPI configuration parameters */

/* --- global type definitions --------------------------------------------- */
#ifndef VOID
#define     VOID                 void
#endif

#define     PB_BOOL	unsigned char
#if (PB_VER < 500)
#undef      BOOL
#define     BOOL        PB_BOOL
#endif

#define     OCTET 	unsigned char        
#define     USIGN8 	unsigned char
#define     USIGN16 	unsigned short
#define     USIGN32 	unsigned int

#define     INT8 	signed char
#define     INT16	signed short
#define     INT32	signed int

#define     FLOAT	float
#define     DOUBLE	double

#define     STRINGV	char
#define     CSTRING	char


/* --- PROFIBUS true and false definitions --------------------------------- */
#define     PB_TRUE              0xFF
#define     PB_FALSE             0x00

#if (PB_VER < 500)
#undef      TRUE
#undef      FALSE
#define     TRUE                 PB_TRUE
#define     FALSE                PB_FALSE
#endif

/* --- PROFIBUS function declarations -------------------------------------- */
#define     LOCAL                static
#define     PUBLIC
#define     GLOBAL

#endif


