/* 
 * Proview   $Id: wb_c_plcpgm.cpp,v 1.2 2006-04-26 04:45:46 claes Exp $
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

#include <X11/Intrinsic.h>
#undef Status
#include "wb_pwrs.h"
#include "pwr_baseclasses.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_foe_api.h"
#include "wb_pwrb_msg.h"
#include "wb_foe_msg.h"
#include "wb_wsx.h"
#include "wb_wsx_msg.h"
#include "wb_utl.h"

#include <Xm/Xm.h>
#include <Xm/XmP.h>
#include <Mrm/MrmPublic.h>
#include <X11/Intrinsic.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "flow_browwidget.h"
#include "wb_wnav.h"
#include "wb_build.h"


/*----------------------------------------------------------------------------*\
  To open a PLC program with the PLC editor.
\*----------------------------------------------------------------------------*/

static pwr_tStatus OpenProgram (
  ldh_sMenuCall *ip
)
{
  foe_Open (ip->EditorContext2, ip->WindowContext, ip->PointedSession,
    ip->Pointed.Objid);

  return 1;
}

/*----------------------------------------------------------------------------*\
  Build, i.e. compile the plcpgm.
\*----------------------------------------------------------------------------*/

static pwr_tStatus Build (
  ldh_sMenuCall *ip
)
{
  WNav *wnav = (WNav *)ip->EditorContext;
  wb_build build( *(wb_session *)ip->PointedSession, wnav, ip->WindowContext);

  build.opt = wnav->gbl.build;
  build.plcpgm( ip->Pointed.Objid);

  if ( build.sts() == PWRB__NOBUILT)
    wnav->message( 'I', "Nothing to build");
  return build.sts();
}


/*----------------------------------------------------------------------------*\
  Syntax check.
\*----------------------------------------------------------------------------*/

static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tObjid Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  pwr_tStatus sts;
  pwr_tObjid *ThreadObjectPtr;
  int size;
  pwr_tObjid child;
  pwr_tClassId cid;

  /*
    Check that ScanTime is set to something.
  */

  sts = ldh_GetObjectPar( Session,
  			Object,
			"RtBody",
			"ThreadObject",
			(char **)&ThreadObjectPtr, &size);
  if ( EVEN(sts)) return sts;


  sts = ldh_GetObjectClass ( Session, *ThreadObjectPtr, &cid);
  if ( EVEN(sts))
    wsx_error_msg( Session, WSX__PLCTHREAD, Object, ErrorCount, WarningCount);
  else if ( cid != pwr_cClass_PlcThread)
    wsx_error_msg( Session, WSX__PLCTHREAD, Object, ErrorCount, WarningCount);

  free( (char *)ThreadObjectPtr);

  /*
    Check that the child is a plcwindow.
  */

  sts = ldh_GetChild( Session, Object, &child);
  if (EVEN(sts))
  {
    wsx_error_msg( Session, WSX__PLCWIND, Object, ErrorCount, WarningCount);
  }
  else
  {
    sts = ldh_GetObjectClass( Session, child, &cid);
    if (EVEN(sts)) return sts;

    if ( cid != pwr_cClass_windowplc)
      wsx_error_msg( Session, WSX__PLCCHILD, Object, ErrorCount, WarningCount);

    sts = ldh_GetNextSibling( Session, child, &child);
    if ( ODD(sts))
      wsx_error_msg( Session, WSX__PLCCHILD, Object, ErrorCount, WarningCount);
  }
  return PWRB__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(PlcPgm) = {
  pwr_BindMethod(OpenProgram),
  pwr_BindMethod(Build),
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
