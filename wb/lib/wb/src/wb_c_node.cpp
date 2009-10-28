/* 
 * Proview   $Id: wb_c_node.cpp,v 1.3 2008-06-24 07:52:21 claes Exp $
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

/* wb_c_node.c -- work bench methods of the Node class. */

#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"
#include "wb_wsx.h"
#include "wb_session.h"
#include "pwr_baseclasses.h"
#include "wb_pwrb_msg.h"

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Object,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  pwr_tObjid oid;
  pwr_tClassId cid;
  pwr_tStatus sts;

  sts = ldh_CreateObject(Session, &oid, "Security", pwr_eClass_Security, Object, ldh_eDest_IntoLast); 

  sts = ldh_ClassNameToId(Session, &cid, "MessageHandler");
  sts = ldh_CreateObject(Session, &oid, "MessageHandler", cid, Object, ldh_eDest_IntoLast); 

  sts = ldh_ClassNameToId(Session, &cid, "IOHandler");
  sts = ldh_CreateObject(Session, &oid, "IOHandler", cid, Object, ldh_eDest_IntoLast); 

  sts = ldh_ClassNameToId(Session, &cid, "Backup_Conf");
  sts = ldh_CreateObject(Session, &oid, "Backup", cid, Object, ldh_eDest_IntoLast); 

  sts = ldh_ClassNameToId(Session, &cid, "OpPlace");
  sts = ldh_CreateObject(Session, &oid, "Op", cid, Object, ldh_eDest_IntoLast); 
  sts = ldh_CreateObject(Session, &oid, "Maintenance", cid, Object, ldh_eDest_IntoLast); 
  sts = ldh_CreateObject(Session, &oid, "OpDefault", cid, Object, ldh_eDest_IntoLast); 

  sts = ldh_ClassNameToId(Session, &cid, "PlcProcess");
  sts = ldh_CreateObject(Session, &oid, "Plc", cid, Object, ldh_eDest_IntoLast); 

  sts = ldh_ClassNameToId(Session, &cid, "WebHandler");
  sts = ldh_CreateObject(Session, &oid, "WebHandler", cid, Object, ldh_eDest_IntoLast); 

  sts = ldh_ClassNameToId(Session, &cid, "WebBrowserConfig");
  sts = ldh_CreateObject(Session, &oid, "WebBrowser", cid, Object, ldh_eDest_IntoLast); 

  sts = ldh_ClassNameToId(Session, &cid, "StatusServerConfig");
  sts = ldh_CreateObject(Session, &oid, "StatusServer", cid, Object, ldh_eDest_IntoLast); 

  return PWRS__SUCCESS;
}

static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tAttrRef Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  wb_session *sp = (wb_session *)Session;
  pwr_tAttrRef aref;
  wb_object o;

  // Check Server objects

  // Security object
  o = sp->object( pwr_eClass_Security);
  if ( !o) 
    wsx_error_msg_str( Session, "No Security object found", Object, 'W', ErrorCount, WarningCount);
    
  // DsTrend
  sp->aref( pwr_cClass_DsTrend, &aref);
  if ( sp->oddSts()) {
    // Check DsTrendConf
    o = sp->object( pwr_cClass_DsTrendConf);
    if ( !o) 
      wsx_error_msg_str( Session, "No DsTrendConf object found", Object, 'W', ErrorCount, WarningCount);
  }

  // DsFast
  sp->aref( pwr_cClass_DsFast, &aref);
  if ( sp->evenSts())
    sp->aref( pwr_cClass_DsFastCurve, &aref);

  if ( sp->oddSts()) {
    // Check DsFastConf
    o = sp->object( pwr_cClass_DsFastConf);
    if ( !o)
      wsx_error_msg_str( Session, "No DsFastConf object found", Object, 'W', ErrorCount, WarningCount);
  }

  // SevHistMonintor
  sp->aref( pwr_cClass_SevHist, &aref);
  if ( sp->oddSts()) {
    // Check SevHistMonitor
    o = sp->object( pwr_cClass_SevHistMonitor);
    if ( !o)
      wsx_error_msg_str( Session, "No SevHistMonitor object found", Object, 'W', ErrorCount, WarningCount);
  }

  // Backup_Conf
  sp->aref( pwr_cClass_Backup, &aref);
  if ( sp->oddSts()) {
    // Check Backup_Conf
    o = sp->object( pwr_cClass_Backup_Conf);
    if ( !o)
      wsx_error_msg_str( Session, "No Backup_Conf object found", Object, 'W', ErrorCount, WarningCount);
  }

  return PWRB__SUCCESS;
}

pwr_dExport pwr_BindMethods($Node) = {
  pwr_BindMethod(PostCreate),
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};









