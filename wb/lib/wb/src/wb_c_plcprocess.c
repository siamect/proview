/* wb_c_plcprocess.c -- work bench methods of the PlcProcess class.

   PROVIEW/R
   Copyright (C) 1994 by Comator Process AB.  */

#include "wb_pwrs.h"
#include "wb_pwrs_msg.h"
#include "wb_ldh.h"

static pwr_tStatus PostCreate (
  ldh_tSesContext   Session,
  pwr_tOid	    Object,
  pwr_tOid	    Father,
  pwr_tCid	    Class
) {
  pwr_tOid oid;
  pwr_tCid cid;
  pwr_tStatus sts;
  pwr_tFloat32 scan_time = 0.1;

  sts = ldh_ClassNameToId(Session, &cid, "PlcThread");
  if ( EVEN(sts)) return sts;

  sts = ldh_CreateObject(Session, &oid, "100ms", cid, Object, ldh_eDest_IntoLast); 
  if ( EVEN(sts)) return sts;

  sts = ldh_SetObjectPar(Session, oid, "RtBody", "ScanTime", (char *)&scan_time,
			 sizeof(scan_time));
  if ( EVEN(sts)) return sts;

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods(PlcProcess) = {
  pwr_BindMethod(PostCreate),
  pwr_NullMethod
};







