#ifndef pwr_vararg_h
#define pwr_vararg_h

/* pwr_vararg.h -- take care of differences in vararg definitions

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifndef __STDARG_LOADED
#include <stdarg.h>
#endif


#ifdef __DECC

#ifdef __ALPHA
unsigned int __VA_COUNT_BUILTIN(void);
#define cvararg_dVaCount(count)         (count = __VA_COUNT_BUILTIN())

#else /* VAX and DECC */

void decc$va_count( int *__count );
#define cvararg_dVaCount(count)         decc$va_count (&count)
#endif

#else /* VAX and VAXC */
void vaxc$va_count( int *__count );
#define cvararg_dVaCount(count)         vaxc$va_count (&count)

#endif

#endif
