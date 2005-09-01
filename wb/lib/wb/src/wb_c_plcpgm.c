/** 
 * Proview   $Id: wb_c_plcpgm.c,v 1.3 2005-09-01 14:57:57 claes Exp $
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
#include "wb_wsx.h"
#include "wb_wsx_msg.h"


/*----------------------------------------------------------------------------*\
  To open a PLC program with the PLC editor.
\*----------------------------------------------------------------------------*/

static pwr_tStatus OpenProgram (
  ldh_sMenuCall *ip
)
{
  foe_Open (ip->EditorContext, ip->WindowContext, ip->PointedSession,
    ip->Pointed.Objid);

  return 1;
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
  pwr_tClassId class;

  /*
    Check that ScanTime is set to something.
  */

  sts = ldh_GetObjectPar( Session,
  			Object,
			"RtBody",
			"ThreadObject",
			(char **)&ThreadObjectPtr, &size);
  if ( EVEN(sts)) return sts;


  sts = ldh_GetObjectClass ( Session, *ThreadObjectPtr, &class);
  if ( EVEN(sts))
    wsx_error_msg( Session, WSX__PLCTHREAD, Object, ErrorCount, WarningCount);
  else if ( class != pwr_cClass_PlcThread)
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
    sts = ldh_GetObjectClass( Session, child, &class);
    if (EVEN(sts)) return sts;

    if ( class != pwr_cClass_windowplc)
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
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
