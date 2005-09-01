/** 
 * Proview   $Id: wb_c_attribute.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* wb_c_attribute.c -- work bench methods of the Attribute class. */

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

  if ( cdh_ObjidIsNull( Father))
    return PWRS__POSATTRIBUTE;

  // Check that the father is of class ClassDef
  sts = ldh_GetObjectClass( Session, Father, &cid);
  if ( EVEN(sts) || cid != pwr_eClass_ObjBodyDef)
    return PWRS__POSATTRIBUTE;
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
    return PWRS__MOVEATTRIBUTE;
  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods($Attribute) = {
  pwr_BindMethod(AnteCreate),
  pwr_BindMethod(AnteMove),
  pwr_NullMethod
};





