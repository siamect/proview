/* 
 * Proview   $Id: rt_cmvolc.h,v 1.4 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_cmvolc_h
#define rt_cmvolc_h

/* rt_cmvol.h -- Cached volumes
   This module contains the meta cache handling routines.  */


#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_net_h
#include "rt_net.h"
#endif

#define cmvolc_AssumeLocked(ccp) pwr_Assert(ccp->flags.b.cacheLock)


gdb_sCclass *
cmvolc_GetCachedClass (
  pwr_tStatus		*sts,
  const gdb_sNode	*np,
  const gdb_sVolume	*vp,
  mvol_sAttribute	*ap,
  pwr_tBoolean		*equal, /**< set if classes are equal then NULL is returned */
  pwr_tBoolean		*fetched, /**< true if the class has been fected from the remote node */
  gdb_sClass            *cp
  );

void
cmvolc_GetNonExistingClass (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  pwr_tCid              cid
  );


void
cmvolc_LockClass (
  pwr_tStatus		*sts,
  gdb_sCclass		*ccp
  );


void
cmvolc_TouchClass (
  gdb_sCclass		*ccp
  );


void
cmvolc_UnlockClass (
  pwr_tStatus		*sts,
  gdb_sCclass		*ccp
  );



#endif
