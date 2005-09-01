/** 
 * Proview   $Id: wb_c_attrxref.c,v 1.2 2005-09-01 14:57:57 claes Exp $
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

/* wb_c_attrxref.c -- work bench methods for AttrXRef */

#include <stdio.h>
#include <string.h>
#include "pwr.h"
#include "wb_pwrs.h"
#include "wb_ldh_msg.h"
#include "wb_ldh.h"
#include "wb_dutl.h"
#include "wb_pwrs_msg.h"
#include "co_cdh.h"



/*----------------------------------------------------------------------------*\
  Is it OK to connect two attributes via an AttrXRef
\*----------------------------------------------------------------------------*/

static int IsOkConnect (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus sts;
  pwr_sAttrRef PattrRef;
  pwr_sAttrRef SattrRef;
  pwr_sAttrXRef PattrDef;
  pwr_sAttrXRef SattrDef;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
    mbp->MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return 0;
  sts = ldh_GetAttrRef(ip->PointedSession, ip->Selected[0].Objid,
    mbp->MethodArguments[1], &SattrRef);
  if (EVEN(sts)) return 0;

  sts = ldh_GetAttrXRefDef(ip->PointedSession, &PattrRef, &PattrDef);
  if (EVEN(sts)) return sts;
  sts = ldh_GetAttrXRefDef(ip->PointedSession, &SattrRef, &SattrDef);
  if (EVEN(sts)) return 0;

  if (strncmp(PattrDef.Identity, SattrDef.Identity, sizeof(SattrDef.Identity)) != 0)
    return 0;

  if (strncmp(PattrDef.Source, SattrDef.Target, sizeof(PattrDef.Source)) != 0)
    return 0;

  if (strncmp(PattrDef.Target, SattrDef.Source, sizeof(SattrDef.Source)) != 0)
    return 0;

  return 1;
}

/*----------------------------------------------------------------------------*\
  To connect two attributes via an AttrXRef
\*----------------------------------------------------------------------------*/

static pwr_tStatus Connect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus sts;
  pwr_sMenuButton mb;
  pwr_sAttrRef PattrRef;
  pwr_sAttrRef PattrXref;
  pwr_sAttrRef SattrRef;
  pwr_sAttrRef SattrXref;
  pwr_sAttrRef TattrRef;
  pwr_tUInt32 Pdim;
  pwr_tUInt32 Sdim;
  pwr_sAttrXRef PattrDef;
  pwr_sAttrXRef SattrDef;
  int i;

  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
    mb.MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;
  sts = ldh_GetAttrRef(ip->PointedSession, ip->Selected[0].Objid,
    mb.MethodArguments[1], &SattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrXRefDef(ip->PointedSession, &PattrRef, &PattrDef);
  if (EVEN(sts)) return sts;
  sts = ldh_GetAttrXRefDef(ip->PointedSession, &SattrRef, &SattrDef);
  if (EVEN(sts)) return sts;

  if (strncmp(PattrDef.Identity, SattrDef.Identity, sizeof(SattrDef.Identity)) != 0)
    return PWRS__IDXREF;

  if (strncmp(PattrDef.Source, SattrDef.Target, sizeof(PattrDef.Source)) != 0)
    return PWRS__STXREF;

  if (strncmp(PattrDef.Target, SattrDef.Source, sizeof(SattrDef.Source)) != 0)
    return PWRS__STXREF;

  Pdim = PattrRef.Size / sizeof(pwr_sAttrRef);
  Sdim = SattrRef.Size / sizeof(pwr_sAttrRef);

  TattrRef = PattrRef;
  TattrRef.Size = sizeof(pwr_sAttrRef);
  for (i = 0; i < Pdim; i++, TattrRef.Offset += sizeof(pwr_sAttrRef)) {
    /* find Sobject */
    sts = ldh_ReadAttribute(ip->PointedSession, &TattrRef, &PattrXref,
      sizeof(pwr_sAttrRef));
    if (EVEN(sts)) return sts;
    if (!cdh_ObjidIsEqual(PattrXref.Objid, ip->Selected[0].Objid))
      continue;
    sts = ldh_ReadAttribute(ip->PointedSession, &PattrXref, &SattrXref,
      sizeof(pwr_sAttrRef));
    if (EVEN(sts)) return sts;
    if (memcmp(&SattrXref, &TattrRef, sizeof(pwr_sAttrRef)) == 0)
      return PWRS__ALRCONN;
  }

  TattrRef = PattrRef;
  TattrRef.Size = sizeof(pwr_sAttrRef);
  for (i = 0; i < Pdim; i++, TattrRef.Offset += sizeof(pwr_sAttrRef)) {
    /* find free slot */
    sts = ldh_ReadAttribute(ip->PointedSession, &TattrRef, &PattrXref,
      sizeof(pwr_sAttrRef));
    if (EVEN(sts)) return sts;
    if (cdh_ObjidIsNull(PattrXref.Objid))
      break;
  }
  if (i >= Pdim) {
    return PWRS__PALRCONN;
  } else {
    PattrRef = TattrRef;
  }

  TattrRef = SattrRef;
  TattrRef.Size = sizeof(pwr_sAttrRef);
  for (i = 0; i < Sdim; i++, TattrRef.Offset += sizeof(pwr_sAttrRef)) {
    /* find free slot */
    sts = ldh_ReadAttribute(ip->PointedSession, &TattrRef, &SattrXref,
      sizeof(pwr_sAttrRef));
    if (EVEN(sts)) return sts;
    if (cdh_ObjidIsNull(SattrXref.Objid))
      break;
  }
  if (i >= Sdim) {
    return PWRS__SALRCONN;
  } else {
    SattrRef = TattrRef;
  }

  sts = ldh_WriteAttribute(ip->PointedSession, &SattrRef, &PattrRef,
    sizeof(pwr_sAttrRef));
  sts = ldh_WriteAttribute(ip->PointedSession, &PattrRef, &SattrRef,
    sizeof(pwr_sAttrRef));
  return PWRS__SUCCESS;
}

/*----------------------------------------------------------------------------*\
  Is it OK to disconnect two attributes via an AttrXRef
\*----------------------------------------------------------------------------*/

static int IsOkDisconnect (
  ldh_sMenuCall *ip,
  pwr_sMenuButton *mbp
) {
  pwr_tStatus sts;
  pwr_sAttrRef PattrRef;
  pwr_sAttrRef SattrRef;
  pwr_sAttrXRef PattrDef;
  pwr_sAttrXRef SattrDef;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
    mbp->MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return 0;
  sts = ldh_GetAttrRef(ip->PointedSession, ip->Selected[0].Objid,
    mbp->MethodArguments[1], &SattrRef);
  if (EVEN(sts)) return 0;

  sts = ldh_GetAttrXRefDef(ip->PointedSession, &PattrRef, &PattrDef);
  if (EVEN(sts)) return sts;
  sts = ldh_GetAttrXRefDef(ip->PointedSession, &SattrRef, &SattrDef);
  if (EVEN(sts)) return 0;

  if (strncmp(PattrDef.Identity, SattrDef.Identity, sizeof(SattrDef.Identity)) != 0)
    return 0;

  if (strncmp(PattrDef.Source, SattrDef.Target, sizeof(PattrDef.Source)) != 0)
    return 0;

  if (strncmp(PattrDef.Target, SattrDef.Source, sizeof(SattrDef.Source)) != 0)
    return 0;

  return 1;
}

/*----------------------------------------------------------------------------*\
  To disconnect two attributes via an AttrXRef
\*----------------------------------------------------------------------------*/

static pwr_tStatus Disconnect (
  ldh_sMenuCall *ip
) {
  pwr_tStatus sts;
  pwr_sMenuButton mb;
  pwr_sAttrRef PattrRef;
  pwr_sAttrRef PattrXref;
  pwr_sAttrRef SattrRef;
  pwr_sAttrRef SattrXref;
  pwr_sAttrRef TattrRef;
  pwr_sAttrRef NoRef;
  pwr_tUInt32 Pdim;
  pwr_tUInt32 Sdim;
  pwr_sAttrXRef PattrDef;
  pwr_sAttrXRef SattrDef;
  int i;

  sts = ldh_ReadObjectBody(ip->PointedSession,
    ip->ItemList[ip->ChosenItem].MenuObject,
    "SysBody", &mb, sizeof(pwr_sMenuButton));
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrRef(ip->PointedSession, ip->Pointed.Objid,
    mb.MethodArguments[0], &PattrRef);
  if (EVEN(sts)) return sts;
  sts = ldh_GetAttrRef(ip->PointedSession, ip->Selected[0].Objid,
    mb.MethodArguments[1], &SattrRef);
  if (EVEN(sts)) return sts;

  sts = ldh_GetAttrXRefDef(ip->PointedSession, &PattrRef, &PattrDef);
  if (EVEN(sts)) return sts;
  sts = ldh_GetAttrXRefDef(ip->PointedSession, &SattrRef, &SattrDef);
  if (EVEN(sts)) return sts;

  if (strncmp(PattrDef.Identity, SattrDef.Identity, sizeof(SattrDef.Identity)) != 0)
    return PWRS__IDXREF;

  if (strncmp(PattrDef.Source, SattrDef.Target, sizeof(PattrDef.Source)) != 0)
    return PWRS__STXREF;

  if (strncmp(PattrDef.Target, SattrDef.Source, sizeof(SattrDef.Source)) != 0)
    return PWRS__STXREF;

  Pdim = PattrRef.Size / sizeof(pwr_sAttrRef);
  Sdim = SattrRef.Size / sizeof(pwr_sAttrRef);

  TattrRef = PattrRef;
  TattrRef.Size = sizeof(pwr_sAttrRef);
  for (i = 0; i < Pdim; i++, TattrRef.Offset += sizeof(pwr_sAttrRef)) {
    /* find Sobject */
    sts = ldh_ReadAttribute(ip->PointedSession, &TattrRef, &PattrXref,
      sizeof(pwr_sAttrRef));
    if (EVEN(sts)) return sts;
    if (!cdh_ObjidIsEqual(PattrXref.Objid, ip->Selected[0].Objid))
      continue;
    sts = ldh_ReadAttribute(ip->PointedSession, &PattrXref, &SattrXref,
      sizeof(pwr_sAttrRef));
    if (EVEN(sts)) return sts;
    if (memcmp(&SattrXref, &TattrRef, sizeof(pwr_sAttrRef)) == 0)
      break;
  }

  if (i >= Pdim) {
    return PWRS__NOTCONN;
  } else {
    PattrRef = TattrRef;
    SattrRef = PattrXref;
  }

  memset(&NoRef, 0, sizeof(NoRef));
  sts = ldh_WriteAttribute(ip->PointedSession, &SattrRef, &NoRef,
    sizeof(pwr_sAttrRef));
  sts = ldh_WriteAttribute(ip->PointedSession, &PattrRef, &NoRef,
    sizeof(pwr_sAttrRef));
  return sts;
}

pwr_dExport pwr_BindMethods($AttrXRef) = {
  pwr_BindMethod(Connect),
  pwr_BindMethod(IsOkConnect),
  pwr_BindMethod(Disconnect),
  pwr_BindMethod(IsOkDisconnect),
  pwr_NullMethod
};
