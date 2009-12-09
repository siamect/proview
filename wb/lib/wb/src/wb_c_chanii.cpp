/* 
 * Proview   $Id: wb_c_chanii.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

/* wb_c_chanii.c -- work bench methods of the ChanIi class. */

#include <stdio.h>
#include <string.h>
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_pwrb_msg.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "wb_wsx.h"
#include "wb_session.h"


/*----------------------------------------------------------------------------*\
  Syntax check.
\*----------------------------------------------------------------------------*/

static pwr_tStatus SyntaxCheck (
  ldh_tSesContext Session,
  pwr_tAttrRef Object,	      /* current object */
  int *ErrorCount,	      /* accumulated error count */
  int *WarningCount	      /* accumulated waring count */
) {
  // pwr_tStatus sts;

  // sts = wsx_CheckSigChanCon( Session, Object, ErrorCount, WarningCount);
  // if (EVEN(sts)) return sts;

  return PWRB__SUCCESS;
}

static pwr_tStatus PostCreate( ldh_tSesContext Session,
			       pwr_tObjid	  Object,
			       pwr_tObjid	  Father,
			       pwr_tClassId	  Class) 
{
  wb_session *sp = (wb_session *)Session;
  int repr_set = 0;

  // Fetch Representation from previous sibling
  wb_object o = sp->object( Object);
  if ( !o) return o.sts();

  wb_object before = o.before();
  if ( before && before.cid() == o.cid()) {

    // Set Representation
    wb_attribute ba_repr = sp->attribute( before.oid(), "RtBody", "Representation");
    if ( !ba_repr) return ba_repr.sts();

    pwr_eDataRepEnum repr;
    ba_repr.value( &repr);

    wb_attribute a_repr = sp->attribute( Object, "RtBody", "Representation");
    if ( !a_repr) return a_repr.sts();

    try {
      sp->writeAttribute( a_repr, &repr, sizeof(repr));
    }
    catch ( wb_error& e) {
      return e.sts();
    }
    if ( sp->evenSts()) return sp->sts();

    repr_set = 1;
  }

  if ( !repr_set) {
    wb_cdef father_cdef = sp->cdef(Class);
    if ( strcmp( father_cdef.name(), "Modbus_TCP_ServerModule") == 0 ||
	 strcmp( father_cdef.name(), "Modbus_Module") == 0) {
      
      wb_attribute a = sp->attribute( Object, "RtBody", "Representation");
      if ( !a) return a.sts();
      
      pwr_eDataRepEnum value = pwr_eDataRepEnum_Int16;
      try {
	sp->writeAttribute( a, &value, sizeof(value));
      }
      catch ( wb_error& e) {
	return e.sts();
      }
      if ( sp->evenSts()) return sp->sts();
    }
  }

  return PWRB__SUCCESS;
}


/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(ChanIi) = {
  pwr_BindMethod(SyntaxCheck),
  pwr_BindMethod(PostCreate),
  pwr_NullMethod
};
