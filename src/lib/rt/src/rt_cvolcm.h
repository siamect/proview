/* 
 * Proview   $Id: rt_cvolcm.h,v 1.3 2006-09-14 14:16:07 claes Exp $
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

#ifndef rt_cvolcm_h
#define rt_cvolcm_h

/* rt_cvolcm.h -- Cached volumes, server monitor API.
   This module contains the cache handling routines.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

void 
cvolcm_AddClassVolume(
  pwr_tStatus		*sts,
  gdb_sNode		*np,
  const net_sGvolume	*vp
);

gdb_sVolume *
cvolcm_ConnectVolume (
  pwr_tStatus	*sts,
  gdb_sVolume	*vp,
  net_sGvolume	*gvp,
  gdb_sNode	*np
);

void
cvolcm_CreateObject (
  qcom_sGet	*get
);

void
cvolcm_DeleteObject (
  qcom_sGet	*get
);

void
cvolcm_FlushNode (
  pwr_tStatus	*sts,
  gdb_sNode	*np 
);

void
cvolcm_FlushVolume (
  pwr_tStatus	*sts,
  gdb_sVolume	*vp 
);

void
cvolcm_MoveObject (
  qcom_sGet	*get
);

void
cvolcm_RenameObject (
  qcom_sGet	*get
);

void
cvolcm_TrimOld (
);

void
cvolcm_ExternVolumeFlush( 
  gdb_sNode *np
);
#endif
