/* 
 * Proview   $Id: wb_c_plcpgm.cpp,v 1.4 2008-05-28 11:48:17 claes Exp $
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

/* wb_c_plcpgm.c -- work bench methods of the PlcPgm class. */

#include "flow_ctx.h"
#include "wb_pwrs.h"
#include "pwr_baseclasses.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_foe.h"
#include "wb_pwrb_msg.h"
#include "wb_foe_msg.h"
#include "wb_wsx.h"
#include "wb_wsx_msg.h"
#include "wb_utl_api.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_wnav.h"
#include "wb_build.h"
#include "wb_session.h"


/*----------------------------------------------------------------------------*\
  To open a PLC program with the PLC editor.
\*----------------------------------------------------------------------------*/

static pwr_tStatus OpenProgram (
  ldh_sMenuCall *ip
)
{
  pwr_tStatus sts;
  void *foe;

  sts = ip->wnav->open_foe( "PlcPgm", ip->Pointed.Objid, &foe, 1,
			    ldh_eAccess_ReadOnly, pwr_cNOid);
  return 1;
}

/*----------------------------------------------------------------------------*\
  Build, i.e. compile the plcpgm.
\*----------------------------------------------------------------------------*/

static pwr_tStatus Build (
  ldh_sMenuCall *ip
)
{
  wb_build build( *(wb_session *)ip->PointedSession, ip->wnav);

  build.opt = ip->wnav->gbl.build;
  build.plcpgm( ip->Pointed.Objid);

  if ( build.sts() == PWRB__NOBUILT)
    ip->wnav->message( 'I', "Nothing to build");
  return build.sts();
}


/*----------------------------------------------------------------------------*\
  Syntax check.
\*----------------------------------------------------------------------------*/

static bool CheckChildCid( wb_object &o, pwr_tCid cid) 
{
  for ( wb_object child = o.first(); child; child = child.after()) {
    if ( child.cid() == cid) {
      return true;
    }
    if ( CheckChildCid( child, cid))
      return true;	 
  }
  return false;
}


static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tAttrRef Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated warning count */
) {
  wb_session *sp = (wb_session *)Session;
  pwr_tOid thread_oid;
  pwr_tOid reset_oid;

  // Check ThreadObject
  wb_attribute a = sp->attribute( Object.Objid, "RtBody", "ThreadObject");
  if (!a) return a.sts();
    
  a.value( &thread_oid);
  if ( !a) return a.sts();

  wb_object othread = sp->object( thread_oid);
  if ( !othread)
    wsx_error_msg_str( Session, "Bad thread object", Object, 'E', ErrorCount, WarningCount);
  else if ( othread.cid() != pwr_cClass_PlcThread)
    wsx_error_msg_str( Session, "Bad thread object class", Object, 'E', ErrorCount, WarningCount);
  

  // Check WindowPlc object
  bool found = 0;
  wb_object o = sp->object( Object.Objid);
  if ( !o) return o.sts();

  for ( wb_object child = o.first(); child; child = child.after()) {
    if ( child.cid() == pwr_cClass_windowplc) {
      found = 1;
      break;
    }
  }
  if ( !found)
    wsx_error_msg_str( Session, "Plc window is not created", Object, 'E', ErrorCount, WarningCount);


  // Check ResetObject if there is a grafcet sequence present
  if ( CheckChildCid( o, pwr_cClass_initstep)) {
    a = sp->attribute( Object.Objid, "DevBody", "ResetObject");
    if (!a) return a.sts();
    
    a.value( &reset_oid);
    if ( !a) return a.sts();

    wb_object oreset = sp->object( reset_oid);
    if ( !oreset)
      wsx_error_msg_str( Session, "Bad reset object", Object, 'E', ErrorCount, WarningCount);
    else if ( !(oreset.cid() == pwr_cClass_Dv ||
		oreset.cid() == pwr_cClass_Di ||
		oreset.cid() == pwr_cClass_Do))
      wsx_error_msg_str( Session, "Bad reset object class", Object, 'E', ErrorCount, WarningCount);      
  }

  return PWRB__SUCCESS;
}

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tOid	    Object,
  pwr_tOid	    Father,
  pwr_tCid	    Class
) {
  pwr_tOid oid;
  pwr_tOid toid;
  pwr_tStatus sts;
  int cnt = 0;

  sts = ldh_GetClassList( Session, pwr_cClass_PlcThread, &oid);
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

static pwr_tStatus AnteAdopt (
  ldh_tSesContext   Session,
  pwr_tOid	    Father,
  pwr_tCid	    Class
) {
  // Only allow plc window objects
  if ( Class != pwr_cClass_windowplc)
    return PWRB__PLCPGMCHILD;

  return PWRB__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(PlcPgm) = {
  pwr_BindMethod(OpenProgram),
  pwr_BindMethod(Build),
  pwr_BindMethod(SyntaxCheck),
  pwr_BindMethod(PostCreate),
  pwr_BindMethod(AnteAdopt),
  pwr_NullMethod
};
