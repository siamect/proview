/* 
 * Proview   $Id: pwr_vararg.h,v 1.2 2005-09-01 14:57:49 claes Exp $
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
 */

#ifndef pwr_vararg_h
#define pwr_vararg_h

/* pwr_vararg.h -- take care of differences in vararg definitions
*/

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
