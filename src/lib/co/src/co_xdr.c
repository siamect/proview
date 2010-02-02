/** 
 * Proview   $Id: co_xdr.c,v 1.6 2007-04-25 13:39:21 claes Exp $
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

/* co_xdr.c -- xdr routines
   This module contains the XDR-routines */

#include <string.h>

#include "co_xdr.h"

#include <rpc/xdr.h>


/*
* NOTE!  len must divide by 4 (len % 4 == 0)
*/
#define PWR_XDR_BYTES(xdrs, addr, len)				\
{								\
  if (((xdrs)->x_handy -= len) < 0)				\
    return (FALSE);						\
    								\
  if ((xdrs)->x_op == XDR_DECODE)				\
    memmove((caddr_t)addr, (xdrs)->x_private, len);		\
  else					 			\
    memmove((xdrs)->x_private, (caddr_t)addr, len);		\
								\
  (xdrs)->x_private += len;	  				\
  return (TRUE);						\
}

/*
* NOTE!  len must divide by 4 (len % 4 == 0)
*/
#define PWR_XDR_STRING(xdrs, addr, len)				\
{								\
  if (((xdrs)->x_handy -= len) < 0)				\
    return (FALSE);						\
    								\
  if ((xdrs)->x_op == XDR_DECODE)				\
    memmove((caddr_t)addr, (xdrs)->x_private, len);		\
  else					 			\
    memmove((xdrs)->x_private, (caddr_t)addr, len);		\
								\
  (xdrs)->x_private += len;	  				\
  return (TRUE);						\
}

/*
* NOTE!  size of int must be 4
*/
#define PWR_XDR_INT(xdrs, objp, objtype)			\
{								\
  if (((xdrs)->x_handy -= sizeof(int)) < 0)			\
    return (FALSE);						\
    								\
  if ((xdrs)->x_op == XDR_DECODE)				\
    *(objp) = (objtype) ntohl((*((u_int *)((xdrs)->x_private))));\
  else					 			\
    *(u_int *)(xdrs)->x_private =(int)htonl((u_int) (*(objp)));\
								\
 (xdrs)->x_private += sizeof(int);	  			\
  return (TRUE);						\
}


/*
* co_IsXdrNeeded
*/
pwr_tBoolean co_IsXdrNeeded(co_sPlatform *plat1, co_sPlatform *plat2)
{
  if (plat1->bo == plat2->bo && plat1->ft == plat2->ft)
      return FALSE;

  return TRUE;
}


/*
* XDR Routines
*/

bool_t 
xdr_co_sPlatform(XDR *xdrs, co_sPlatform *objp)
{
  if (!xdr_int(xdrs, (int *)&objp->os)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->hw)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->bo)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->ft)) return FALSE;

  return TRUE;
}

bool_t
xdr_cdh_sObjName(XDR *xdrs, cdh_sObjName *objp)
{
	if (!xdr_pwr_tObjName(xdrs, &objp->orig)) {
		return (FALSE);
	}
	if (!xdr_pwr_tObjName(xdrs, &objp->norm)) {
		return (FALSE);
	}
	if (!xdr_pwr_tUInt32(xdrs, &objp->pack.key)) {
		return (FALSE);
	}
	return (TRUE);
}

bool_t
xdr_cdh_sFamily(XDR *xdrs, cdh_sFamily *objp)
{
	if (!xdr_cdh_sObjName(xdrs, &objp->name)) {
		return (FALSE);
	}
	if (!xdr_pwr_tObjid(xdrs, &objp->poid)) {
		return (FALSE);
	}
	return (TRUE);
}

bool_t
xdr_pwr_eType(XDR *xdrs, pwr_eType *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tUInt32);
}


bool_t
xdr_pwr_mAdef(XDR *xdrs, pwr_mAdef *objp)
{
  PWR_XDR_INT(xdrs, &objp->m, pwr_tUInt32);
}

bool_t
xdr_pwr_sClassDef(XDR *xdrs, pwr_sClassDef *objp)
{
  if (!xdr_int(xdrs, (int *)&objp->Editor)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Method)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Flags.m)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->NumOfObjBodies)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->PopEditor)) return FALSE;
  
  return TRUE;
}


bool_t
xdr_pwr_sObjBodyDef(XDR *xdrs, pwr_sObjBodyDef *objp)
{
  if (!xdr_pwr_tStructName(xdrs, &objp->StructName)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->NumOfParams)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Size)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->NextAix)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Flags)) return FALSE;

  return TRUE;
}


bool_t
xdr_pwr_sParInfo(XDR *xdrs, pwr_sParInfo *objp)
{
  if (!xdr_pwr_tPgmName(xdrs, &objp->PgmName)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Type)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Offset)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Size)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Flags)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Elements)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->ParamIndex)) return FALSE;
  
  return TRUE;
}

bool_t
xdr_pwr_sParam(XDR *xdrs, pwr_sParam *objp)
{
  if (!xdr_pwr_sParInfo(xdrs, &objp->Info)) return FALSE;
  PWR_XDR_INT(xdrs, &objp->TypeRef, pwr_tTypeId);
  
}



bool_t
xdr_pwr_tAix(XDR *xdrs, pwr_tAix *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tAix);
}

bool_t
xdr_pwr_tBit(XDR *xdrs, pwr_tBit *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tBit);
}


bool_t
xdr_pwr_tBitMask(XDR *xdrs, pwr_tBitMask *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tBitMask);
}



bool_t
xdr_pwr_tBoolean(XDR *xdrs, pwr_tBoolean *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tBoolean);
}


bool_t
xdr_pwr_tGeneration(XDR *xdrs, pwr_tGeneration *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tGeneration);
}


bool_t
xdr_pwr_tChar(XDR *xdrs, pwr_tChar *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tChar);
}


bool_t
xdr_pwr_tInt8(XDR *xdrs, pwr_tInt8 *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tInt8);
}


bool_t
xdr_pwr_tInt16(XDR *xdrs, pwr_tInt16 *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tInt16);
}


bool_t
xdr_pwr_tInt32(XDR *xdrs, pwr_tInt32 *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tInt32);
}


bool_t
xdr_pwr_tInt64(XDR *xdrs, pwr_tInt64 *objp)
{
  if (!xdr_int(xdrs, (int *)&((__pwr_tInt64 *)objp)->low)) return FALSE;
  if (!xdr_int(xdrs, &((__pwr_tInt64 *)objp)->high)) return FALSE;

  return TRUE;
}


bool_t
xdr_pwr_tUInt8(XDR *xdrs, pwr_tUInt8 *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tUInt8);
}


bool_t
xdr_pwr_tUInt16(XDR *xdrs, pwr_tUInt16 *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tUInt16);
}


bool_t
xdr_pwr_tUInt32(XDR *xdrs, pwr_tUInt32 *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tUInt32);
}


bool_t
xdr_pwr_tUInt64(XDR *xdrs, pwr_tUInt64 *objp)
{
  if (!xdr_int(xdrs, (int *)&((__pwr_tUInt64 *)objp)->low)) return FALSE;
  if (!xdr_int(xdrs, (int *)&((__pwr_tUInt64 *)objp)->high)) return FALSE;

  return TRUE;
}


bool_t
xdr_pwr_tStatus(XDR *xdrs, pwr_tStatus *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tStatus);
}


bool_t
xdr_pwr_tVolumeId(XDR *xdrs, pwr_tVolumeId *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tVolumeId);
}


bool_t
xdr_pwr_tObjectIx(XDR *xdrs, pwr_tObjectIx *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tObjectIx);
}


bool_t
xdr_pwr_tObjid(XDR *xdrs, pwr_tObjid *objp)
{
  if (!xdr_int(xdrs, (int *)&objp->oix)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->vid)) return FALSE;

  return TRUE;
}


bool_t
xdr_pwr_tClassId(XDR *xdrs, pwr_tClassId *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tClassId);
}


bool_t
xdr_pwr_tTypeId(XDR *xdrs, pwr_tTypeId *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tTypeId);
}


bool_t
xdr_pwr_tVersion(XDR *xdrs, pwr_tVersion *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tVersion);
}


bool_t
xdr_pwr_tPwrVersion(XDR *xdrs, pwr_tPwrVersion *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tPwrVersion);
}


bool_t
xdr_pwr_tProjVersion(XDR *xdrs, pwr_tProjVersion *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tProjVersion);
}


bool_t
xdr_pwr_tUserId(XDR *xdrs, pwr_tUserId *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tUserId);
}


bool_t
xdr_pwr_tDbId(XDR *xdrs, pwr_tDbId *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tDbId);
}


bool_t
xdr_pwr_tNodeId(XDR *xdrs, pwr_tNodeId *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tNodeId);
}


bool_t
xdr_pwr_tNodeIndex(XDR *xdrs, pwr_tNodeIndex *objp)
{
  PWR_XDR_INT(xdrs, objp, pwr_tNodeIndex);
}


bool_t
xdr_pwr_tRefId(XDR *xdrs, pwr_tRefId *objp)
{
  if (!xdr_int(xdrs, (int *)&objp->rix)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->nid)) return FALSE;

  return TRUE;
}


bool_t
xdr_pwr_tDlid(XDR *xdrs, pwr_tDlid *objp)
{
  if (!xdr_int(xdrs, (int *)&objp->rix)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->nid)) return FALSE;

  return TRUE;
}


bool_t
xdr_pwr_tSubid(XDR *xdrs, pwr_tSubid *objp)
{
  if (!xdr_int(xdrs, (int *)&objp->rix)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->nid)) return FALSE;

  return TRUE;
}


bool_t
xdr_pwr_tTime(XDR *xdrs, pwr_tTime *objp)
{
  if (!xdr_pwr_tInt64(xdrs, &objp->tv_sec)) return FALSE;
  if (!xdr_pwr_tInt64(xdrs, &objp->tv_nsec)) return FALSE;

  return TRUE;
}


bool_t
xdr_pwr_tDeltaTime(XDR *xdrs, pwr_tDeltaTime *objp)
{
  if (!xdr_pwr_tInt64(xdrs, &objp->tv_sec)) return FALSE;
  if (!xdr_pwr_tInt64(xdrs, &objp->tv_nsec)) return FALSE;

  return TRUE;
}


bool_t
xdr_pwr_tObjName(XDR *xdrs, pwr_tObjName *objp)
{
  PWR_XDR_STRING(xdrs, *objp, sizeof(*objp));
}


bool_t
xdr_pwr_tPgmName(XDR *xdrs, pwr_tPgmName *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tXRef(XDR *xdrs, pwr_tXRef *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tGraphName(XDR *xdrs, pwr_tGraphName *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tStructName(XDR *xdrs, pwr_tStructName *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tAttrName(XDR *xdrs, pwr_tAttrName *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}

bool_t
xdr_pwr_tPathName(XDR *xdrs, pwr_tPathName *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}

bool_t
xdr_pwr_tAName(XDR *xdrs, pwr_tAName *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tFullName(XDR *xdrs, pwr_tFullName *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tString256(XDR *xdrs, pwr_tString256 *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tString132(XDR *xdrs, pwr_tString132 *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tString80(XDR *xdrs, pwr_tString80 *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tString40(XDR *xdrs, pwr_tString40 *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tString32(XDR *xdrs, pwr_tString32 *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tString16(XDR *xdrs, pwr_tString16 *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_tString8(XDR *xdrs, pwr_tString8 *objp)
{
  PWR_XDR_BYTES(xdrs, objp, sizeof(*objp));
}


bool_t
xdr_pwr_mAttrRef(XDR *xdrs, pwr_mAttrRef *objp)
{
  PWR_XDR_INT(xdrs, &objp->m, pwr_tBitMask);
}


bool_t 
xdr_pwr_sAttrRef(XDR *xdrs, pwr_sAttrRef *objp)
{
  if (!xdr_int(xdrs, (int *)&objp->Objid.oix)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Objid.vid)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Body)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Offset)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Size)) return FALSE;
  if (!xdr_int(xdrs, (int *)&objp->Flags.m)) return FALSE;

  return TRUE;
}
