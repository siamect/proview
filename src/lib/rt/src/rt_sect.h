#ifndef	rt_sect_h
#define rt_sect_h

/* rt_sect.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#if defined(OS_ELN)
# include stddef
#else
# include <stddef.h>
#endif

#include "pwr.h"

#if defined OS_LYNX
# include <semaphore.h>

  typedef sem_t sect_sMutex;

#elif defined OS_LINUX
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

#elif defined OS_LYNX || defined OS_LINUX
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
