/* 
 * Proview   $Id: wb_c_objxref.cpp,v 1.1 2007-01-04 07:29:03 claes Exp $
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

/* wb_c_objxref.c -- work bench methods of the ObjXRef class. */

#include <stdio.h>
#include <string.h>
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_pwrs_msg.h"
#include "co_cdh.h"


/*----------------------------------------------------------------------------*\
  Is it OK to connect two attributes via an ObjXRef
\*----------------------------------------------------------------------------*/

static int IsOkConnect (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus	    sts;
  pwr_sAttrRef	    PattrRef;
  pwr_sAttrRef	    SattrRef;
  pwr_sObjXRef	    PobjXDef;
  pwr_sObjXRef	    SobjXDef;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
    mbp->MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return 0;

  sts = ldh_GetObjXRefDef(ip->PointedSession, &PattrRef, &PobjXDef);
  if (EVEN(sts)) return 0;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Selected[0].Objid,
    PobjXDef.TargetAttribute, &SattrRef);
  if (EVEN(sts)) return 0;

  sts = ldh_GetObjXRefDef(ip->PointedSession, &SattrRef, &SobjXDef);
  if (EVEN(sts)) return 0;

  if (strncmp(PobjXDef.Identity, SobjXDef.Identity, sizeof(SobjXDef.Identity)) != 0)
    return 0;

  if (strncmp(PobjXDef.Source, SobjXDef.Target, sizeof(PobjXDef.Source)) != 0)
    return 0;

  if (strncmp(PobjXDef.Target, SobjXDef.Source, sizeof(SobjXDef.Source)) != 0)
    return 0;

  return 1;
}

/*----------------------------------------------------------------------------*\
  To connect two attributes via an ObjXRef
\*----------------------------------------------------------------------------*/

static pwr_tStatus Connect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    sts;
  pwr_sMenuButton   mb;
  pwr_sAttrRef	    PattrRef;
  pwr_tObjid	    Pobjid;
  pwr_sAttrRef	    SattrRef;
  pwr_tObjid	    Sobjid;
  pwr_sObjXRef	    PobjXDef;
  pwr_sObjXRef	    SobjXDef;

  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
    mb.MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_GetObjXRefDef(ip->PointedSession, &PattrRef, &PobjXDef);
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Selected[0].Objid,
    PobjXDef.TargetAttribute, &SattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_GetObjXRefDef(ip->PointedSession, &SattrRef, &SobjXDef);
  if (EVEN(sts)) return sts;

  if (strncmp(PobjXDef.Identity, SobjXDef.Identity, sizeof(SobjXDef.Identity)) != 0)
    return PWRS__IDXREF;

  if (strncmp(PobjXDef.Source, SobjXDef.Target, sizeof(PobjXDef.Source)) != 0)
    return PWRS__STXREF;

  if (strncmp(PobjXDef.Target, SobjXDef.Source, sizeof(SobjXDef.Source)) != 0)
    return PWRS__STXREF;

  sts = ldh_ReadAttribute(ip->PointedSession, &PattrRef, &Pobjid,
    sizeof(pwr_tObjid));
  if (EVEN(sts)) return sts;
  sts = ldh_ReadAttribute(ip->PointedSession, &SattrRef, &Sobjid,
    sizeof(pwr_tObjid));
  if (EVEN(sts)) return sts;

  if (cdh_ObjidIsEqual(Pobjid, ip->Selected[0].Objid) &&
    cdh_ObjidIsEqual(Sobjid, ip->Pointed.Objid)
  ) {
    return PWRS__ALRCONN;
  } else if (cdh_ObjidIsEqual(Pobjid, ip->Selected[0].Objid)){
    return PWRS__PERRCONN;
  } else if (cdh_ObjidIsEqual(Sobjid, ip->Pointed.Objid)) {
    return PWRS__SERRCONN;
  } else if (!cdh_ObjidIsNull(Pobjid)) {
    return PWRS__PALRCONN;  
  } else if (!cdh_ObjidIsNull(Sobjid)) {
    return PWRS__SALRCONN;  
  }

  Pobjid = ip->Pointed.Objid;
  Sobjid = ip->Selected[0].Objid;

  sts = ldh_WriteAttribute(ip->PointedSession, &SattrRef, &Pobjid,
    sizeof(pwr_tObjid));
  sts = ldh_WriteAttribute(ip->PointedSession, &PattrRef, &Sobjid,
    sizeof(pwr_tObjid));
  return PWRS__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Is it OK to disconnect two attributes via an ObjXRef
\*----------------------------------------------------------------------------*/

static int IsOkDisconnect (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {

  return IsOkConnect(ip, mbp);
}

/*----------------------------------------------------------------------------*\
  To disconnect two attributes via an ObjXRef
\*----------------------------------------------------------------------------*/

static pwr_tStatus Disconnect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    sts = PWRS__SUCCESS;
  pwr_tStatus	    psts = PWRS__SUCCESS;
  pwr_tStatus	    ssts = PWRS__SUCCESS;
  pwr_sMenuButton   mb;
  pwr_tObjid	    Nobjid = pwr_cNObjid;
  pwr_sAttrRef	    PattrRef;
  pwr_tObjid	    Pobjid;
  pwr_sAttrRef	    SattrRef;
  pwr_tObjid	    Sobjid;
  pwr_sObjXRef	    PobjXDef;
  pwr_sObjXRef	    SobjXDef;

  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
    mb.MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_GetObjXRefDef(ip->PointedSession, &PattrRef, &PobjXDef);
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Selected[0].Objid,
    PobjXDef.TargetAttribute, &SattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_GetObjXRefDef(ip->PointedSession, &SattrRef, &SobjXDef);
  if (EVEN(sts)) return sts;

  if (strncmp(PobjXDef.Identity, SobjXDef.Identity, sizeof(SobjXDef.Identity)) != 0)
    return PWRS__IDXREF;

  if (strncmp(PobjXDef.Source, SobjXDef.Target, sizeof(PobjXDef.Source)) != 0)
    return PWRS__STXREF;

  if (strncmp(PobjXDef.Target, SobjXDef.Source, sizeof(SobjXDef.Source)) != 0)
    return PWRS__STXREF;

  sts = ldh_ReadAttribute(ip->PointedSession, &PattrRef, &Pobjid,
    sizeof(pwr_tObjid));
  if (EVEN(sts)) return sts;
  sts = ldh_ReadAttribute(ip->PointedSession, &SattrRef, &Sobjid,
    sizeof(pwr_tObjid));
  if (EVEN(sts)) return sts;

  if (cdh_ObjidIsEqual(Pobjid, ip->Selected[0].Objid)) {
    psts = ldh_WriteAttribute(ip->PointedSession, &PattrRef, &Nobjid,
      sizeof(pwr_tObjid));
    if (EVEN(psts))
      return psts;
    else
      psts = PWRS__PWASCONN;
  } else {
    psts = PWRS__PNOTCONN;
  }
  if (cdh_ObjidIsEqual(Sobjid, ip->Pointed.Objid)) {
    ssts = ldh_WriteAttribute(ip->PointedSession, &SattrRef, &Nobjid,
      sizeof(pwr_tObjid));
    if (EVEN(ssts))
      return ssts;
    else
      ssts = PWRS__SWASCONN;
  } else {
    ssts = PWRS__SNOTCONN;
  }

  if (psts == PWRS__PWASCONN && ssts == PWRS__SWASCONN) {
    return PWRS__SUCCESS;
  } else if (psts == PWRS__PNOTCONN && ssts == PWRS__SNOTCONN) {
    return PWRS__NOTCONN;
  } else if (psts == PWRS__PWASCONN) {
    return PWRS__PWERRCONN;
  } else if (ssts == PWRS__SWASCONN) {
    return PWRS__SWERRCONN;
  }

  return PWRS__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Is it OK to disconnect two attributes via an ObjXRef
\*----------------------------------------------------------------------------*/


static int IsOkDisconnectSingle (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus sts;

  sts = ldh_CheckObjXRef(ip->PointedSession, ip->Pointed.Objid,
    mbp->MethodArguments[0]);

  return sts != LDH__UNCONN;
}

/*----------------------------------------------------------------------------*\
  To disconnect two attributes via an ObjXRef
\*----------------------------------------------------------------------------*/

static pwr_tStatus DisconnectSingle (
  ldh_sMenuCall *ip
) {
  pwr_tStatus	    sts = PWRS__SUCCESS;
  pwr_sMenuButton   mb;
  pwr_tObjid	    Nobjid = pwr_cNObjid;
  pwr_tObjid	    Pobject = ip->Pointed.Objid;
  pwr_sAttrRef	    PattrRef;
  pwr_tObjid	    Sobject;
  pwr_sAttrRef	    SattrRef;
  pwr_tObjid	    Sobjid;
  pwr_sObjXRef	    PobjXDef;
  pwr_sObjXRef	    SobjXDef;
  pwr_tBoolean	    DiscSobject;

  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrRef(ip->PointedSession, Pobject,
    mb.MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_GetObjXRefDef(ip->PointedSession, &PattrRef, &PobjXDef);
  if (EVEN(sts)) return sts;

  sts = ldh_ReadAttribute(ip->PointedSession, &PattrRef, &Sobject,
    sizeof(pwr_tObjid));
  if (EVEN(sts)) return sts;

  if (cdh_ObjidIsNull(Sobject))
    return PWRS__PNOTCONN;

  while (1) {
    DiscSobject = 0;
    
    sts = ldh_GetAttrRef(ip->PointedSession, Sobject,
      PobjXDef.TargetAttribute, &SattrRef);
    if (EVEN(sts)) break;

    sts = ldh_GetObjXRefDef(ip->PointedSession, &SattrRef, &SobjXDef);
    if (EVEN(sts)) break;

    if (strncmp(PobjXDef.Identity, SobjXDef.Identity, sizeof(SobjXDef.Identity)) != 0)
      break;

    if (strncmp(PobjXDef.Source, SobjXDef.Target, sizeof(PobjXDef.Source)) != 0)
      break;

    if (strncmp(PobjXDef.Target, SobjXDef.Source, sizeof(SobjXDef.Source)) != 0)
      break;

    sts = ldh_ReadAttribute(ip->PointedSession, &SattrRef, &Sobjid,
      sizeof(pwr_tObjid));
    if (EVEN(sts))
      break;

    DiscSobject = cdh_ObjidIsEqual(Sobjid, Pobject);
    break;
  }

  sts = ldh_WriteAttribute(ip->PointedSession, &PattrRef, &Nobjid,
    sizeof(pwr_tObjid));
  if (EVEN(sts))
    return sts;

  if (DiscSobject) {
    sts = ldh_WriteAttribute(ip->PointedSession, &SattrRef, &Nobjid,
      sizeof(pwr_tObjid));
  }

  return PWRS__SUCCESS;
}

pwr_dExport pwr_BindMethods($ObjXRef) = {
  pwr_BindMethod(Connect),
  pwr_BindMethod(IsOkConnect),
  pwr_BindMethod(Disconnect),
  pwr_BindMethod(IsOkDisconnect),
  pwr_BindMethod(DisconnectSingle),
  pwr_BindMethod(IsOkDisconnectSingle),
  pwr_NullMethod
};
