/* wb_c_classdef.c -- work bench methods of the ClassDef class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <X11/Intrinsic.h>
#undef Status
#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"
#include "ge.h"

static pwr_tStatus AnteCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  pwr_tCid cid;
  pwr_tStatus sts;

  if ( cdh_ObjidIsNull( Father))
    return PWRS__POSCLASSDEF;

  // Check that the father is of class ClassHier
  sts = ldh_GetObjectClass( Session, Father, &cid);
  if ( EVEN(sts) || cid != pwr_eClass_ClassHier)
    return PWRS__POSCLASSDEF;
  return PWRS__SUCCESS;
}

static pwr_tStatus OpenClassGraph (
  ldh_sMenuCall *ip
)
{
  int 		sts;
  char		name[32];
  int		size;
 
  sts = ldh_ObjidToName( ip->PointedSession, ip->Pointed.Objid, ldh_eName_Object, 
	name, sizeof(name), &size);
  if ( EVEN(sts)) return sts;

  cdh_ToLower( name, name);
  ge_new( NULL, ip->WindowContext, ip->PointedSession, 0, name);
  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods($ClassDef) = {
  pwr_BindMethod(AnteCreate),
  pwr_BindMethod(OpenClassGraph),
  pwr_NullMethod
};





