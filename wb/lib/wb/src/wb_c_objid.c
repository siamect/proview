/* wb_c_objid.c -- work bench methods of the Objid class.

   PROVIEW/R
   Copyright (C) 1994, 1996 by Comator Process AB.  */

#include <stdio.h>
#include <string.h>
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_dutl.h"
#include "wb_pwrs_msg.h"
#include "co_cdh.h"


/*----------------------------------------------------------------------------*\
  Is it OK to connect an object ?
\*----------------------------------------------------------------------------*/

static int IsOkConnect (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	sts;
  pwr_tCid	cid;
  pwr_tCid	arg_cid;

  if ( strcmp( mbp->FilterArguments[1], "") != 0) {

    // arg 1 holds the allowed class to connect to
    sts = ldh_ClassNameToId( ip->PointedSession, &arg_cid, mbp->FilterArguments[1]);
    if ( EVEN(sts)) return 0;

    sts = ldh_GetObjectClass( ip->PointedSession, ip->Selected[0].Objid, &cid);
    if ( EVEN(sts)) return 0;

    if ( cid != arg_cid) 
      return 0;
  }

  return 1;
}

/*----------------------------------------------------------------------------*\
  To connect an object to Objid
\*----------------------------------------------------------------------------*/

static pwr_tStatus Connect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    	sts;
  pwr_sMenuButton   	mb;
  pwr_sAttrRef	    	PattrRef;
  pwr_tCid		cid;
  pwr_tCid		arg_cid;


  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  if ( strcmp( mb.MethodArguments[1], "") != 0) {

    // arg 1 holds the allowed class to connect to
    sts = ldh_ClassNameToId( ip->PointedSession, &arg_cid, mb.MethodArguments[1]);
    if ( EVEN(sts)) return 0;

    sts = ldh_GetObjectClass( ip->PointedSession, ip->Selected[0].Objid, &cid);
    if ( EVEN(sts)) return 0;

    if ( cid != arg_cid) 
      return 0;
  }

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
    mb.MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_WriteAttribute(ip->PointedSession, &PattrRef, &ip->Selected[0].Objid,
    sizeof(pwr_tObjid));

  return PWRS__SUCCESS;
}


static int IsOkDisconnect (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	    	sts;
  pwr_sAttrRef	    	PattrRef;
  pwr_tOid		oid;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
		       mbp->FilterArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_ReadAttribute( ip->PointedSession, &PattrRef, &oid, sizeof(pwr_tOid));

  if ( cdh_ObjidIsNull( oid))
    return 0;

  return 1;
}
/*----------------------------------------------------------------------------*\
  To disconnect an objid attribute
\*----------------------------------------------------------------------------*/

static pwr_tStatus Disconnect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    	sts;
  pwr_sMenuButton   	mb;
  pwr_sAttrRef	    	PattrRef;
  pwr_tOid		oid;

  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
		       mb.MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;

  oid = pwr_cNObjid;
  sts = ldh_WriteAttribute( ip->PointedSession, &PattrRef, &oid, sizeof(pwr_tOid));

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods($Objid) = {
  pwr_BindMethod(Connect),
  pwr_BindMethod(IsOkConnect),
  pwr_BindMethod(Disconnect),
  pwr_BindMethod(IsOkDisconnect),
  pwr_NullMethod
};







