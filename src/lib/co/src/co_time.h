#ifndef co_time_h
#define co_time_h
/* co_time.h -- Time utilities

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This include file contains definitions and function prototypes
   needed to use time functions.

   time_AtoFormAscii replaces co_TimeToAsc and
   time_FormAsciiToA replaces co_AscToTime from Comator Lib. */

#ifndef pwr_h
# include "pwr.h" 
#endif

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

typedef enum {
  time_eFormat_DateAndTime = 0,
  time_eFormat_Time  = 1,
  time_eFormat_ComprDateAndTime = 2
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
    clockid_t		clockid,
    struct timespec	*ts
  );

  int
  clock_getres (
    clockid_t		clockid,
    struct timespec	*tp
  );
#endif

pwr_tTime *		time_Aabs	(pwr_tTime*, pwr_tTime*);
pwr_tTime *		time_Aadd	(pwr_tTime*, pwr_tTime*, pwr_tDeltaTime*);
int			time_Acomp	(pwr_tTime*, pwr_tTime*);
pwr_tDeltaTime *	time_Adiff	(pwr_tDeltaTime*, pwr_tTime*, pwr_tTime*);
pwr_tTime *		time_Aneg	(pwr_tTime*, pwr_tTime*);
pwr_tTime *		time_Asub	(pwr_tTime*, pwr_tTime*, pwr_tDeltaTime*);
pwr_tDeltaTime *	time_Dabs	(pwr_tDeltaTime*, pwr_tDeltaTime*);
pwr_tDeltaTime *	time_Dadd	(pwr_tDeltaTime*, pwr_tDeltaTime*, pwr_tDeltaTime*);
pwr_tDeltaTime *	time_Dneg	(pwr_tDeltaTime*, pwr_tDeltaTime*);
pwr_tDeltaTime *	time_Dsub	(pwr_tDeltaTime*, pwr_tDeltaTime*, pwr_tDeltaTime*);

#if defined (OS_VMS) || defined(OS_ELN)
pwr_tStatus		time_Dmul	(pwr_tDeltaTime*, pwr_tDeltaTime*, pwr_tInt32);
#endif

int			time_Dcomp	(pwr_tDeltaTime*, pwr_tDeltaTime*);
pwr_tStatus		time_DtoAscii	(pwr_tDeltaTime*, int, char*, int);
pwr_tStatus		time_AtoAscii	(pwr_tTime*, time_eFormat, char*, int);
pwr_tStatus		time_AsciiToD	(char*, pwr_tDeltaTime*);
pwr_tStatus		time_AsciiToA	(char*, pwr_tTime*);
pwr_tStatus		time_TmToAscii	(struct tm*, time_eFormat, char*, int);
pwr_tStatus		time_AsciiToTm	(char*, struct tm*);
pwr_tStatus		time_FormAsciiToA(char*, short, short, pwr_tTime*);
void			time_AtoFormAscii(pwr_tTime*, short, short, char[], int);
pwr_tDeltaTime *	time_MsToD	(pwr_tDeltaTime*, pwr_tInt32);

#if defined(OS_VMS) || defined(OS_ELN)
pwr_tStatus		time_PwrDeltaToVms	(pwr_tDeltaTime*, pwr_tVaxTime*);
pwr_tStatus		time_PwrToVms	(pwr_tTime*, pwr_tVaxTime*);
pwr_tStatus		time_VmsToPwr	(pwr_tVaxTime*, pwr_tTime*);
pwr_tStatus		time_SetTime	(pwr_tTime*);
#endif

pwr_tDeltaTime *	time_FloatToD	(pwr_tDeltaTime*, pwr_tFloat32);
pwr_tFloat32		time_DToFloat	(pwr_tFloat32*, pwr_tDeltaTime*);

pwr_tDeltaTime *	time_Uptime	(pwr_tStatus*, pwr_tDeltaTime*, pwr_tDeltaTime*);
time_tClock		time_Clock	(pwr_tStatus*, pwr_tDeltaTime*);
time_tOs *		time_Os		(pwr_tStatus*, time_tOs*);
time_tClock		time_DtoClock	(pwr_tStatus*, pwr_tDeltaTime*);
pwr_tDeltaTime *	time_ClockToD	(pwr_tStatus*, pwr_tDeltaTime*, time_tClock);
time_tOs *		time_AtoOs	(pwr_tStatus*, time_tOs*, pwr_tTime*);
time_tOs *		time_DtoOs	(pwr_tStatus*, time_tOs*, pwr_tDeltaTime*);
pwr_tDeltaTime *	time_ZeroD	(pwr_tDeltaTime*);

#if defined(OS_ELN)
int			eln_clock_gettime (clockid_t, struct timespec*);
struct tm 		*eln_localtime 	(const time_t*);
time_t 			eln_mktime (tm_t*);
int			eln_clock_getres (clockid_t, struct timespec*);

#define clock_gettime eln_clock_gettime
#define localtime eln_localtime
#define mktime eln_mktime
#define clock_getres eln_clock_getres
#endif

#endif
