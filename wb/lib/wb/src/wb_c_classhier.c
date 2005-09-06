/* 
 * Proview   $Id: wb_c_classhier.c,v 1.3 2005-09-06 10:43:30 claes Exp $
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

/* wb_c_classdef.c -- work bench methods of the ClassDef class. */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"

static pwr_tStatus AnteCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  // ClassHier should be a top object
  if ( Father.oix != 0)
    return PWRS__POSCLASSHIER;

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods($ClassHier) = {
  pwr_BindMethod(AnteCreate),
  pwr_NullMethod
};





