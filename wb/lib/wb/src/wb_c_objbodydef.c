/* wb_c_classdef.c -- work bench methods of the ClassDef class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"

static pwr_tStatus AnteCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  pwr_tCid cid;
  pwr_tStatus sts;

  if ( cdh_ObjidIsNull( Father))
    return PWRS__POSOBJBODYDEF;

  // Check that the father is of class ClassDef
  sts = ldh_GetObjectClass( Session, Father, &cid);
  if ( EVEN(sts) || cid != pwr_eClass_ClassDef)
    return PWRS__POSOBJBODYDEF;
  return PWRS__SUCCESS;
}


static pwr_tStatus AnteMove (
  ldh_tSesContext   Session,
  pwr_tObjid	    Object,
  pwr_tObjid	    Father,
  pwr_tObjid	    OldFather
) {

  // Check that that the father is the same
  if ( !(Father.oix == OldFather.oix && Father.vid == OldFather.vid))
    return PWRS__MOVEBODY;
  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods($ObjBodyDef) = {
  pwr_BindMethod(AnteCreate),
  pwr_BindMethod(AnteMove),
  pwr_NullMethod
};





