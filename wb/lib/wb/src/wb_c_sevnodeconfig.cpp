/* 
 * Proview   $Id: wb_c_sevnodeconfig.cpp,v 1.1 2008-09-05 09:06:56 claes Exp $
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

/* wb_c_sevnodeconfig.c -- work bench methods of the SevNodeConfig class. */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"
#include "pwr_baseclasses.h"

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Object,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  pwr_tObjid poid;
  pwr_tClassId cid;
  pwr_tStatus sts;
  pwr_tMask comp;

  comp = pwr_mDistrComponentMask_UserDatabase |
    // pwr_mDistrComponentMask_LoadFiles |
    // pwr_mDistrComponentMask_ApplFile |
    // pwr_mDistrComponentMask_XttHelpFile |
    // pwr_mDistrComponentMask_RHostFiles |
    pwr_mDistrComponentMask_AuthorizedKeysFile;


  sts = ldh_ClassNameToId(Session, &cid, "Distribute");
  sts = ldh_CreateObject(Session, &poid, "Distribute", cid, Object, ldh_eDest_IntoLast);
  if ( ODD(sts))
    sts = ldh_SetObjectPar( Session, poid, "DevBody", "Components", (char *)&comp,
			    sizeof(comp));

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods(SevNodeConfig) = {
  pwr_BindMethod(PostCreate),
  pwr_NullMethod
};

