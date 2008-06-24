/* 
 * Proview   $Id: wb_c_security.cpp,v 1.1 2008-06-24 07:42:58 claes Exp $
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

/* wb_c_security.c -- work bench methods of the $Security class. */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"

static pwr_tStatus AnteCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  pwr_tCid cid;
  pwr_tStatus sts;
  pwr_tOid oid;

  // ClassHier should be child to a $Node object
  if ( Father.oix == 0)
    return PWRS__POSSECURITY;

  sts = ldh_GetObjectClass( Session, Father, &cid);
  if ( EVEN(sts)) return sts;
  
  if ( cid != pwr_eClass_Node)
    return PWRS__POSSECURITY;

  // There should only be one security object
  sts = ldh_GetClassList( Session, pwr_eClass_Security, &oid);
  if ( ODD(sts))
    return PWRS__SECURITYALREX;

  return PWRS__SUCCESS;
}

static pwr_tStatus AnteMove (
  ldh_tSesContext   Session,
  pwr_tObjid	    Object,
  pwr_tObjid	    Father,
  pwr_tObjid	    OldFather
) {

  // Check that that the father is the same
  if ( !(Father.oix == OldFather.oix && Father.vid == OldFather.vid))
    return PWRS__POSSECURITY;
  return PWRS__SUCCESS;
}

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Object,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  pwr_tStatus sts;

  // Set name to Security
  sts = ldh_SetObjectName( Session, Object, "Security");
  if ( EVEN(sts)) return sts;

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods($Security) = {
  pwr_BindMethod(AnteCreate),
  pwr_BindMethod(AnteMove),
  pwr_BindMethod(PostCreate),
  pwr_NullMethod
};





