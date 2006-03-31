/* 
 * Proview   $Id: wb_c_rootvolume.cpp,v 1.1 2006-03-31 14:24:34 claes Exp $
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

/* wb_c_rootvolume.c -- work bench methods of the RootVolume class. */

#include <string.h>
#include <X11/Intrinsic.h>
#undef Status
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_pwrb_msg.h"
#include "wb_foe_msg.h"
#include "wb_ldh.h"
#include "wb_wsx.h"
#include "pwr_baseclasses.h"
#include "co_dcli.h"
#include "wb_lfu.h"

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
#include "wb_wtt.h"
#include "co_msgwindow.h"


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

  sts = wsx_CheckVolume( Session, Object, ErrorCount, WarningCount);
  if ( EVEN(sts)) return sts;

  return PWRS__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods($RootVolume) = {
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
