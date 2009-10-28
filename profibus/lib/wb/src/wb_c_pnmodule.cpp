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

/* wb_c_pnmodule.c -- work bench methods of the PnModule class. */

#include <stdio.h>
#include <string.h>

#include "pwr.h"
#include "pwr_baseclasses.h"
#include "pwr_profibusclasses.h"
#include "wb_env.h"
#include "co_msg.h"
#include "co_cdh.h"
#include "co_dcli.h"
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_pwrb_msg.h"
#include "rt_pb_msg.h"
#include "wb_wsx.h"
#include "wb_session.h"

//
//  Syntax check.
//

static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tAttrRef Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  return wsx_CheckIoDevice( Session, Object, ErrorCount, WarningCount, wsx_mCardOption_None);
}

//
//  Every method to be exported to the workbench should be registred here.
//

pwr_dExport pwr_BindMethods(PnModule) = {
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};


