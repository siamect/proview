/** 
 * Proview   $Id: co_time.c,v 1.9 2006-05-02 07:06:53 claes Exp $
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

/* co_time.c -- Utilities for time management.
   NOTE! to convert timespecs to tm's, the threadsafe version of localtime,
         localtime_r must be used, which doesn't exist on DEC.  */
 
#include "co_time.h"
#include "co_time_msg.h"


#ifdef OS_ELN
# include stdio
# include ctype
# include string
# include stdlib
# include descrip
# include errno
# include limits
# include times
#else
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <stdlib.h>
#endif

#if defined OS_LYNX
# include <sys/times.h>
#elif defined OS_LINUX
# include <time.h> 
#endif

#if defined OS_LYNX
# define assertAbs(p) do {\
    pwr_Assert(p->tv_sec >= 0 && p->tv_nsec >= 0 && p->tv_nsec < 1000000000);\
  } while (0)
#else
# define assertAbs(p) do {\
    pwr_Assert(p->tv_nsec >= 0 && p->tv_nsec < 1000000000);\
  } while (0)
#endif

#define assertDelta(p) do {\
  pwr_Assert((p->tv_sec > 0) ? (p->tv_nsec >= 0 && p->tv_nsec <  1000000000) : TRUE);\
  pwr_Assert((p->tv_sec < 0) ? (p->tv_nsec <= 0 && p->tv_nsec > -1000000000) : TRUE);\
} while (0)

/* String representations of months.  */

static const char *monStr[] =
{
  "JAN", "FEB", "MAR", "APR", "MAY", "JUN", 
  "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"
};

static pwr_tStatus  validateTm		(struct tm *tms);


/* Validate data in struct tm.  */

static pwr_tStatus
validateTm (
  struct tm	*tms
)
{
  int year;

  /* Check generic ranges.  */

  if (70 > tms->tm_year || tms->tm_year > 137) /* EPOCH is 1970 end feb 2038 */
    return TIME__RANGE;
  else if (0 > tms->tm_mon || tms->tm_mon > 11)
    return TIME__RANGE;
  else if(1 > tms->tm_mday || tms->tm_mday > 31)
    return TIME__RANGE;
  else if(0 >  tms->tm_hour || tms->tm_hour > 23)
    return TIME__RANGE;
  else if(0 > tms->tm_min || tms->tm_min > 59)
    return TIME__RANGE;
  else if(0 > tms->tm_sec || tms->tm_sec > 59) /* Should be 61 according to POSIX */
    return TIME__RANGE;

  /*
   * validate date
   */
  switch (tms->tm_mon) {
  case 1:
    /* check for leap year */
    year = tms->tm_year + 1900;
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
      if (tms->tm_mday > 29)
	return TIME__RANGE;
    } else {
      if (tms->tm_mday > 28)
	return TIME__RANGE;
    }
    break;

  case 3:
  case 5:
  case 8:
  case 10:
    if (tms->tm_mday > 30)
      return TIME__RANGE;
    break;

  case 0:
  case 2:
  case 4:
  case 6:
  case 7:
  case 9:
  case 11:
    if (tms->tm_mday > 31)
      return TIME__RANGE;
    break;

  default:
    return TIME__RANGE;
  }
  return TIME__SUCCESS;
}

//! Add an absolute time and a delta time.
/*! Add two timespecs, result = t + d, where:
   
   -  'result' and 't' is an absolute time, and
   -  'd' is a delta time.

  If 'result' argument is NULL
  then 't' will be used as resultant.
  Returns the address to the resulting time.
*/

pwr_tTime *
time_Aadd (
  pwr_tTime		*result,
  pwr_tTime		*t,
  pwr_tDeltaTime	*a
)
{
  pwr_tTime		*r = result;
  int			tv_nsec;

  assertAbs(t);
  assertDelta(a);

  if (result == NULL) r = t;

  tv_nsec = t->tv_nsec + a->tv_nsec;
  r->tv_sec = t->tv_sec + a->tv_sec + (tv_nsec / 1000000000);
  r->tv_nsec = tv_nsec % 1000000000;

  return r;
}

//! Compare two timespecs.
/*!   Returns \n
    1 if t1  > t2 \n
    0 if t1 == t2 \n
   -1 if t1  < t2 \n

   If argument 't2' is NULL the comparison will
   be done as if t2 == 0.  
*/

int 
time_Acomp (
  pwr_tTime *t1, 
  pwr_tTime *t2
)
{
  static pwr_tTime null = {0, 0};

  if (t2 == NULL) t2 = &null;

  assertAbs(t1);
  assertAbs(t2);

  if (t1->tv_sec == t2->tv_sec)
  {
    if ( t1->tv_nsec == t2->tv_nsec)
      return 0;
    return ((t1->tv_nsec > t2->tv_nsec) ? 1 : -1);
  }
  else
    return ((t1->tv_sec > t2->tv_sec) ? 1 : -1);
}

//! Subtract a time from a time,
/*!   r = t - s

   Result is always a delta time.  
*/

pwr_tDeltaTime *
time_Adiff (
  pwr_tDeltaTime	*r,
  pwr_tTime		*t,
  pwr_tTime		*s
)
{
  int			tv_nsec = t->tv_nsec - s->tv_nsec;
  int			tv_sec  = t->tv_sec  - s->tv_sec;

  pwr_Assert(r != NULL);
  assertAbs(t);
  assertAbs(s);

  tv_sec  = tv_sec + tv_nsec / 1000000000;
  tv_nsec = tv_nsec % 1000000000;
  if (tv_nsec < 0 && tv_sec > 0) {
    tv_sec--;
    tv_nsec += 1000000000;
  } else if (tv_sec < 0 && tv_nsec > 0) {
    tv_sec++;
    tv_nsec -= 1000000000;
  }

  r->tv_sec = tv_sec;
  r->tv_nsec = tv_nsec;

  return r;
}

//! Subtract a delta time from a time,
/*!   r = t - s

   Result is always an abstime.  
*/

pwr_tTime *
time_Asub (
  pwr_tTime		*result,
  pwr_tTime		*t,
  pwr_tDeltaTime	*s
)
{
  int			tv_nsec = t->tv_nsec - s->tv_nsec;
  int			tv_sec  = (int)t->tv_sec  - s->tv_sec;
  pwr_tTime		*r = result;

  assertAbs(t);
  assertDelta(s);

  if (r == NULL) r = t;

  tv_sec  += tv_nsec / 1000000000;
  tv_nsec %= 1000000000;
  if (tv_nsec < 0 && tv_sec > 0) {
    tv_sec--;
    tv_nsec += 1000000000;
  } else if (tv_sec < 0 && tv_nsec > 0) {
    tv_sec++;
    tv_nsec -= 1000000000;
  }

  r->tv_sec = tv_sec;
  r->tv_nsec = tv_nsec;
  return r;

}

//! Take the absolute walue of a delta time.
/*!   
   'result' = |'t'|

   A NULL address => abs value is written to 't'.
   Returns the address to the resulting time. 
*/

pwr_tDeltaTime *
time_Dabs (
  pwr_tDeltaTime	*result,
  pwr_tDeltaTime	*t
)
{
  pwr_tDeltaTime	*r = result;

  assertDelta(t);

  if (r == NULL) r = t;

  if (r->tv_sec < 0)  r->tv_sec	 = -r->tv_sec;
  if (r->tv_nsec < 0) r->tv_nsec = -r->tv_nsec;

  return r;
}

//! Add two delta times, the result is also delta.
/*!   If 'result' is NULL then 'a' will be added to 't'.  */

pwr_tDeltaTime * 
time_Dadd (
  pwr_tDeltaTime	*result,
  pwr_tDeltaTime	*t,
  pwr_tDeltaTime	*a
)
{
  pwr_tDeltaTime	*r = result;
  int			tv_nsec, tv_sec;

  assertDelta(t);
  assertDelta(a);

  if (result == NULL) r = t;

  tv_nsec = t->tv_nsec + a->tv_nsec;
  tv_sec = t->tv_sec + a->tv_sec + (tv_nsec / 1000000000);
  tv_nsec = tv_nsec % 1000000000;
  
  if (tv_nsec < 0 && tv_sec > 0) {
    tv_sec--;
    tv_nsec += 1000000000;
  } else if (tv_sec < 0 && tv_nsec > 0) {
    tv_sec++;
    tv_nsec -= 1000000000;
  }
  
  r->tv_sec = tv_sec;
  r->tv_nsec = tv_nsec;

  return r;
}

//! Compare two delta times.
/*!   Returns \n
    1 if t1  > t2 \n
    0 if t1 == t2 \n
   -1 if t1  < t2 \n

   If argument 't2' is NULL the comparison will
   be done as if t2 == 0.  
*/

int 
time_Dcomp (
  pwr_tDeltaTime	*t1, 
  pwr_tDeltaTime	*t2
)
{
  static pwr_tDeltaTime null = {0, 0};

  if (t2 == NULL)
    t2 = &null;

  assertDelta(t1);
  assertDelta(t2);

  if (t1->tv_sec == t2->tv_sec) {
    if (t1->tv_nsec == t2->tv_nsec)
      return 0;
    return ((t1->tv_nsec > t2->tv_nsec) ? 1 : -1);
  }
  return ((t1->tv_sec > t2->tv_sec) ? 1 : -1);
}

//! Negate a delta time,
/*!
   result = -d

  If 'result' argument is NULL
  then 'd' will be used as resultant.
  Returns the address to the resulting time.  
*/

pwr_tDeltaTime *
time_Dneg (
  pwr_tDeltaTime	*result,
  pwr_tDeltaTime	*t
)
{
  pwr_tDeltaTime	*r = result;

  assertDelta(t);

  if (r == NULL) r = t;

  r->tv_sec  = -r->tv_sec;
  r->tv_nsec = -r->tv_nsec;

  return r;
}

//! Subtract two delta times.
/*! The result is also delta.  */

pwr_tDeltaTime *
time_Dsub (
  pwr_tDeltaTime	*result,
  pwr_tDeltaTime	*t,
  pwr_tDeltaTime	*s
)
{
  int			tv_nsec = t->tv_nsec - s->tv_nsec;
  int			tv_sec  = t->tv_sec  - s->tv_sec;
  pwr_tDeltaTime	*r = result;

  assertDelta(t);
  assertDelta(s);

  if (r == NULL) r = t;

  tv_sec  = tv_sec + tv_nsec / 1000000000;
  tv_nsec = tv_nsec % 1000000000;
  if (tv_nsec < 0 && tv_sec > 0) {
    tv_sec--;
    tv_nsec += 1000000000;
  } else if (tv_sec < 0 && tv_nsec > 0) {
    tv_sec++;
    tv_nsec -= 1000000000;
  }

  r->tv_sec = tv_sec;
  r->tv_nsec = tv_nsec;

  return r;
}

//! Convert a delta time to ascii string.

pwr_tStatus
time_DtoAscii ( 
  pwr_tDeltaTime  *dt,
  int		  hundreds,
  char		  *buf,
  int		  bufsize
)
{
  char tmpStr[24];
  div_t day, hour, min;

  if (dt == NULL)
    return TIME__IVDTIME;

  day  = div(dt->tv_sec, 24 * 3600);
  hour = div(day.rem, 3600);
  min  = div(hour.rem, 60);

  if (day.quot) {
    if (hundreds)
      sprintf(tmpStr, "%d %d:%02d:%02d.%02d", day.quot, hour.quot, min.quot, min.rem, dt->tv_nsec/10000000);
    else
      sprintf(tmpStr, "%d %d:%02d:%02d", day.quot, hour.quot, min.quot, min.rem);
  } else {
    if (hundreds)
      sprintf(tmpStr, "%d:%02d:%02d.%02d", hour.quot, min.quot, min.rem, dt->tv_nsec/10000000);
    else
      sprintf(tmpStr, "%d:%02d:%02d", hour.quot, min.quot, min.rem);
  }

  strncpy(buf, tmpStr, bufsize);
  buf[bufsize-1] = '\0';

  return TIME__SUCCESS;
}


//! Convert timespec to ascii
/*!   
   NOTE: Not thread safe.  */

pwr_tStatus
time_AtoAscii ( 
  pwr_tTime	*ts,
  time_eFormat	format,
  char		*buf,
  int		bufsize
)
{
  struct tm	*tmpTm;
  int		buflen;
  char		tmpStr[16];
  pwr_tTime	time;
  pwr_tTime	*tp;


  if (ts == NULL)
  {
    clock_gettime(CLOCK_REALTIME, &time);
    tp = &time;
  }
  else
    tp = ts;

  tmpTm = localtime(&tp->tv_sec);
  if (EVEN(time_TmToAscii(tmpTm, format, buf, bufsize)))
    return TIME__RANGE;

  
  sprintf(tmpStr, ".%02d", (int)(tp->tv_nsec / 10000000));
  buflen = strlen(buf);
  if (strlen(tmpStr) + buflen < (unsigned int) bufsize)
    strcpy(&buf[buflen], tmpStr);

  return TIME__SUCCESS;
}

//! Convert ascii to timespec.
pwr_tStatus 
time_AsciiToD (
  char		  *tstr, 
  pwr_tDeltaTime  *ts
)
{
  char *sp,*dp;
  char buf[64]; 
  int day, hour, min, sec, hun = 0;
  int useday = 1;

  strncpy(buf, tstr, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';
  sp = buf;

  day = strtoul(sp, &dp, 10);
  if (dp == NULL)
    return TIME__RANGE;
  
  if (*dp == ':') {
    hour = day;    
    day = 0;
    useday = 0;
    if (hour > 23)
      return TIME__RANGE;
  } 
  else if (*dp != ' ')
    return TIME__RANGE;
  sp = dp + 1;
  
  if (useday) {
    if (day > 24855)
      return TIME__RANGE;
    hour =  strtoul(sp, &dp, 10);
    if (dp == NULL || *dp != ':' || hour > 23)
      return TIME__RANGE;
    sp = dp + 1;
  }

  min = strtoul(sp, &dp, 10);
  if (dp == NULL || *dp != ':' || min > 59)
    return TIME__RANGE;
  sp = dp + 1;

  sec = strtoul(sp, &dp, 10);
  if ((dp && *dp && *dp != ' ' && *dp != '.') || sec > 59)
    return TIME__RANGE;
  
  if (dp && *dp == '.') {
    hun =  strtoul(dp + 1, &dp, 10);
    if ((dp && *dp && *dp != ' ') || hun > (10000000 - 1))
      return TIME__RANGE;
  }


  ts->tv_sec = day * 24 * 3600 + hour * 3600 + min * 60 + sec;
  ts->tv_nsec = hun * 10000000;

  return TIME__SUCCESS;
}


//! Convert ascii time to timespec.

pwr_tStatus 
time_AsciiToA (
  char	      *tstr, 
  pwr_tTime   *ts
)
{
  struct tm   tmpTm;
  int	      tmphs = 0;
  char	      *dotp;
  char	      buf[64]; 
  pwr_tStatus sts;

  strncpy(buf, tstr, sizeof(buf) - 1);
  buf[sizeof(buf) - 1] = '\0';

  if ((dotp = strchr(buf, '.'))) {
    int len;
    char *cp;

    *dotp = '\0';
    dotp++;
    len = strlen(dotp);
    if (len > 2) /* only hundreds of seconds */
      *(dotp + 2) = 0;
    
    if (*dotp == '0') dotp++; /* remove first zero */
    tmphs = strtoul(dotp, &cp, 0);
    if (*cp)
      return TIME__RANGE; 
    if (len == 1)
      tmphs *= 10;

  }

  sts = time_AsciiToTm(buf, &tmpTm);
  if (EVEN(sts))
    return sts;

  ts->tv_sec = mktime(&tmpTm);
  ts->tv_nsec = tmphs * 10000000;
  return TIME__SUCCESS;
}

//! Convert time struct to string.

pwr_tStatus 
time_TmToAscii (
  struct tm	*tmptr,
  time_eFormat	format,
  char		*buf,
  int		bufsize
)
{
  pwr_tStatus	sts;

  *buf = '\0';
  if (EVEN(sts = validateTm(tmptr)))
    return sts;

  if (format == time_eFormat_DateAndTime) {
    int i;
    strftime(buf, bufsize, "%d-%b-%Y %H:%M:%S", tmptr);
    for (i = 3; i < 6; i++)
      buf[i] = toupper(buf[i]);
  } else if (format == time_eFormat_ComprDateAndTime) {
    strftime(buf, bufsize, "%y-%m-%d %H:%M:%S", tmptr);
  } else {
    strftime(buf, bufsize, "%H:%M:%S", tmptr);
  }

  return TIME__SUCCESS;
}

//! Convert timestring to struct.

pwr_tStatus 
time_AsciiToTm (
  char		*tstr, 
  struct tm	*tmptr
)
{
  char		tmpMonStr[4];
  char		*cp;
  struct tm	tt;
  int		i;
  pwr_tStatus	sts;

  sscanf(tstr, "%02d-%3c-%4d %02d:%02d:%02d", &tt.tm_mday,
         tmpMonStr, &tt.tm_year, &tt.tm_hour, &tt.tm_min, &tt.tm_sec);

  tmpMonStr[3] = '\0';
  tt.tm_year -= 1900;


  /* We don't handle this in current version */

  tt.tm_wday = -1; 
  tt.tm_yday = -1;
  tt.tm_isdst = -1;


  /* check month */
  for (cp = tmpMonStr; *cp; cp++)
    *cp = toupper(*cp);
   
  tt.tm_mon = -1;
  for (i=0; i<12; i++) {
    if (strcmp(tmpMonStr, monStr[i]) == 0) {
      tt.tm_mon = i;
      break;
    }
  }

  if (EVEN(sts = validateTm(&tt)))
    return sts;

  *tmptr = tt;

  return TIME__SUCCESS;
}

/* Compatibility-function that substitutes co_TimeToAsc.  */

pwr_tStatus
time_FormAsciiToA (
  char		*tstr,
  short		dissolution,
  short		formType,
  pwr_tTime	*ts
)
{
  struct tm	tmpTm;
  int		i;
  int		year;
  int		month;
  int		day;
  int		tmphs = 0;
  char		*dotp;
  char		buf[64]; 
  pwr_tStatus	sts;
  char		*cp = tstr;

  /* Format of the date string should be YYYY-MM-DD HH:MM[:SS.CC] */
  while (*cp && isspace(*cp))
    cp++;

  /* Get year */
  for (i = 0; i < 4; i++,cp++) {
    if (*cp == '\0' || !isdigit(*cp))
      return TIME__RANGE;
    buf[i] = *cp;
  }
  buf[i] = '\0';
  year = atoi(buf);
  if (*cp == '\0' || *cp != '-')
      return TIME__RANGE;
  cp++;
      

  /* Get month */
  for (i = 0; i < 2; i++,cp++) {
    if (*cp == '\0' || !isdigit(*cp))
      return TIME__RANGE;
    buf[i] = *cp;
  }
  buf[i] = '\0';
  month = atoi(buf) - 1;
  if (month < 0 || month > 11)
    return TIME__RANGE;
  if (*cp == '\0' || *cp != '-')
    return TIME__RANGE;
  cp++;
       

  /* Get Day */
  for (i = 0; i < 2; i++,cp++) {
    if (*cp == '\0' || !isdigit(*cp))
      return TIME__RANGE;
    buf[i] = *cp;
  }
  buf[i] = '\0';
  day = atoi(buf);

  /* Build a new date string on VMS format, dd-mmm-yyyy ...*/
  sprintf(buf, "%02d-%s-%d%s", day, monStr[month], year, cp);  

  if (dissolution == MINUTE) {
     strcat(buf, ":00");
  } else if (dissolution == HUNDRED) {
    if ((dotp = strchr(buf, '.'))) {
      int len;
      char *cp;

      *dotp = '\0';
      dotp++;
      len = strlen(dotp);
      if (len > 2) /* only hundreds of seconds */
        *(dotp + 2) = 0;
    
      if (*dotp == '0') dotp++;
      tmphs = strtoul(dotp, &cp, 0);
      if (*cp)
        return TIME__RANGE; 
      if (len == 1)
        tmphs *= 10;
    }
  }     

  sts = time_AsciiToTm(buf, &tmpTm);
  if (EVEN(sts))
    return sts;

  ts->tv_sec = mktime(&tmpTm);
  ts->tv_nsec = tmphs * 10000000;
  return TIME__SUCCESS;
}

/* .  */

void
time_AtoFormAscii (
  pwr_tTime	*ts,
  short		dissolution,
  short		formType,
  char		buf[],
  int		bufsize
)
{
  int		len;
  struct tm *tmpTm;
  char   tmphs[16];
  pwr_tTime time, *tp;

  if (ts == NULL) {
    clock_gettime(CLOCK_REALTIME, &time);
    tp = &time;
  } else {
    tp = ts;
  }

  switch (formType) {
  case GB:
  case SWE:
  default:
    tmpTm = localtime(&tp->tv_sec);

    switch (dissolution) {
    case HUNDRED:
      len = strftime(buf, bufsize, "%Y-%m-%d %H:%M:%S", tmpTm);
      if (len != 0 && len + 4 <= bufsize) { 
        sprintf(tmphs, ".%02d", (int)(tp->tv_nsec / 10000000));
        strcat(buf, tmphs);
      }
      break;

    case MINUTE:
      strftime(buf, bufsize, "%Y-%m-%d %H:%M", tmpTm);
      break;

    case SECOND:
    default:
      strftime(buf, bufsize, "%Y-%m-%d %H:%M:%S", tmpTm);
      break;
    }
  }
}

//! Convert millisec to timespec.

pwr_tDeltaTime * 
time_MsToD (
  pwr_tDeltaTime	*dt,
  pwr_tInt32		ms
)
{
  static pwr_tDeltaTime	time;
  static pwr_tDeltaTime	*t = &time;

  if (dt != NULL) t = dt;

  t->tv_sec  = ms / 1000;
  t->tv_nsec = (ms % 1000) * 1000000;

  return t;
}

//! Convert float to time.

pwr_tDeltaTime * 
time_FloatToD (
  pwr_tDeltaTime	*dt,
  pwr_tFloat32		f
)
{
  static pwr_tDeltaTime	time;
  static pwr_tDeltaTime	*t = &time;

  if (dt != NULL) t = dt;

  t->tv_sec  = f;
  t->tv_nsec = (f - t->tv_sec) * 1e9;

  return t;
}


//! Convert time to float.

pwr_tFloat32 
time_DToFloat (
  pwr_tFloat32		*f,
  pwr_tDeltaTime	*dt
)
{
  static pwr_tFloat32	flt;

  flt =  1e-9 * dt->tv_nsec  + dt->tv_sec;

  if (f != NULL) *f = flt;

  return flt;
}

time_tClock
time_DtoClock (
  pwr_tStatus *status,
  pwr_tDeltaTime *tp
)
{
  pwr_dStatus(sts, status, TIME__SUCCESS);

  return tp->tv_sec * 100 + tp->tv_nsec / 10000000; 
}

pwr_tDeltaTime *
time_ClockToD (
  pwr_tStatus *status,
  pwr_tDeltaTime *tp,
  time_tClock clock
)
{
  pwr_tDeltaTime time;
  pwr_dStatus(sts, status, TIME__SUCCESS);

  if (tp == NULL)
    tp = &time;
  
  tp->tv_sec = clock / 100;
  tp->tv_nsec = clock % 100 * 10000000;

  return tp;
}  

pwr_tDeltaTime *
time_ZeroD (
  pwr_tDeltaTime *tp
)
{
  static pwr_tDeltaTime time;

  if (tp == NULL)
    tp = &time;

  memset(tp, 0, sizeof(*tp));

  return tp;  
}

void time_Sleep( float time)
{
#ifdef OS_VMS
	int sts;
        sts = lib$wait(&time);
#elif OS_ELN
	LARGE_INTEGER	l_time;

	l_time.high = -1;	
	l_time.low = - time * 10000000;	
	ker$wait_any( NULL, NULL, &l_time);
#elif defined(OS_LYNX) || defined (OS_LINUX)
	pwr_tDeltaTime	p_time;

	time_FloatToD( &p_time, time);
	nanosleep( (pwr_tTime *)&p_time, NULL);
#endif
}

