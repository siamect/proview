/* 
 * Proview   $Id: rt_subc.h,v 1.2 2005-09-01 14:57:56 claes Exp $
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

#ifndef rt_subc_h
#define rt_subc_h

/* rt_subc.h -- Object data subscription, client side.  */

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
