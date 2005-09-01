/* 
 * Proview   $Id: rt_dl.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_dl_h
#define rt_dl_h

/* rt_dl.h -- direct linkage
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
