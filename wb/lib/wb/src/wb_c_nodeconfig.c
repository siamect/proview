/* wb_c_nodeconfig.c -- work bench methods of the NodeConfig class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include <string.h>
#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tObjid	    Object,
  pwr_tObjid	    Father,
  pwr_tClassId	    Class
) {
  pwr_tObjid oid, poid;
  pwr_tClassId cid;
  pwr_tStatus sts;
  char source[80];

  sts = ldh_ClassNameToId(Session, &cid, "RootVolumeLoad");
  sts = ldh_CreateObject(Session, &oid, "O1", cid, Object, ldh_eDest_IntoLast);

  sts = ldh_ClassNameToId(Session, &cid, "$NodeHier");
  sts = ldh_CreateObject(Session, &poid, "Distribute", cid, Object, ldh_eDest_IntoLast);

  sts = ldh_ClassNameToId(Session, &cid, "GraphDistribute");
  sts = ldh_CreateObject(Session, &oid, "AllPwg", cid, poid, ldh_eDest_IntoLast);

  strcpy( source, "$pwrp_pop/*.pwg");
  sts = ldh_SetObjectPar(Session, oid, "DevBody", "Source", source,
			 sizeof(source));
  if ( EVEN(sts)) return sts;

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods(NodeConfig) = {
  pwr_BindMethod(PostCreate),
  pwr_NullMethod
};

