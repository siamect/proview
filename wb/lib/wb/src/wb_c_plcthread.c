/* wb_c_plcthread.c -- work bench methods of the PlcThread class.

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
  pwr_tEnum delay_action;
  pwr_tString80 detect_text = "Plc thread delayed";
  pwr_tFloat32 max_delay;

  sts = ldh_ClassNameToId(Session, &cid, "CycleSup");
  if ( EVEN(sts)) return sts;

  sts = ldh_CreateObject(Session, &oid, "Alarm", cid, Object, ldh_eDest_IntoLast); 
  if ( EVEN(sts)) return sts;

  delay_action = 1;
  sts = ldh_SetObjectPar(Session, oid, "RtBody", "DelayAction", (char *)&delay_action,
			 sizeof(delay_action));
  if ( EVEN(sts)) return sts;

  max_delay = 0.1;
  sts = ldh_SetObjectPar(Session, oid, "RtBody", "MaxDelay", (char *)&max_delay,
			 sizeof(max_delay));
  if ( EVEN(sts)) return sts;

  sts = ldh_SetObjectPar(Session, oid, "RtBody", "DetectText", (char *)&detect_text,
			 sizeof(detect_text));
  if ( EVEN(sts)) return sts;

  sts = ldh_CreateObject(Session, &oid, "Halt", cid, Object, ldh_eDest_IntoLast); 
  if ( EVEN(sts)) return sts;

  delay_action = 2;
  sts = ldh_SetObjectPar(Session, oid, "RtBody", "DelayAction", (char *)&delay_action,
			 sizeof(delay_action));
  if ( EVEN(sts)) return sts;

  max_delay = 1.0;
  sts = ldh_SetObjectPar(Session, oid, "RtBody", "MaxDelay", (char *)&max_delay,
			 sizeof(max_delay));
  if ( EVEN(sts)) return sts;

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods(PlcThread) = {
  pwr_BindMethod(PostCreate),
  pwr_NullMethod
};

