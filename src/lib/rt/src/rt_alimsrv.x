#ifdef RPC_HDR
%#ifndef rt_alimsrv_h
%#define rt_alimsrv_h
%
%/* rt_alimserver.h -- 
%
%   PROVIEW/R
%   Copyright (C) 1997 by Comator Process AB.
%
%     */
%
%#if !defined(__rpc_rpc_h) && !defined(__RPC__RPC_H) && !defined(__RPC_HEADER__)
%# include <rpc/rpc.h>
%#endif
%
%#ifndef pwr_baseclasses_h
%# include "pwr_baseclasses.h"
%#endif
%
%#ifndef co_xdr_h
%# include "co_xdr.h"
%#endif
%
%
%#define alimsrv_cQix		124
%#define alimsrv_cMsgType	202
%
%typedef enum {
%  alimsrv_eSubType__		= 0,
%  alimsrv_eSubType_Request	= 1,
%  alimsrv_eSubType_Answer	= 2
%} alimsrv_eSubType;
%
%#define alimsrv_cBufSize      8192
%
#endif


/* Buffer used to request supervisor data from the server */

struct alimsrv_sRequest {
  pwr_tBoolean		Xdr;
  pwr_sAttrRef		Attribute;
  pwr_tBoolean 		AllAttr;
};


/* Primary data for an Asup object, more (Text data ) must be requested using
   gdh_GetObjectInfo */

struct alimsrv_sASupAlim {
  pwr_tUInt32		EvType;
  pwr_tUInt32		EvPrio;
  pwr_tFloat32		Limit;
  pwr_tFloat32		Hyst;
  pwr_tBoolean		High;
  pwr_tBoolean		MsgOn;
};

struct alimsrv_sDSupAlim {
  pwr_tUInt32		EvType;
  pwr_tUInt32		EvPrio;
  pwr_tBoolean		CtrlPos;
};

#ifdef RPC_HDR
%union alimsrv_uSupAlim {
%  alimsrv_sASupAlim	A	pwr_dPacked;
%  alimsrv_sDSupAlim	D	pwr_dPacked;
%};
%
%typedef union alimsrv_uSupAlim alimsrv_uSupAlim;
%
%struct alimsrv_sSupObjEntry {
%  pwr_tObjid	 	SupObjid pwr_dPacked;
%  alimsrv_uSupAlim 	SupData	 pwr_dPacked;
%};
%typedef struct alimsrv_sSupObjEntry  alimsrv_sSupObjEntry;
%
%bool_t
%xdr_alimsrv_sSupObjEntry(
%  XDR 			*xdr,
%  alimsrv_sSupObjEntry *objp,
%  int 			supclass
%);
%
%
%struct alimsrv_sAttrSupList {
%  pwr_sAttrRef		SupAttr		pwr_dPacked;
%  pwr_tInt32	    	NoOfSupObjs	pwr_dPacked;
%  pwr_tUInt32	    	SupClass	pwr_dPacked;
%  alimsrv_sSupObjEntry SupList[1]	pwr_dPacked;
%};
%typedef struct alimsrv_sAttrSupList alimsrv_sAttrSupList;
%
%bool_t
%xdr_alimsrv_sAttrSupList(
%  XDR 			*xdrs,
%  alimsrv_sAttrSupList *objp,
%  int 			nofsupattr
%);
%
% /* The answer from the server */
%
%struct alimsrv_sSupDataBuf {
%  pwr_tBoolean 	Xdr		pwr_dPacked;
%  pwr_tUInt32		NoOfSupAttr	pwr_dPacked;
%  alimsrv_sAttrSupList  AttrSupList[1]	pwr_dPacked;
%};
%typedef struct alimsrv_sSupDataBuf alimsrv_sSupDataBuf;
%
%bool_t
%xdr_alimsrv_sSupDataBuf();
%
#else
%
%bool_t
%xdr_alimsrv_sSupObjEntry(
%	XDR *xdrs,
%	alimsrv_sSupObjEntry *objp,
%	int supclass)
%{
%	if (!xdr_pwr_tObjid(xdrs, &objp->SupObjid)) {
%		return (FALSE);
%	}
%	if (supclass == pwr_cClass_ASup) {
%		if (!xdr_alimsrv_sASupAlim(xdrs, &objp->SupData.A)) {
%			return (FALSE);
%		}
%	}
%	else if (supclass == pwr_cClass_DSup) {
%		if (!xdr_alimsrv_sDSupAlim(xdrs, &objp->SupData.D)) {
%			return (FALSE);
%		}
%		if (!xdr_opaque(xdrs, (void *)(&objp->SupData.D + 1), sizeof(objp->SupData.A) - sizeof(objp->SupData.D))) {
%			return (FALSE);
%		}
%	}
%	else
%		return (FALSE);
% 
%	return (TRUE);
%}
%
%
%bool_t
%xdr_alimsrv_sAttrSupList(
%	XDR *xdrs,
%	alimsrv_sAttrSupList *objp,
%	int nofsupattr
%)
%{
%        int supclass, nofsups, i, j;
%
%    for (i = 0; i < nofsupattr; i++) {
%	if (xdrs->x_op == XDR_ENCODE) {
%	 	supclass = objp->SupClass;
%		nofsups = objp->NoOfSupObjs;
%	}
%	else {
%	    	supclass = ntohl(objp->SupClass);
%	    	nofsups = ntohl(objp->NoOfSupObjs);
%        }
%
%	if (!xdr_pwr_sAttrRef(xdrs, &objp->SupAttr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tInt32(xdrs, &objp->NoOfSupObjs)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->SupClass)) {
%		return (FALSE);
%	}
%        for (j = 0; j < nofsups; j++) {
%		if (!xdr_alimsrv_sSupObjEntry(xdrs, &objp->SupList[j], supclass)) {
%			return (FALSE);
%		} 
%	}
%    }
%	return (TRUE);
%}
%
%bool_t
%xdr_alimsrv_sSupDataBuf(xdrs, objp)
%	XDR *xdrs;
%	alimsrv_sSupDataBuf *objp;
%{
%        int i, nofsupattr;
%
%	if (xdrs->x_op == XDR_ENCODE)
%	  	nofsupattr = objp->NoOfSupAttr; 
%	else
%	  	nofsupattr = ntohl(objp->NoOfSupAttr); 
%
%	if (!xdr_pwr_tBoolean(xdrs, &objp->Xdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->NoOfSupAttr)) {
%		return (FALSE);
%	}
%        for (i = 0; i < nofsupattr; i++) {
%		if (!xdr_alimsrv_sAttrSupList(xdrs, &objp->AttrSupList[i], nofsupattr)) {
%			return (FALSE);
%		} 
%	}
%	return (TRUE);
%}
%
#endif
#ifdef RPC_HDR
%#endif
#endif

 
