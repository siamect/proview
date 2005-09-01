/* 
 * Proview   $Id: rt_cvolsm.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_cvolsm_h
#define rt_cvolsm_h

/* rt_cvolsm.h -- Cached volumes, server monitor API.
   This module contains the cache handling routines.  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef rt_gdb_h
#include "rt_gdb.h"
#endif

#ifndef rt_cvol_h
#include "rt_cvol.h"
#endif

gdb_sMountedOn *
cvolsm_AddMountedOn (
  pwr_tStatus		*sts,
  pwr_tVolumeId		vid,
  gdb_sNode		*np
);

void
cvolsm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
);

void
cvolsm_GetObjectInfo (
  qcom_sGet		*get
);

void
cvolsm_NameToObject (
  qcom_sGet		*get
);

void
cvolsm_OidToObject (
  qcom_sGet		*get
);

void
cvolsm_RemoveMountedOn (
  pwr_tStatus		*sts,
  gdb_sMountedOn	*mop
);

void
cvolsm_SetObjectInfo (
  qcom_sGet		*get
);

#endif
