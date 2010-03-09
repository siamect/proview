/* 
 * Proview   $Id: wb_c_sevhist.cpp,v 1.1 2008-09-18 15:01:13 claes Exp $
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

/* wb_c_sevhistthread.cpp -- work bench methods of the SevHistThread class. */

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


//
//  Syntax check.
//
static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tAttrRef Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  wb_session *sp = (wb_session *)Session;
  pwr_tString40 server_name;

  wb_attribute a = sp->attribute( &Object);
  if ( !a) return a.sts();

  // Check ServerNode
  wb_attribute thread_a( a, 0, "ServerNode");
  if (!thread_a) return thread_a.sts();
    
  thread_a.value( &server_name);
  if ( !thread_a) return thread_a.sts();

  if ( strcmp( server_name, "") == 0)
    wsx_error_msg_str( Session, "Server name is missing", Object, 'E', ErrorCount, WarningCount);
  
  return PWRB__SUCCESS;
}


//  Every method to be exported to the workbench should be registred here.

pwr_dExport pwr_BindMethods(SevHistThread) = {
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
