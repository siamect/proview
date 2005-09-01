/** 
 * Proview   $Id: wb_c_plcprocess.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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
 **/

/* wb_c_plcprocess.c -- work bench methods of the PlcProcess class. */

#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tOid	    Object,
  pwr_tOid	    Father,
  pwr_tCid	    Class
) {
  pwr_tOid oid;
  pwr_tCid cid;
  pwr_tStatus sts;
  pwr_tFloat32 scan_time = 0.1;

  sts = ldh_ClassNameToId(Session, &cid, "PlcThread");
  if ( EVEN(sts)) return sts;

  sts = ldh_CreateObject(Session, &oid, "100ms", cid, Object, ldh_eDest_IntoLast); 
  if ( EVEN(sts)) return sts;

  sts = ldh_SetObjectPar(Session, oid, "RtBody", "ScanTime", (char *)&scan_time,
			 sizeof(scan_time));
  if ( EVEN(sts)) return sts;

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods(PlcProcess) = {
  pwr_BindMethod(PostCreate),
  pwr_NullMethod
};







