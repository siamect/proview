/* 
 * Proview   $Id: wb_c_backup.cpp,v 1.1 2007-01-04 07:29:02 claes Exp $
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

/* wb_c_backup.c -- work bench methods of the Backup class. */

#include <string.h>
#include <X11/Intrinsic.h>
#undef Status
#include "pwr.h"
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_pwrb_msg.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"
#include "wb_wsx.h"
#include "wb_wsx_msg.h"


/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus PostCreate (
  ldh_tSesContext Session,
  pwr_tObjid	  Object,
  pwr_tObjid	  Father,
  pwr_tClassId	  Class
) {
  pwr_tStatus sts;
  int size;
  pwr_tAName Name;
  pwr_sAttrRef Attribute;
  
  /*
    If father of this object has an "ActualValue" attribute, then make this object
    refer to that attribute.
  */

  sts = ldh_ObjidToName(Session, Father, ldh_eName_Hierarchy, Name,
    sizeof(Name), &size);
  if (EVEN(sts)) return PWRB__SUCCESS;

  strcat(Name, ".ActualValue");

  sts = ldh_NameToAttrRef(Session, Name, &Attribute);
  if (EVEN(sts)) return PWRB__SUCCESS;

  sts = ldh_SetObjectPar(Session, Object, "RtBody", "DataName", (char *)&Attribute,
    sizeof(Attribute));
  if (EVEN(sts)) return PWRB__SUCCESS;

  return PWRB__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  
\*----------------------------------------------------------------------------*/

static pwr_tStatus PostMove (
  ldh_tSesContext Session,
  pwr_tObjid	  Object,
  pwr_tObjid	  Father,
  pwr_tClassId	  Class
) {
  pwr_tStatus sts;
  int size;
  pwr_tAName Name;
  pwr_sAttrRef Attribute;
  
  /*
    If father of ASup has an "ActualValue" attribute, then make this ASup
    refer to this attribute.
  */

  sts = ldh_ObjidToName(Session, Father, ldh_eName_Hierarchy, Name,
    sizeof(Name), &size);
  if (EVEN(sts)) return PWRB__SUCCESS;

  strcat(Name, ".ActualValue");

  sts = ldh_NameToAttrRef(Session, Name, &Attribute);
  if (EVEN(sts)) return PWRB__SUCCESS;

  sts = ldh_SetObjectPar(Session, Object, "RtBody", "DataName", Name,
    strlen(Name) + 1);
  if (EVEN(sts)) return PWRB__SUCCESS;

  return PWRB__SUCCESS;
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
  int size;
  char Name[255];
  pwr_sAttrRef Attribute;
  char *NamePtr;
  char *s;
  pwr_tClassId cid;
  
  /*
    If DataName is a signal, the attribute has to be ActualValue.
  */

  sts = ldh_GetObjectPar( Session,
  			Object,
			"RtBody",
			"DataName",
			(char **)&NamePtr, &size);
  if ( EVEN(sts)) return sts;
  strcpy( Name, NamePtr);
  free( NamePtr);

  if ( strcmp( Name, "") == 0)
    return PWRB__SUCCESS;
  
  sts = ldh_NameToAttrRef(Session, Name, &Attribute);
  if (EVEN(sts)) return PWRB__SUCCESS;
  
  sts = ldh_GetObjectClass ( Session, Attribute.Objid, &cid);
  if ( EVEN(sts)) return sts;

  switch ( cid)
  {
    case pwr_cClass_Do:
    case pwr_cClass_Dv:
    case pwr_cClass_Ao:
    case pwr_cClass_Av:
    case pwr_cClass_Po:
      s = strchr( Name, '.');
      if ( s == 0)
      {
        wsx_error_msg( Session, WSX__BCKINVALID, Object, ErrorCount, WarningCount);
        return PWRB__SUCCESS;
      }
      if ( strcmp( s, ".ActualValue") != 0)
      {
        wsx_error_msg( Session, WSX__BCKINVALID, Object, ErrorCount, WarningCount);
        return PWRB__SUCCESS;
      }
      break;
    case pwr_cClass_Co:
    case pwr_cClass_Di:
    case pwr_cClass_Ai:
      wsx_error_msg( Session, WSX__BCKINVALID, Object, ErrorCount, WarningCount);
      return PWRB__SUCCESS;
      break;
  }

  return PWRB__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Every method to be exported to the workbench should be registred here.
\*----------------------------------------------------------------------------*/

pwr_dExport pwr_BindMethods(Backup) = {
  pwr_BindMethod(PostCreate),
  pwr_BindMethod(PostMove),
  pwr_BindMethod(SyntaxCheck),
  pwr_NullMethod
};
