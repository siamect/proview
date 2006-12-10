/** 
 * Proview   $Id: co_time.h,v 1.8 2006-12-10 14:33:34 lw Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
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
 **/

#ifndef co_time_h
#define co_time_h

#ifndef pwr_h
# include "pwr.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* co_time.h -- Time utilities

   This include file contains definitions and function prototypes
   needed to use time functions.

   time_AtoFormAscii replaces co_TimeToAsc and
   time_FormAsciiToA replaces co_AscToTime from Comator Lib. */

#if defined OS_LINUX
# include <time.h>
#endif

typedef unsigned int time_tClock;
#if defined OS_VMS || defined OS_ELN
 typedef pwr_tVaxTime time_tOs;
 typedef pwr_tVaxTime time_tOsDelta;
#elif defined OS_LYNX || defined OS_LINUX
 typedef pwr_tTime time_tOs;
 typedef pwr_tDeltaTime time_tOsDelta;
#else
# error Not defined for this platform
#endif

/*! \file co_time.h
    \brief Time function.
   This include file contains definitions and function prototypes
   needed to use time functions.
*/
/*! \addtogroup Time */
/*@{*/

//! Time string format
typedef enum {
  time_eFormat_DateAndTime = 0,       //!< Display date and time, 01-JAN-1970 01:00:00.00
  time_eFormat_Time  = 1,             //!< Display time only 01:00:00.00
  time_eFormat_ComprDateAndTime = 2,  //!< Date and time compressed format, 70-01-01 00:00:00
  time_eFormat_NumDateAndTime = 3     //!< Date and time format, 2005-01-01 00:00:00
} time_eFormat;

#define SWE 0
#define GB  1

#define SECOND    0
#define HUNDRED   1
#define MINUTE    2

#if defined(OS_VMS) || defined(OS_ELN)

# define CLOCK_REALTIME 1
  typedef int clockid_t;

  int
  clock_gettime (
    clockid_t   clockid,
    struct timespec *ts
  );

  int
  clock_getres (
    clockid_t   clockid,
    struct timespec *tp
  );
#endif

int time_IsNull (pwr_tTime *t1);
pwr_tTime *   time_Aabs (pwr_tTime*, pwr_tTime*);
pwr_tTime *   time_Aadd (pwr_tTime*, pwr_tTime*, pwr_tDeltaTime*);
int     time_Acomp  (pwr_tTime*, pwr_tTime*);
pwr_tDeltaTime *  time_Adiff  (pwr_tDeltaTime*, pwr_tTime*, pwr_tTime*);
pwr_tTime *   time_Aneg (pwr_tTime*, pwr_tTime*);
pwr_tTime *   time_Asub (pwr_tTime*, pwr_tTime*, pwr_tDeltaTime*);
pwr_tDeltaTime *  time_Dabs (pwr_tDeltaTime*, pwr_tDeltaTime*);
pwr_tDeltaTime *  time_Dadd (pwr_tDeltaTime*, pwr_tDeltaTime*, pwr_tDeltaTime*);
pwr_tDeltaTime *  time_Dneg (pwr_tDeltaTime*, pwr_tDeltaTime*);
pwr_tDeltaTime *  time_Dsub (pwr_tDeltaTime*, pwr_tDeltaTime*, pwr_tDeltaTime*);

#if defined (OS_VMS) || defined(OS_ELN)
pwr_tStatus   time_Dmul (pwr_tDeltaTime*, pwr_tDeltaTime*, pwr_tInt32);
#endif

int     time_Dcomp  (pwr_tDeltaTime*, pwr_tDeltaTime*);
pwr_tStatus   time_DtoAscii (pwr_tDeltaTime*, int, char*, int);
pwr_tStatus   time_AtoAscii (pwr_tTime*, time_eFormat, char*, int);
pwr_tStatus   time_AsciiToD (char*, pwr_tDeltaTime*);
pwr_tStatus   time_AsciiToA (char*, pwr_tTime*);
pwr_tStatus   time_TmToAscii  (struct tm*, time_eFormat, char*, int);
pwr_tStatus   time_AsciiToTm  (char*, struct tm*);
pwr_tStatus   time_FormAsciiToA(char*, short, short, pwr_tTime*);
void      time_AtoFormAscii(pwr_tTime*, short, short, char[], int);
pwr_tDeltaTime *  time_MsToD  (pwr_tDeltaTime*, pwr_tInt32);

#if defined(OS_VMS) || defined(OS_ELN)
pwr_tStatus   time_PwrDeltaToVms  (pwr_tDeltaTime*, pwr_tVaxTime*);
pwr_tStatus   time_PwrToVms (pwr_tTime*, pwr_tVaxTime*);
pwr_tStatus   time_VmsToPwr (pwr_tVaxTime*, pwr_tTime*);
pwr_tStatus   time_SetTime  (pwr_tTime*);
#endif

pwr_tDeltaTime *  time_FloatToD (pwr_tDeltaTime*, pwr_tFloat32);
pwr_tFloat32    time_DToFloat (pwr_tFloat32*, pwr_tDeltaTime*);

pwr_tDeltaTime *  time_Uptime (pwr_tStatus*, pwr_tDeltaTime*, pwr_tDeltaTime*);
time_tClock   time_Clock  (pwr_tStatus*, pwr_tDeltaTime*);
time_tOs *    time_Os   (pwr_tStatus*, time_tOs*);
time_tClock   time_DtoClock (pwr_tStatus*, pwr_tDeltaTime*);
pwr_tDeltaTime *  time_ClockToD (pwr_tStatus*, pwr_tDeltaTime*, time_tClock);
time_tOs *    time_AtoOs  (pwr_tStatus*, time_tOs*, pwr_tTime*);
time_tOs *    time_DtoOs  (pwr_tStatus*, time_tOs*, pwr_tDeltaTime*);
pwr_tDeltaTime *  time_ZeroD  (pwr_tDeltaTime*);
void 			time_Sleep	(float time);

#if defined(OS_ELN)
int     eln_clock_gettime (clockid_t, struct timespec*);
struct tm     *eln_localtime  (const time_t*);
time_t      eln_mktime (tm_t*);
int     eln_clock_getres (clockid_t, struct timespec*);

#define clock_gettime eln_clock_gettime
#define localtime eln_localtime
#define mktime eln_mktime
#define clock_getres eln_clock_getres
#endif

/*@}*/

#ifdef __cplusplus
}
#endif

#endif
