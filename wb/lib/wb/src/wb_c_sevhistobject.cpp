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

/* wb_c_sevhistobject.cpp -- work bench methods of the SevHistObject class. */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_pwrb_msg.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "wb_session.h"
#include "wb_wsx.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus PostCreate (
  ldh_tSesContext Session,
  pwr_tObjid	  Object,
  pwr_tObjid	  Father,
  pwr_tClassId	  Class
) {
  pwr_tStatus sts;
  pwr_tOid oid;
  pwr_tOid toid;
  int cnt = 0;  

  // Insert a thread object
  sts = ldh_GetClassList( Session, pwr_cClass_SevHistThread, &oid);
  while ( ODD(sts)) {
    cnt++;
    toid = oid;
    sts = ldh_GetNextObject( Session, oid, &oid);
  }

  if ( cnt > 0) {
    sts = ldh_SetObjectPar( Session, Object, "RtBody", "ThreadObject", (char *)&toid,
			 sizeof(toid));
    if ( EVEN(sts)) return sts;
  }
  return PWRB__SUCCESS;
}

//
//  Syntax check.
//
static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tAttrRef Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  pwr_tOid thread_oid;
  wb_session *sp = (wb_session *)Session;

  wb_attribute a = sp->attribute( &Object);
  if ( !a) return a.sts();

  // Check ThreadObject
  wb_attribute thread_a( a, 0, "ThreadObject");
  if (!thread_a) return thread_a.sts();
    
  thread_a.value( &thread_oid);
  if ( !thread_a) return thread_a.sts();

  wb_object othread = sp->object( thread_oid);
  if ( !othread)
    wsx_error_msg_str( Session, "Bad thread object", Object, 'E', ErrorCount, WarningCount);
  else if ( othread.cid() != pwr_cClass_SevHistThread)
    wsx_error_msg_str( Session, "Bad thread object class", Object, 'E', ErrorCount, WarningCount);
  
  return PWRB__SUCCESS;
}


//  Every method to be exported to the workbench should be registred here.

pwr_dExport pwr_BindMethods(SevHistObject) = {
  pwr_BindMethod(PostCreate),
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
