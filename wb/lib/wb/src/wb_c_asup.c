/* wb_c_asup.c -- work bench methods of the ASup class

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_pwrb_msg.h"
#include "pwr_baseclasses.h"
#include "wb_ldh.h"


static pwr_tStatus
PostCreate (
  ldh_tSesContext Session,
  pwr_tObjid	  Object,
  pwr_tObjid	  Father,
  pwr_tClassId	  Class
) {
  pwr_tStatus sts;
  int size;
  char Name[255];
  pwr_sAttrRef Attribute;
  
  /*  If father of ASup has an "ActualValue" attribute, then make this ASup
      refer to this attribute.  */

  sts = ldh_ObjidToName(Session, Father, ldh_eName_Hierarchy, Name,
    sizeof(Name), &size);
  if (EVEN(sts)) return PWRB__SUCCESS;

  strcat(Name, ".ActualValue");

  sts = ldh_NameToAttrRef(Session, Name, &Attribute);
  if (EVEN(sts)) {
    memset(&Attribute, 0, sizeof(Attribute));
  }

  sts = ldh_SetObjectPar(Session, Object, "RtBody", "Attribute",
                         (char *)&Attribute, sizeof(Attribute));
  if (EVEN(sts)) return PWRB__SUCCESS;

  return PWRB__SUCCESS;
}

static pwr_tStatus
PostMove (
  ldh_tSesContext Session,
  pwr_tObjid	  Object,
  pwr_tObjid	  Father,
  pwr_tClassId	  Class
) {
  pwr_tStatus sts;
  int size;
  char Name[255];
  pwr_sAttrRef Attribute;
  
  /*  If father of ASup has an "ActualValue" attribute, then make this ASup
      refer to this attribute.  */

  sts = ldh_ObjidToName(Session, Father, ldh_eName_Hierarchy, Name,
    sizeof(Name), &size);
  if (EVEN(sts)) return PWRB__SUCCESS;

  strcat(Name, ".ActualValue");

  sts = ldh_NameToAttrRef(Session, Name, &Attribute);

  if (EVEN(sts)) {
    memset(&Attribute, 0, sizeof(Attribute));
  }

  sts = ldh_SetObjectPar(Session, Object, "RtBody", "Attribute",
                         (char *)&Attribute, sizeof(Attribute));

  if (EVEN(sts)) return PWRB__SUCCESS;

  return PWRB__SUCCESS;
}

/*  Every method to be exported to the workbench should be registred here.  */

pwr_dExport pwr_BindMethods(ASup) = {
  pwr_BindMethod(PostCreate),
  pwr_BindMethod(PostMove),
  pwr_NullMethod
};
