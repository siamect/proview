#ifndef rt_dl_h
#define rt_dl_h

/* rt_dl.h -- direct linkage

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   This module contains routines to handle direct linkage.
   Those routines can be called from the GDH API.  */

/* Direct linking datastructures.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_mvol_h
#include "rt_mvol.h"
#endif

typedef struct {
  pwr_tDlid		dlid;		/* pwr_tDlid, must be at same offset as
					   pwr_tSubid in subscription clients and servers */
  pool_sQlink		subc_htl;	/* Subscription client hash table link.  */
  pool_sQlink		dl_ll;		/* Link to next/previous.  */
  pool_tRef		or;		/* Pool reference to object.  */
  pid_t			user;		/* Who requested the direct link.  */
  pwr_sAttrRef		aref;		/* Object data referred.  */
} dl_sLink;

void
dl_Cancel (
  pwr_tStatus		*sts,
  pwr_tDlid		dlid
);

void
dl_CancelUser (
  pid_t			user
);

dl_sLink *
dl_Create (
  pwr_tStatus		*sts,
  mvol_sAttribute	*ap,
  pwr_sAttrRef		*arp
);

#endif
