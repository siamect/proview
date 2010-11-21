/* 
 * Proview   $Id: rt_sect.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef	rt_sect_h
#define rt_sect_h

#if defined(OS_ELN)
# include stddef
#else
# include <stddef.h>
#endif

#include "pwr.h"

#if defined OS_LYNX
# include <semaphore.h>

  typedef sem_t sect_sMutex;

#elif defined OS_LINUX || defined OS_MACOS || defined OS_FREEBSD
# include "rt_semaphore.h"

  typedef sem_t sect_sMutex;

#endif

#ifdef	OS_ELN
# include $mutex
# include descrip

  typedef AREA_LOCK_VARIABLE sect_sMutex;
#endif

#ifdef	OS_VMS
# include <descrip.h>

  typedef struct {
    pwr_tInt64		entry;
    pwr_tInt32		flag;
    pwr_tInt32		fill;
  } sect_sMutexEntry;

  typedef struct {
    pwr_tInt64		ownerroot;
    pwr_tInt64		freeroot;
    sect_sMutexEntry	list[32];
  } sect_sMutex;
#endif

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( mapped	, 1 ),

    pwr_Bits( fill	, 31 ),,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define sect_mHead__		0
#define sect_mHead_mapped	pwr_Bit(0)
#define sect_mHead_		(~sect_mHead__)
} sect_mHead;

#if defined OS_VMS || defined OS_ELN

  typedef	struct { 
    void		*base;		/* Virtual address of section.  */
    size_t		size;
    pwr_tUInt32		sectadr[2];	/* Retadr info from create.  */
    struct dsc$descriptor namedsc;	/* Descriptor to name string.  */
    char		name[16];	/* Name of section.  */
    pwr_tUInt32		area;		/* Used by ELN.  */
    sect_mHead		flags;
  } sect_sHead;

#elif defined OS_LYNX || defined OS_LINUX || OS_MACOS || defined OS_FREEBSD
  typedef struct { 
    void		*base;		/* Virtual address of section.  */
    size_t		size;
    char		name[32];	/* Name of section.  */
    sect_mHead		flags;
  } sect_sHead;
#endif


/* Function prototypes */

sect_sHead *
sect_Alloc (
  pwr_tStatus		*sts,
  pwr_tBoolean		*created,
  sect_sHead		*shp,
  size_t		size,
  char			*name
);

pwr_tBoolean
sect_Free (
  pwr_tStatus		*sts,
  sect_sHead		*shp
);

pwr_tBoolean
sect_InitLock (
  pwr_tStatus		*sts,
  sect_sHead		*shp,
  sect_sMutex		*sect_mutex
);

pwr_tBoolean
sect_Lock (
  pwr_tStatus		*sts,
  sect_sHead		*shp,
  sect_sMutex		*sect_mutex
);

pwr_tBoolean
sect_Unlock (
  pwr_tStatus		*sts,
  sect_sHead		*shp,
  sect_sMutex		*sect_mutex
);

#endif
