/** 
 * Proview   $Id: wb_c_nodeconfig.c,v 1.4 2005-09-01 14:57:57 claes Exp $
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

/* wb_c_nodeconfig.c -- work bench methods of the NodeConfig class. */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Object,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  pwr_tObjid oid, poid;
  pwr_tClassId cid;
  pwr_tStatus sts;

  sts = ldh_ClassNameToId(Session, &cid, "RootVolumeLoad");
  sts = ldh_CreateObject(Session, &oid, "O1", cid, Object, ldh_eDest_IntoLast);

  sts = ldh_ClassNameToId(Session, &cid, "Distribute");
  sts = ldh_CreateObject(Session, &poid, "Distribute", cid, Object, ldh_eDest_IntoLast);

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods(NodeConfig) = {
  pwr_BindMethod(PostCreate),
  pwr_NullMethod
};

