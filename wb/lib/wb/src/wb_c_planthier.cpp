/* 
 * Proview   $Id: wb_c_planthier.cpp,v 1.2 2007-01-04 07:29:03 claes Exp $
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

/* wb_c_planthier.c -- work bench methods of the PlantHier class. */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_pwrb_msg.h"
#include "wb_foe_msg.h"
#include "wb_ldh.h"
#include "wb_wsx.h"
#include "pwr_baseclasses.h"
#include "co_dcli.h"
#include "flow.h"
#include "flow_browctx.h"
#include "flow_browapi.h"
#include "wb_wnav.h"
#include "wb_build.h"
#include "wb_wsx.h"
#include "cow_msgwindow.h"

/*----------------------------------------------------------------------------*\
  Build volume.
\*----------------------------------------------------------------------------*/
static pwr_tStatus Build (
  ldh_sMenuCall *ip
)
{
  wb_build build( *(wb_session *)ip->PointedSession, ip->wnav);

  build.opt = ip->wnav->gbl.build;
  build.planthier( ip->Pointed.Objid);

  if ( build.sts() == PWRB__NOBUILT)
    ip->wnav->message( 'I', "Nothing to build");
  return build.sts();
}

//
// Syntax Check
//
static pwr_tStatus SyntaxCheck( ldh_tSesContext Session,
				pwr_tAttrRef Object,
				int *ErrorCount,
				int *WarningCount) 
{
  pwr_tStatus sts;
  pwr_tCid defgraph_class[] = { pwr_cClass_XttGraph, 0};
  pwr_tCid deftrend_class[] = { pwr_cClass_DsTrend, pwr_cClass_DsFast, pwr_cClass_DsFastCurve,
				pwr_cClass_PlotGroup, 0};

  sts = wsx_CheckAttrRef( Session, Object, "DefGraph", defgraph_class, 1, ErrorCount, WarningCount);
  if ( EVEN(sts)) return sts;
  
  sts = wsx_CheckAttrRef( Session, Object, "DefTrend", deftrend_class, 1, ErrorCount, WarningCount);
  if ( EVEN(sts)) return sts;

  return PWRS__SUCCESS;
}  

/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods($PlantHier) = {
  pwr_BindMethod(Build),
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};

