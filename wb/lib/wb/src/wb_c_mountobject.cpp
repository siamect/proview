/* 
 * Proview   $Id$
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

/* wb_c_mountobject.c -- work bench methods of the MountObject class. */

#include "pwr_baseclasses.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"
#include "wb_wsx.h"
#include "wb_wsx_msg.h"
#include "wb_session.h"

//
//  Syntax check.
//

static pwr_tStatus SyntaxCheck( ldh_tSesContext Session,
				pwr_tAttrRef Object,
				int *ErrorCount,
				int *WarningCount) 
{
  wb_session *sp = (wb_session *)Session;
  pwr_tOid mount_oid;

  // Check Object attribute
  wb_attribute a = sp->attribute( Object.Objid, "SysBody", "Object");
  if (!a) return a.sts();
    
  a.value( &mount_oid);
  if ( !a) return a.sts();

  if ( cdh_ObjidIsNull( mount_oid)) {
    wsx_error_msg_str( Session, "Mounted object is missing", Object, 'E', ErrorCount, WarningCount);
  }
  return PWRS__SUCCESS;
}

//
//  Every method to be exported to the workbench should be registred here.
//
pwr_dExport pwr_BindMethods($MountObject) = {
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
