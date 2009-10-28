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

/* wb_c_abb_acs800motoraggr.cpp -- work bench methods of the ABB_ACS800MotorAggr class */

#include <stdio.h>
#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrb_msg.h"
#include "pwr_abbclasses.h"
#include "wb_ldh.h"
#include "wb_wsx.h"
#include "wb_session.h"

//
//  Syntax check.
//

static pwr_tStatus SyntaxCheck( ldh_tSesContext Session,
				pwr_tAttrRef Object,
				int *ErrorCount,
				int *WarningCount)
{
  pwr_tStatus sts;
  pwr_tCid plcconnect_class[] = { pwr_cClass_ABB_ACS800MotorAggrFo, 0};
  pwr_tCid simconnect_class[] = { pwr_cClass_ABB_ACS800MotorAggrSim, 0};

  if ( Object.Offset == 0) {
    sts = wsx_CheckXAttrRef( Session, Object, "PlcConnect", "PlcConnect", plcconnect_class, 
			    0, ErrorCount, WarningCount);
    if ( EVEN(sts)) return sts;

    sts = wsx_CheckXAttrRef( Session, Object, "SimConnect", "PlcConnect", simconnect_class, 
			    1, ErrorCount, WarningCount);
    if ( EVEN(sts)) return sts;
  }
  return PWRB__SUCCESS;
}

//
//  Every method to be exported to the workbench should be registred here.
//

pwr_dExport pwr_BindMethods(ABB_ACS800MotorAggr) = {
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
