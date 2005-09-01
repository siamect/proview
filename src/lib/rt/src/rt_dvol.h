/* 
 * Proview   $Id: rt_dvol.h,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#ifndef rt_dvol_h
#define rt_dvol_h

/* rt_dvol.h -- Dynamic volumes  */

#ifndef pwr_h
#include "pwr.h"
#endif

#ifndef rt_vol_h
#include "rt_vol.h"
#endif

#ifndef rt_cvol_h
#include "rt_cvol.h"
#endif

gdb_sObject *
dvol_AdoptObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  gdb_sObject		*p_op,
  net_sNotify		*nmp
);

gdb_sObject *
dvol_CreateObject (
  pwr_tStatus		*sts,
  cdh_sParseName	*pn,
  pwr_tClassId		cid,
  pwr_tUInt32		size,
  pwr_tObjid		oid		/* Requested objid, */
);

pwr_tBoolean
dvol_DeleteObject (
  pwr_tStatus		*sts,
  pwr_tObjid		oid
);

gdb_sObject *
dvol_MoveObject (
  pwr_tStatus		*sts,
  pwr_tObjid		oid,
  pwr_tObjid		poid
);

gdb_sObject *
dvol_RenameObject (
  pwr_tStatus		*sts,
  pwr_tObjid		oid,
  cdh_sParseName	*pn
);

gdb_sObject *
dvol_UnadoptObject (
  pwr_tStatus		*sts,
  gdb_sObject		*op,
  gdb_sObject		*pop,
  net_sNotify		*nmp
);

#endif
