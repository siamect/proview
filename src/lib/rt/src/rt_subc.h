#ifndef rt_subc_h
#define rt_subc_h

/* rt_subc.h -- Object data subscription, client side.

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.
   
   <Description.>  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_pool_h
#include "rt_pool.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_sub_h
#include "rt_sub.h"
#endif

void
subc_ActivateList (
  pool_sQlink		*lh,
  pwr_tObjid		oid
);

void
subc_CancelList (
  pool_sQlink		*lh
);

void
subc_CancelUser (
  pid_t			subscriber
);


sub_sClient *
subc_Create (
  char			*name,			/* Input or NULL */
  pwr_sAttrRef		*arp,			/* Input or NULL */
  pool_sQlink		*lh			/* List header. */
);

void
subc_RemoveFromMessage (
  sub_sClient		*cp
);

void
subc_SetDefaults (
  pwr_tInt32		dt,
  pwr_tInt32		tmo
);

void
subc_SetOld (
  sub_sClient		*cp
);

#endif
