/*****************************************************************************/
/*                                                                           */
/*                                SOFTING AG                                 */
/*                        Richard-Reitzner-Allee 6                           */
/*                              D-85540 Haar                                 */
/*                      Phone: (++49)-(0)89-45656-0                          */
/*                      Fax:   (++49)-(0)89-45656-399                        */
/*                                                                           */
/*                    Copyright (C) SOFTING AG 1995-2003                     */
/*                            All Rights Reserved                            */
/*                                                                           */
/*****************************************************************************/
/*****************************************************************************/
/*                          PROFIBUS BASIC TYPES                             */
/*                                                                           */
/*  Filename    : PB_TYPE.H                                                  */
/*  Version     : 5.22.0.00.release                                          */
/*  Date        : 26-February-1999                                           */
/*  Author      : SOFTING AG                                                 */
/*                                                                           */
/*  Description : This file contains the PROFIBUS basic types                */
/*                                                                           */
/*****************************************************************************/

#ifndef __PB_TYPE__
#define __PB_TYPE__

#include "pb_conf.h"                        /* PAPI configuration parameters */

#ifndef NULL
   #ifdef __cplusplus
       #define NULL    0
   #else
       #define NULL ((void *)0)
   #endif
#endif


/* --- global type definitions --------------------------------------------- */
#ifndef VOID
#define     VOID                 void
#endif

typedef     unsigned char        PB_BOOL;
#if (PB_VER < 500)
#undef      BOOL
#define     BOOL                 PB_BOOL
#endif

typedef     unsigned char        OCTET;
typedef     unsigned char        USIGN8;
typedef     unsigned short       USIGN16;
typedef     unsigned long        USIGN32;

typedef     signed char          INT8;
typedef     signed short         INT16;

#ifdef WIN32
typedef     int                  INT32;
#else
typedef     signed long          INT32;
#endif

#ifdef _LINUX
typedef     unsigned int         UINT;
#endif

typedef     float                FLOAT;
typedef     double               DOUBLE;

typedef     char                 STRINGV;

#ifndef PB_CSTRING_NOT_USED
typedef     char                 CSTRING;
#endif


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

