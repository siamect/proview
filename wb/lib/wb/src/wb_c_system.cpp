/* 
 * Proview   $Id: wb_c_ai.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

/* wb_c_ai.c -- work bench methods of the Ai class. */

#include <stdio.h>
#include <string.h>
#include "co_dcli.h"
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_pwrb_msg.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "wb_session.h"
#include "wb_wsx.h"


/*----------------------------------------------------------------------------*\
  Syntax check.
\*----------------------------------------------------------------------------*/

static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tAttrRef Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  wb_session *sp = (wb_session *)Session;
  pwr_tString80 str;

  wb_object o = sp->object( Object.Objid);
  wb_object p = o.parent();
  if ( !(!p || p.oid().oix == 0))
    wsx_error_msg_str( Session, "Not on top level", Object, 'E', ErrorCount, WarningCount);

  if ( Object.Objid.vid != ldh_cDirectoryVolume)
    wsx_error_msg_str( Session, "Not a DirectoryVolume", Object, 'E', ErrorCount, WarningCount);

  // Check SystemName
  wb_attribute a = sp->attribute( Object.Objid, "SysBody", "SystemName");
  if (!a) return a.sts();
    
  a.value( &str);
  if ( !a) return a.sts();

  dcli_trim( str, str);
  if ( strcmp( str, "") == 0)
    wsx_error_msg_str( Session, "SystemName is missing", Object, 'E', ErrorCount, WarningCount);

  // Check SystemGroup
  a = sp->attribute( Object.Objid, "SysBody", "SystemGroup");
  if (!a) return a.sts();
    
  a.value( &str);
  if ( !a) return a.sts();

  dcli_trim( str, str);
  if ( strcmp( str, "") == 0)
    wsx_error_msg_str( Session, "SystemGroup is missing", Object, 'E', ErrorCount, WarningCount);

  return PWRB__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods($System) = {
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};

