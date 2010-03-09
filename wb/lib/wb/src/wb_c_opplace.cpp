/* 
 * Proview   $Id: wb_c_opplace.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

/* wb_c_opplace.c -- work bench methods of the OpPlace class. */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"
#include "wb_session.h"
#include "wb_wsx.h"
#include "wb_pwrb_msg.h"

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tOid	    Object,
  pwr_tOid	    Father,
  pwr_tCid	    Class
) {
  pwr_tStatus sts;
  pwr_tObjName name;
  pwr_tMask   value = 1;
  int size;

  sts = ldh_ObjidToName( Session, Object, ldh_eName_Object,
			 name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  if ( strcmp( name, "OpDefault") == 0) {
    // Set OpHide in OpWindLayout
    
    sts = ldh_SetObjectPar( Session, Object, "RtBody",
			    "OpWindLayout", (char *) &value, sizeof(value));
    if ( EVEN(sts)) return sts;
  }

  return PWRS__SUCCESS;
}

//
// SyntaxCheck
//
static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tAttrRef Object,
  int *ErrorCount,
  int *WarningCount
) {
  wb_session *sp = (wb_session *)Session;
  pwr_tString80 selectlist[40];

  wb_attribute a = sp->attribute( &Object);
  if ( !a) return a.sts();

  // Check Attribute
  wb_attribute selectlist_a( a, 0, "EventSelectList");
  if (!selectlist_a) return selectlist_a.sts();
    
  selectlist_a.value( selectlist);
  if ( !selectlist_a) return selectlist_a.sts();

  bool empty = true;
  for ( unsigned int i = 0; i < sizeof(selectlist)/sizeof(selectlist[0]); i++) {
    if ( strcmp( selectlist[i], "") != 0) {
      empty = false;
      break;
    }
  }
  if ( empty)
    wsx_error_msg_str( Session, "EventSelectList is empty", Object, 'W', ErrorCount, WarningCount);

  return PWRB__SUCCESS;
}

pwr_dExport pwr_BindMethods(OpPlace) = {
  pwr_BindMethod(PostCreate),
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};

