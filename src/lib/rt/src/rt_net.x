#ifdef RPC_HDR
%/* 
% * Proview   $Id: rt_net.x,v 1.13 2008-06-24 07:14:53 claes Exp $
% * Copyright (C) 2005 SSAB Oxel�sund AB.
% *
% * This program is free software; you can redistribute it and/or 
% * modify it under the terms of the GNU General Public License as 
% * published by the Free Software Foundation, either version 2 of 
% * the License, or (at your option) any later version.
% *
% * This program is distributed in the hope that it will be useful 
% * but WITHOUT ANY WARRANTY; without even the implied warranty of 
% * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
% * GNU General Public License for more details.
% *
% * You should have received a copy of the GNU General Public License 
% * along with the program, if not, write to the Free Software 
% * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
% */
%
%#ifndef rt_net_h
%#define rt_net_h
%
%#if defined __cplusplus
%extern "C" {
%#endif
%
%#ifndef pwr_h
%#include "pwr.h"
%#endif
%
%#ifndef rt_qcom_h
%#include "rt_qcom.h"
%#endif
%
%#ifndef co_xdr_h
%# include "co_xdr.h"
%#endif
%
%#ifndef co_cdh_h
%#include "co_cdh.h"
%#endif
%
%/** Protocol version. Increase whenever an
%    incompatible change is done to the protocol.
%    Update supported features in function id() in file rt_neth.c  */
%
%#define net_cVersion		8
%
% /** @note DO NOT change this number */
%#define net_cFirstCclassVersion 8 /* first version with cached classes */
%
%#define net_cSendRcvTmo	20000	/* Timeout (ms) used in pams_rcv_msgw calls */
%#define net_cProcHandler	110
%#define net_cMsgClass		200
% 
%#define net_cSizeSmall		600
%#define net_cSizeMedium	5000
%#define net_cSizeLarge		9900
%
% 
#endif

#ifndef RPC_HDR
%#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
%#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

enum net_eMsg {
  net_eMsg__ = -1,
  net_eMsg_error = 0,		/* A network error was detected */

  net_eMsg_id,			/* Nethandler identification */
  net_eMsg_idAck,		/* Nethandler identification acknowlege */
  net_eMsg_idAck2,		/* Nethandler idAck acknowlege */

  net_eMsg_volumes,		/*  */
  net_eMsg_volumesR,		/*  */

  net_eMsg_subAdd,		/* Add subscription */
  net_eMsg_subRemove,		/* Remove subscription */
  net_eMsg_subData,		/* Subscription data transfer */

  net_eMsg_sanAdd,		/* Add subscription */
  net_eMsg_sanRemove,		/* Remove subscription */
  net_eMsg_sanUpdate,		/* Subscription data transfer */

  net_eMsg_nameToObject,	/* Cache inquiry, fetch name */
  net_eMsg_oidToObject,		/* Cache inquiry, fetch objid */
  net_eMsg_objectR,		/* response */

  net_eMsg_getObjectInfo,
  net_eMsg_getObjectInfoR,
  net_eMsg_setObjectInfo,
  net_eMsg_setObjectInfoR,

  net_eMsg_flush,		/* Cache flush request */

  net_eMsg_createObject,
  net_eMsg_deleteObject,
  net_eMsg_moveObject,
  net_eMsg_renameObject,

  net_eMsg_nodeUp,		/* A node is up */
  net_eMsg_nodeDown,		/* A node is up */

  net_eMsg_getCclass,		/* Cached class info*/
  net_eMsg_getCclassR,

  net_eMsg_getGclass,		/* The whole class */
  net_eMsg_getGclassR,

  net_eMsg_serverConnect,       /* New server */

  net_eMsg_fileList,
  net_eMsg_fileListR,

  net_eMsg_getCircBuffer,
  net_eMsg_getCircBufferR,
  net_eMsg_updateCircBuffer,
  net_eMsg_updateCircBufferR,

  net_eMsg_classList,
  net_eMsg_classListR,

  net_eMsg_,			/* Not a valid message */
  
  net_eMsg_volumes7,            /* Version 7. Internal only */

  net_eMsg_end
};


/* Link events, used by the link state machine.  */

enum net_eEvent {
  net_eEvent__ = -1,
  net_eEvent_nodeUp = 0,
  net_eEvent_nodeDown,
  net_eEvent_id,
  net_eEvent_idAck,
  net_eEvent_idAck2,
  net_eEvent_error,
  net_eEvent_other,
  net_eEvent_
};

#ifdef RPC_HDR
%typedef union {
%  pwr_tBitMask	m;
%  pwr_32Bits (
%    pwr_Bits( linkStates	, 1),
%    pwr_Bits( id		, 1),
%    pwr_Bits( messages		, 1),
%    pwr_Bits( tmon		, 1),
%    pwr_Bits( cacheTrim	, 1),
%    pwr_Bits( sub		, 1),
%    pwr_Bits( san		, 1),
%    pwr_Bits( fill0		, 1),
%
%    pwr_Bits( fill1		, 8),,,,,,,,
%    pwr_Bits( fill2		, 8),,,,,,,,
%    pwr_Bits( fill3		, 8),,,,,,,
%  ) b;
%
%#define net_mLog__		0
%#define net_mLog_linkStates	pwr_Bit(0)
%#define net_mLog_id		pwr_Bit(1)
%#define net_mLog_messages	pwr_Bit(2)
%#define net_mLog_tmon		pwr_Bit(3)
%#define net_mLog_cacheTrim	pwr_Bit(4)
%#define net_mLog_sub		pwr_Bit(5)
%#define net_mLog_san		pwr_Bit(6)
%#define net_mLog_		(~net_mLog__)
%
%#define net_mLog_all		net_mLog_
%} net_mLog;
%
#endif
#ifdef RPC_XDR
%
%bool_t
%xdr_net_mLog(xdrs, objp)
%	XDR *xdrs;
%	net_mLog *objp;
%{
%	if (!xdr_pwr_tBitMask(xdrs, (pwr_tBitMask *) &objp->m)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

/**< Generic message header.
 * All class NETH messages MUST start with this header.  
 * This header needs a version(s) memeber(s), but we can't add
 * due to backward compability.
 * If you can skip this requirement, please add the version  
 */

struct net_sMessage {
  pwr_tBoolean		xdr;
  pwr_tUInt32		msn;		/* Message sequence number, should be zero if
					   not a request/reply. A reply always get the
					   msn of a request.  */
  pwr_tNodeId		nid;		/* Sender's node identity */
};

/* Net error detected.  */

struct net_sError {
  net_sMessage		hdr;		/* Header */
  qcom_sQid		remqid;		/* Error port */
  pwr_tStatus		sts;		/* Status code */
};

/* Nethandler identification.  */

struct net_sNode {
  qcom_sQid		handler;	/* Net address of net handler */
  pwr_tUInt32		netver;		/* Net protocol version	*/
  pwr_tObjid		nod_oid;	/* Sender's node object */
  pwr_tObjid		vol_oid;	/* Sender's root volume object */
};

struct net_sId {
  net_sMessage		hdr;		/* Header */
  net_sNode		node;
};

struct net_sIdAck2 {
  net_sMessage		hdr;		/* Header */
};

struct net_sNodeUp {
  net_sMessage		hdr;		/* Header */
  pwr_tNodeId		nid;		/* nodindex */
};

/* Link states.  */

enum net_eState {
  net_eState__ = -1,
  net_eState_down,		/* Down. No connection */
  net_eState_istrt,		/* Start state I */
  net_eState_astrt,		/* Start state A */
  net_eState_up,		/* Running. Now we're talking... */
  net_eState_
};

/* ClassDef backward compatible */
struct net_sClassDef {
  pwr_tInt32	Editor;
  pwr_tInt32	Method;
  pwr_tUInt32	Flags;
  pwr_tUInt32	NumOfObjBodies;
  pwr_tInt32 PopEditor;
};

/* ObjBodyDef backward compatible */
struct net_sObjBodyDef {
  pwr_tStructName	StructName;
  pwr_tUInt32		NumOfParams;
  pwr_tUInt32		Size;
  pwr_tUInt32		NextAix;
  pwr_tUInt32		Flags;
};

struct net_sParInfo {
  pwr_tPgmName		PgmName;
  pwr_eType		Type;
  pwr_tUInt32		Offset;
  pwr_tUInt32		Size;
  pwr_tUInt32		Flags;
  pwr_tUInt32		Elements;
  pwr_tUInt32		ParamIndex;
};

struct net_sParam {
  net_sParInfo	Info;
  pwr_tTypeId  	TypeRef;
};

/* Add subscription.  */

struct net_sSubSpec {
  pwr_tSubid		sid;		/* associated SUBCLI */
  pwr_tUInt32		dt;		/* Requested refresh time */
  pwr_tBoolean		sub_by_name;	/* Subscription by name indicator */
  pwr_sAttrRef		aref;		/* Subscription by attrref or name */
};

#ifdef RPC_HDR
%#define net_cSubMaxAdd (net_cSizeLarge/sizeof(net_sSubSpec))
%
%typedef struct {
%  net_sMessage		hdr 	pwr_dPacked;	/* Header */
%  pwr_tUInt32		count 	pwr_dPacked;	/* # of entries */
%  net_sSubSpec		spec[1];		/* Specifications (dynamic) */
%} net_sSubAdd;
%
%bool_t xdr_net_sSubAdd(XDR *xdrs, net_sSubAdd *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sSubAdd(XDR *xdrs, net_sSubAdd *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->spec, count, sizeof(net_sSubSpec), (xdrproc_t)xdr_net_sSubSpec)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

#ifdef RPC_HDR
%/* Remove subscription.  */
%
%#define net_cSubMaxRemove (net_cSizeLarge/sizeof(pwr_tSubid))
%
%typedef struct {
%  net_sMessage		hdr    pwr_dPacked;	/* Header */
%  pwr_tUInt32		count  pwr_dPacked;	/* # of removals */
%  pwr_tSubid		sid[1] pwr_dPacked;	/* Subscription to remove */
%} net_sSubRemove;
%
%bool_t xdr_net_sSubRemove(XDR *xdrs, net_sSubRemove *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sSubRemove(XDR *xdrs, net_sSubRemove *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);

%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->sid, count, sizeof(pwr_tSubid), (xdrproc_t)xdr_pwr_tSubid)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

#ifdef RPC_HDR
%/* Subscription data transfer.  */
%
%typedef struct {
%  pwr_tSubid		sid 	pwr_dPacked;	/* Tag */
%  pwr_tStatus		sts 	pwr_dPacked;	/* Status */
%  pwr_tUInt32		size 	pwr_dPacked;	/* Size of data */
%  char			data[1];		/* Dynamic */
%} net_sSubData;
%bool_t xdr_net_sSubData(XDR *xdrs, net_sSubData *objp, int *offset);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sSubData(XDR *xdrs, net_sSubData *objp, int *offset)
%{
%	int size;
%
%	objp = (net_sSubData *)(((char *)objp) + *offset);
%
%	if (xdrs->x_op == XDR_DECODE) {
%		size = (int) ntohl(objp->size);
%	} else {
%		size = objp->size;
%	}
%
%	*offset += sizeof(net_sSubData) + size - 1;
%
%	if (!xdr_pwr_tSubid(xdrs, &objp->sid)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tStatus(xdrs, &objp->sts)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->size)) {
%		return (FALSE);
%	}
%	if (!xdr_opaque(xdrs, (char *)objp->data, size)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

#ifdef RPC_HDR
%#define net_cSubMaxMessage 100
%
%typedef struct {
%  net_sMessage		hdr 	pwr_dPacked;		/* Header when used as message */
%  pwr_tUInt32		count 	pwr_dPacked;		/* # of subdata entries in msg */
%  net_sSubData		subdata[1];			/* Dynamic */
%} net_sSubMessage;
%
%bool_t xdr_net_sSubMessage(XDR *xdrs, net_sSubMessage *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sSubMessage(XDR *xdrs, net_sSubMessage *objp)
%{
%	int count;
%	int offset;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	for (offset = 0; count > 0; count--) {
%		if (!xdr_net_sSubData(xdrs, (net_sSubData *)objp->subdata, &offset)) {
%			return (FALSE);
%		}
%	}
%	return (TRUE);
%}
#endif

/* Subscribed alarm notification.  */

struct net_sSanEntry {
  pwr_tObjid		oid;		/* Specifications (dynamic) */
  pwr_tSubid		sid;
};
  
#ifdef RPC_HDR
%#define	net_cSanMaxAdd	(net_cSizeLarge/sizeof(net_sSanEntry))
%
%typedef struct {
%  net_sMessage		hdr 	pwr_dPacked;		/* Header */
%  pwr_tVolumeId	vid 	pwr_dPacked;
%  pwr_tUInt32		count 	pwr_dPacked;		/* # of entries */
%  net_sSanEntry	sane[1] pwr_dPacked;
%} net_sSanAdd;
%
%bool_t xdr_net_sSanAdd(XDR *xdrs, net_sSanAdd *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sSanAdd(XDR *xdrs, net_sSanAdd *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tVolumeId(xdrs, &objp->vid)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->sane, count, sizeof(net_sSanEntry), (xdrproc_t)xdr_net_sSanEntry)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

/* Remove subscription.  */

#ifdef RPC_HDR
%#define	net_cSanMaxRemove (net_cSizeLarge/sizeof(pwr_tSubid))
%
%typedef struct net_sSanRemove {
%  net_sMessage		hdr 	pwr_dPacked;		/* Header */
%  pwr_tVolumeId	vid 	pwr_dPacked;
%  pwr_tUInt32		count 	pwr_dPacked;		/* # of removals */
%  pwr_tSubid		sid[1] 	pwr_dPacked;		/* Subscriptions to remove (dynamic) */
%} net_sSanRemove;
%
%bool_t xdr_net_sSanRemove(XDR *xdrs, net_sSanRemove *objp);
%
#elif defined RPC_XDR
%bool_t xdr_net_sSanRemove(XDR *xdrs, net_sSanRemove *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tVolumeId(xdrs, &objp->vid)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->sid, count, sizeof(pwr_tSubid), (xdrproc_t)xdr_pwr_tSubid)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

/* Subscription data transfer.  */

struct net_sAlarm {
  pwr_tUInt32		a;		/* This object's alarm level */
  pwr_tUInt32		b;		/* Alarm block level */
  pwr_tUInt32		maxa;		/* maximized local alarm level */
  pwr_tUInt32		maxb;		/* maximized local block level */
  pwr_tUInt32		idx;		/* Alarm block level index.  */
};
  
struct net_sSanData {
  net_sSanEntry		sane;
  net_sAlarm		al;
};

#ifdef RPC_HDR
%#define	net_cSanMaxUpdate (net_cSizeLarge/sizeof(net_sSanData))
%
%typedef struct {
%  net_sMessage		hdr 	pwr_dPacked;	/* Header when used as message */
%  pwr_tUInt32		count 	pwr_dPacked;	/* # of SUBDATA entries in msg */
%  net_sSanData		data[1]	pwr_dPacked;	/* Dynamic */
%} net_sSanUpdate;
%
%bool_t xdr_net_sSanUpdate(XDR *xdrs, net_sSanUpdate *objp);
#elif defined RPC_XDR
%bool_t xdr_net_sSanUpdate(XDR *xdrs, net_sSanUpdate *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->data, count, sizeof(net_sSanData), (xdrproc_t)xdr_net_sSanData)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

#ifdef RPC_HDR
%/*  Cache inquiry, fetch by name.  */
%
%typedef struct {
%  net_sMessage		hdr	pwr_dPacked;	/* Header */
%  pwr_tObjid		poid	pwr_dPacked;	/* Parent of name */
%  pwr_tBitMask		trans	pwr_dPacked;	/* Translation mask.  */
%  pwr_tUInt32		lcount	pwr_dPacked;	/* Number of left siblings wanted. */
%  pwr_tUInt32		rcount	pwr_dPacked;	/* Number of lright siblings wanted. */
%  pwr_tUInt32		len	pwr_dPacked;	/* name buffer length */
%  char			name[1];		/* name buffer */
%} net_sNameToObject;
%
%bool_t xdr_net_sNameToObject(XDR *xdrs, net_sNameToObject *objp);
#elif defined RPC_XDR
%bool_t xdr_net_sNameToObject(XDR *xdrs, net_sNameToObject *objp)
%{
%	int len;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		len = (int) ntohl(objp->len);
%	} else {
%		len = objp->len;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tObjid(xdrs, &objp->poid)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tBitMask(xdrs, &objp->trans)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->lcount)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->rcount)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->len)) {
%		return (FALSE);
%	}
%	if (!xdr_opaque(xdrs, (char *)objp->name, len)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

/*  Cache inquiry, fetch by objid.  */

struct net_sOidToObject {
  net_sMessage		hdr;		/* Header */
  pwr_tObjid		oid;		/* Object we are asking for */
  pwr_tBitMask		trans;		/* Translation mask.  */
  pwr_tUInt32		lcount;		/* Number of left siblings wanted. */
  pwr_tUInt32		rcount;		/* Number of lright siblings wanted. */
};

/*  Cache inquiry response.  */
 
#ifdef RPC_HDR
%typedef union {
%  pwr_tBitMask	m;
%  pwr_32Bits (
%    pwr_Bits( isParent		, 1),
%    pwr_Bits( isAliasClient	, 1),
%    pwr_Bits( isAliasServer	, 1),
%    pwr_Bits( isMountClient	, 1),
%    pwr_Bits( inSibList	, 1),
%    pwr_Bits( hasSubClass	, 1),
%    pwr_Bits( fill0		, 2),,
%
%    pwr_Bits( fill1		, 8),,,,,,,,
%    pwr_Bits( fill2		, 8),,,,,,,,
%    pwr_Bits( fill3		, 8),,,,,,,
%  ) b;
%
%#define net_mGo__		0
%#define net_mGo_isParent	pwr_Bit(0)
%#define net_mGo_isAliasClient	pwr_Bit(1)
%#define net_mGo_isAliasServer	pwr_Bit(2)
%#define net_mGo_isMountClient	pwr_Bit(3)
%#define net_mGo_inSibList	pwr_Bit(4)
%#define net_mGo_hasSubClass	pwr_Bit(5)
%
%#define net_mGo_isClient	(net_mGo_isAliasClient|net_mGo_isMountClient)
%
%#define net_mGo_		(~net_mGo__)
%} net_mGo;
#endif
#ifdef RPC_XDR
%
%bool_t
%xdr_net_mGo(xdrs, objp)
%	XDR *xdrs;
%	net_mGo *objp;
%{
%	if (!xdr_pwr_tBitMask(xdrs, (pwr_tBitMask *) &objp->m)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

struct net_sOixLink {
  pwr_tObjectIx		flink;	/* Forward link */
  pwr_tObjectIx		blink;	/* Backward link */
};

struct net_sGobject {
  pwr_tObjid		oid;		/* Object id */
  pwr_tClassId		cid;		/* ClassId of the class definition */
  cdh_sFamily		f;
  net_sOixLink		sib;		/* Next/prv object with same dad */
  pwr_tObjid		soid;		/* First child/Alias server/Mount server?pwr_NObjid.  */
  pwr_tUInt32		size;		/* Size of object body */
  net_mGo		flags;		/* Global object flags */
};

#ifdef RPC_HDR
%#define net_cObjectMaxCount (net_cSizeLarge/sizeof(net_sGobject))	
%
%typedef struct {
%  net_sMessage		hdr	pwr_dPacked;	/* Header */
%  pwr_tStatus		sts	pwr_dPacked;	/* Status code */
%  pwr_tObjid		oid	pwr_dPacked;	/* Objid of referenced object */
%  pwr_tUInt32		count	pwr_dPacked;	/* # of headers in response */
%  net_sGobject		g[1]	pwr_dPacked;	/* Array of object headers */
%} net_sObjectR;
%
%bool_t xdr_net_sObjectR(XDR *xdrs, net_sObjectR *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sObjectR(XDR *xdrs, net_sObjectR *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tStatus(xdrs, &objp->sts)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tObjid(xdrs, &objp->oid)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->g, count, sizeof(net_sGobject), (xdrproc_t)xdr_net_sGobject)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

/* Get object information.  */

struct net_sGetObjectInfo {
  net_sMessage		hdr;		/* Header */
  pwr_sAttrRef		aref;		/* Whished info */
};

#ifdef RPC_HDR
%typedef struct {
%  net_sMessage		hdr	pwr_dPacked;	/* Header */
%  pwr_tUInt32		sts	pwr_dPacked;	/* Status code */
%  pwr_sAttrRef		aref	pwr_dPacked;	/* Copy of requested info */
%  pwr_tUInt32		size	pwr_dPacked;	/* Size of requested data, same as aref.size, in bytes.  */
%  char			info[1];		/* Data requested.  */
%} net_sGetObjectInfoR;
%
%bool_t xdr_net_sGetObjectInfoR(XDR *xdrs, net_sGetObjectInfoR *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sGetObjectInfoR(XDR *xdrs, net_sGetObjectInfoR *objp)
%{
%	int size;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		size = (int) ntohl(objp->size);
%	} else {
%		size = objp->size;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->sts)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_sAttrRef(xdrs, &objp->aref)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->size)) {
%		return (FALSE);
%	}
%	if (!xdr_opaque(xdrs, (char *)objp->info, size)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

#ifdef RPC_HDR
%/* Set object information.  */
%
%typedef struct {
%  net_sMessage		hdr	pwr_dPacked;	/* Header */
%  pwr_sAttrRef		aref	pwr_dPacked;	/* Data reference */
%  pwr_tUInt32		size	pwr_dPacked;	/* Size of requested data, same as aref.size, in bytes.  */
%  char			info[1];		/* Data to write (size bytes) */
%} net_sSetObjectInfo;
%
%bool_t xdr_net_sSetObjectInfo(XDR *xdrs, net_sSetObjectInfo *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sSetObjectInfo(XDR *xdrs, net_sSetObjectInfo *objp)
%{
%	int size;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		size = (int) ntohl(objp->size);
%	} else {
%		size = objp->size;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_sAttrRef(xdrs, &objp->aref)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->size)) {
%		return (FALSE);
%	}
%	if (!xdr_opaque(xdrs, (char *)objp->info, size)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

struct net_sSetObjectInfoR {
  net_sMessage		hdr;		/* Header */
  pwr_tUInt32		sts;		/* Status code */
  pwr_sAttrRef		aref;		/* Referenced data */
};

/* Object notification.  */

struct net_sSiblings {
  pwr_tObjectIx		flink;
  pwr_tObjectIx		newblink;
  pwr_tObjectIx		blink;
  pwr_tObjectIx		newflink;
};

struct net_sParent {
  pwr_tObjid		oid;
  pwr_tObjid		soid;
};

struct net_sNotify {
  net_sMessage		hdr;
  pwr_tObjid		oid;

};

struct net_sCreateObject {
  net_sNotify		notify;
  net_sParent		par;
  net_sSiblings		sib;
};

struct net_sDeleteObject {
  net_sNotify		notify;
  net_sParent		par;
  net_sSiblings		sib;
};

struct net_sMoveObject {
  net_sNotify		notify;
  net_sOixLink		sib;
  net_sParent		opar;
  net_sSiblings		osib;
  net_sParent		npar;
  net_sSiblings		nsib;
};

struct net_sRenameObject {
  net_sNotify		notify;
  cdh_sFamily		f;		/* Used for Rename */
};

/* For compatibility with < 4.7 versions. Should be replaced by a pwr_tTime. */
struct net_sTime {
  unsigned int 		tv_sec;
  unsigned int 		tv_nsec;
};

/* net_sGvolume for Neth Ver > 7 */
struct net_sGvolume {
  pwr_tVolumeId		vid;
  pwr_tObjid		oid;		/* Oid of volume object.  */
  cdh_sObjName		name;
  pwr_tClassId		cid;		/* Class of volume. */
  pwr_tNodeId		nid;		/* Node id of owner of volume,
					   pwr_cNNodeId if shared or class volume.  */
  net_sTime		time;           /* Currently only used for Class volumes. 
                                         * Added in Neth version 8 */
};


/* net_sGvolume for Neth Ver == 7 */
struct net_sGvolume7 {
  pwr_tVolumeId		vid;
  pwr_tObjid		oid;		/* Oid of volume object.  */
  cdh_sObjName		name;
  pwr_tClassId		cid;		/* Class of volume. */
  pwr_tNodeId		nid;		/* Node id of owner of volume,
					   pwr_cNNodeId if shared or class volume.  */
};



#ifdef RPC_HDR
%
%#define net_cVolumeMaxCount	(net_cSizeLarge/sizeof(net_sGvolume))
%
%typedef struct {
%  net_sMessage		hdr	pwr_dPacked;	/* Header */
%  pwr_tUInt32		ctx	pwr_dPacked;
%  pwr_tUInt32		count	pwr_dPacked;	/* # of headers  */
%  net_sGvolume		g[1]	pwr_dPacked;	/* Array of volume headers */
%} net_sVolumes;
%
%bool_t xdr_net_sVolumes(XDR *xdrs, net_sVolumes *objp);
%
%typedef struct {
%  net_sMessage		hdr	pwr_dPacked;	/* Header */
%  pwr_tUInt32		ctx	pwr_dPacked;
%  pwr_tUInt32		count	pwr_dPacked;	/* # of headers  */
%  net_sGvolume7	g[1]	pwr_dPacked;	/* Array of volume headers */
%} net_sVolumes7;
%
%bool_t xdr_net_sVolumes7(XDR *xdrs, net_sVolumes7 *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sVolumes(XDR *xdrs, net_sVolumes *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->ctx)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->g, count, sizeof(net_sGvolume), (xdrproc_t)xdr_net_sGvolume)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
%
%
%bool_t xdr_net_sVolumes7(XDR *xdrs, net_sVolumes7 *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->ctx)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->g, count, sizeof(net_sGvolume7), (xdrproc_t)xdr_net_sGvolume7)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

struct net_sMount {
  pwr_tVolumeId		vid;
};

#ifdef RPC_HDR
%typedef struct {
%  net_sMessage		hdr	pwr_dPacked;	/* Header */
%  pwr_tUInt32		ctx	pwr_dPacked;
%  pwr_tUInt32		count	pwr_dPacked;	/* # of headers  */
%  net_sMount		mount[1] pwr_dPacked;/* Array of mount requests */
%} net_sVolumesR;
%
%bool_t xdr_net_sVolumesR(XDR *xdrs, net_sVolumesR *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_net_sVolumesR(XDR *xdrs, net_sVolumesR *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->count);
%	} else {
%		count = objp->count;
%	}
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->ctx)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->mount, count, sizeof(net_sMount), (xdrproc_t)xdr_net_sMount)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif


struct net_sGattribute {
  net_sGobject		ao;
  net_sParam		ab;
};


struct net_sGclass {
  net_sTime		time; 	/**< Time of last change */
  pwr_tBitMask		dbsFlags;
  net_sGobject		co;
  net_sClassDef		cb;
  net_sGobject		bo;
  net_sObjBodyDef	bb;
  pwr_tUInt32		size;	/**< Size of objects body  */
  pwr_tUInt32		acount;	/**< # of attributes  */
};







struct net_sGetGclass {
  net_sMessage		hdr;
  pwr_tUInt32		ver; /**< see net_sMessage for comment */
  pwr_tClassId	       	cid;
  pwr_tUInt32 		aidx; /**< Starting attribute index */ 
};




#ifdef RPC_HDR
%
%
%typedef struct {
%  net_sMessage 	hdr;/**< Header */
%  pwr_tUInt32		ver;/**< see net_sMessage for comment */
%  pwr_tStatus          sts;/**< Status */
%
%  /* The data below is only valid if ODD(sts) */
%
%  pwr_tObjName		vname;  /**< Volume name, needed if volume must be created */
%  pwr_tUInt32		acntmsg; /**< Number of attributes in this message. */  
%  pwr_tUInt32 		saidx; /**< Starting attribute index */ 
%  pwr_tUInt32 		naidx; /**< Index for next attribute which didn't fit 
%                                    in this message or ULONG_MAX if all included */
%  net_sGclass          gclass;
%  net_sGattribute	attr[1]; /**< Array of attributes */
%} net_sGetGclassR;
%
%bool_t xdr_net_sGetGclassR(XDR *xdrs, net_sGetGclassR *objp);
%
#elif defined RPC_XDR
%
%bool_t
%xdr_net_sGetGclassR(XDR *xdrs, net_sGetGclassR *objp)
%{
%	int 		acntmsg;
%	pwr_tStatus 	sts;
% 
%	if (xdrs->x_op == XDR_DECODE) {
%		acntmsg = (int) ntohl(objp->acntmsg);
%		sts = (int) ntohl(objp->sts);
%	} else {
%		acntmsg = objp->acntmsg;
%		sts = objp->sts;
%	}
%
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->ver)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tStatus(xdrs, &objp->sts)) {
%		return (FALSE);
%	}
%
%	if (EVEN(sts)) {
%		acntmsg = 0;
%		if (!xdr_pwr_tUInt32(xdrs, (unsigned int *)&acntmsg)) {
%				return (FALSE);
%		}
%		return (TRUE); 
%       }  
%
%
%	if (!xdr_pwr_tObjName(xdrs, &objp->vname )) {
%		return (FALSE);
%	} 
%	if (!xdr_pwr_tUInt32(xdrs, &objp->acntmsg  )) {
%		return (FALSE);
%	} 
%	if (!xdr_pwr_tUInt32(xdrs, &objp->saidx)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->naidx)) {
%		return (FALSE);
%	}
%	if (!xdr_net_sGclass(xdrs, &objp->gclass)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->attr, acntmsg, sizeof(net_sGattribute), (xdrproc_t)xdr_net_sGattribute)) {
%		return (FALSE);
%	}
%
%	return (TRUE);
%
%}
#endif




  



struct net_sCattribute {
  pwr_tAix              aix;
  pwr_mAdef		flags; /* Do we need this? */
  pwr_eType		type;
  pwr_tUInt32		offs;
  pwr_tUInt32		size;
  pwr_tUInt32		elem;
  pwr_tUInt32		moffset;
};


struct  net_sCclass {
  pwr_tClassId		cid; 	/**< Class Id */
  net_sTime		time; 	/**< Time of last change */
  pwr_tUInt32		size;	/**< Size of objects body  */
  pwr_tUInt32		acount;	/**< # of attributes  */
};



struct net_sGetCclass {
  net_sMessage		hdr;
  pwr_tUInt32		ver; /**< see net_sMessage for comment */
  pwr_tClassId	       	cid;
  net_sTime		time;
  pwr_tUInt32 		aidx; /**< Starting attribute index */ 
};



#ifdef RPC_HDR
%
%typedef struct {
%  net_sMessage 	hdr;   /**< Header */
%  pwr_tUInt32		ver; /**< see net_sMessage for comment */
%  pwr_tStatus          sts;   /**< Status */
%  pwr_tBoolean         equal; /**< The remote class is equal to the native */
%
%  /* 
%   * The data below is only valid if ODD(sts) and !equal 
%   */
%
%  pwr_tUInt32		acntmsg; /**< Number of attributes in this message. */  
%  pwr_tUInt32 		saidx;   /**< Starting attribute index */ 
%  pwr_tUInt32 		naidx;   /**< Index for next attribute which didn't fit 
%                                    in this message or ULONG_MAX if all included */ 
%  net_sCclass          cclass;
%  net_sCattribute	attr[1]; /**< Array of attributes */
%} net_sGetCclassR;
%
%
%bool_t xdr_net_sGetCclassR(XDR *xdrs, net_sGetCclassR *objp);
%
#elif defined RPC_XDR
%
%bool_t
%xdr_net_sGetCclassR(XDR *xdrs, net_sGetCclassR *objp)
%{
%	int 		acntmsg;
%	pwr_tStatus 	sts;
%	pwr_tBoolean    equal; 
% 
%	if (xdrs->x_op == XDR_DECODE) {
%		acntmsg = (int) ntohl(objp->acntmsg);
%		sts = (pwr_tStatus) ntohl(objp->sts);
%		equal = (pwr_tBoolean) ntohl(objp->equal); 
%	} else {
%		acntmsg = objp->acntmsg;
%		sts = objp->sts;
%		equal = objp->equal; 
%	}
%
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->ver)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tStatus(xdrs, &objp->sts)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tBoolean(xdrs, &objp->equal)) {
%		return (FALSE);
%	}
%
%
%	if (EVEN(sts) || equal) {
%		acntmsg = 0;
%		if (!xdr_pwr_tUInt32(xdrs, (unsigned int *)&acntmsg)) {
%				return (FALSE);
%		}
%		return (TRUE); 
%       }  
%
%
%	if (!xdr_pwr_tUInt32(xdrs, &objp->acntmsg)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->saidx)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->naidx)) {
%		return (FALSE);
%	}
%	if (!xdr_net_sCclass(xdrs, &objp->cclass)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->attr, acntmsg, sizeof(net_sCattribute), (xdrproc_t)xdr_net_sCattribute)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif


struct net_sFileList {
  net_sMessage		hdr;
  char			dir[256];  
  char			pattern[40];
};


#ifdef RPC_HDR
%
%
%typedef struct {
%  net_sMessage 	hdr;		/**< Header */
%  pwr_tStatus          sts;		/**< Status */
%  pwr_tUInt32		filecnt; 	/**< Number of files found. */  
%  char			files[1]; 	/**< List of files */
%} net_sFileListR;
%
%bool_t xdr_net_sFileListR(XDR *xdrs, net_sFileListR *objp);
%
#elif defined RPC_XDR
%
%bool_t
%xdr_net_sFileListR(XDR *xdrs, net_sFileListR *objp)
%{
%	int 		filecnt;
%	pwr_tStatus 	sts;
% 
%	if (xdrs->x_op == XDR_DECODE) {
%		filecnt = (int) ntohl(objp->filecnt);
%		sts = (int) ntohl(objp->sts);
%	} else {
%		filecnt = objp->filecnt;
%		sts = objp->sts;
%	}
%
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tStatus(xdrs, &objp->sts)) {
%		return (FALSE);
%	}
%
%	if (EVEN(sts)) {
%		filecnt = 0;
%		if (!xdr_pwr_tUInt32(xdrs, (unsigned int *)&filecnt)) {
%				return (FALSE);
%		}
%		return (TRUE); 
%       }  
%
%
%	if (!xdr_pwr_tUInt32(xdrs, &objp->filecnt  )) {
%		return (FALSE);
%	} 
%	if (!xdr_opaque(xdrs, (char *)objp->files, filecnt * sizeof(pwr_tString40))) {
%		return (FALSE);
%	}
%	return (TRUE);
%
%}
#endif

struct  net_sGetCircBuffer {
  net_sMessage		hdr;
  pwr_sAttrRef		circ_aref;
  pwr_tUInt32		resolution;
  pwr_tUInt32		samples;
  pwr_tUInt32		bufsize;
};

#ifdef RPC_HDR
%
%
%typedef struct {
%  net_sMessage 	hdr;		/**< Header */
%  pwr_tStatus          sts;		/**< Status */
%  pwr_sAttrRef		circ_aref; 	/**< CircBuffer attribute reference. */
%  pwr_tUInt32		first_idx; 	/**< First index in buffer. */
%  pwr_tUInt32		last_idx; 	/**< Last index in buffer. */
%  pwr_tUInt32		offset; 	/**< Offset for next update. */
%  pwr_tUInt32		size; 		/**< Returned number of samples. */
%  pwr_tUInt32		bufsize; 	/**< Size data in bytes. */
%  char			buf[1]; 	/**< Data */
%} net_sGetCircBufferR;
%
%bool_t xdr_net_sGetCircBufferR(XDR *xdrs, net_sGetCircBufferR *objp);
%
#elif defined RPC_XDR
%
%bool_t
%xdr_net_sGetCircBufferR(XDR *xdrs, net_sGetCircBufferR *objp)
%{
%	pwr_tStatus 	sts;
%	pwr_tUInt32 	bufsize;
% 
%	if (xdrs->x_op == XDR_DECODE) {
%		sts = (int) ntohl(objp->sts);
%		bufsize = (int) ntohl(objp->bufsize);
%	} else {
%		sts = objp->sts;
%		bufsize = objp->bufsize;
%	}
%
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tStatus(xdrs, &objp->sts)) {
%		return (FALSE);
%	}
%
%	if (EVEN(sts)) {
%		if (!xdr_pwr_sAttrRef(xdrs, &objp->circ_aref)) {
%			return (FALSE);
%		}	
%		if (!xdr_pwr_tUInt32(xdrs, &objp->size  )) {
%			return (FALSE);
%		}
%		return (TRUE); 
%       }  
%
%
%	if (!xdr_pwr_sAttrRef(xdrs, &objp->circ_aref)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->size  )) {
%		return (FALSE);
%	} 
%	if (!xdr_pwr_tUInt32(xdrs, &objp->last_idx  )) {
%		return (FALSE);
%	} 
%	if (!xdr_pwr_tUInt32(xdrs, &objp->first_idx  )) {
%		return (FALSE);
%	} 
%	if (!xdr_pwr_tUInt32(xdrs, &objp->offset  )) {
%		return (FALSE);
%	} 
%	if (!xdr_opaque(xdrs, (char *)objp->buf, bufsize)) {
%		return (FALSE);
%	}
%	return (TRUE);
%
%}
#endif


struct  net_sUpdateCircBuffer {
  net_sMessage		hdr;
  pwr_tUInt32		info_size;	
  pwr_sAttrRef		circ_aref[10];
  pwr_tUInt32		resolution[10];
  pwr_tUInt32		samples[10];
  pwr_tUInt32		last_idx[10];
  pwr_tUInt32		offset[10];
  pwr_tUInt32		bufsize[10];
};

#ifdef RPC_HDR
%
%
%typedef struct {
%  net_sMessage 	hdr;		/**< Header */
%  pwr_tStatus          sts;		/**< Status */
%  pwr_tUInt32		info_size;
%  pwr_sAttrRef		circ_aref[10]; 	/**< CircBuffer attribute reference. */
%  pwr_tUInt32		first_idx[10]; 	/**< First index in buffer. */
%  pwr_tUInt32		last_idx[10]; 	/**< Last index in buffer. */
%  pwr_tUInt32		offset[10]; 	/**< Offset for next update. */
%  pwr_tUInt32		size[10]; 	/**< Returned number of samples. */
%  pwr_tUInt32		bufsize[10]; 	/**< Size data for each item in bytes. */
%  pwr_tUInt32		bsize; 		/**< Total size data in bytes. */
%  char			buf[1]; 	/**< Data */
%} net_sUpdateCircBufferR;
%
%bool_t xdr_net_sUpdateCircBufferR(XDR *xdrs, net_sUpdateCircBufferR *objp);
%
#elif defined RPC_XDR
%
%bool_t
%xdr_net_sUpdateCircBufferR(XDR *xdrs, net_sUpdateCircBufferR *objp)
%{
%	pwr_tStatus 	sts;
%	pwr_tUInt32 	bsize;
%	pwr_tUInt32 	info_size;
%	int	 	i;
% 
%	if (xdrs->x_op == XDR_DECODE) {
%		sts = (int) ntohl(objp->sts);
%		bsize = (int) ntohl(objp->bsize);
%		info_size = (int) ntohl(objp->info_size);
%	} else {
%		sts = objp->sts;
%		bsize = objp->bsize;
%		info_size = objp->info_size;
%	}
%
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tStatus(xdrs, &objp->sts)) {
%		return (FALSE);
%	}
%
%	if (EVEN(sts)) {
%		if (!xdr_pwr_tUInt32(xdrs, &objp->info_size  )) {
%			return (FALSE);
%		}
%		return (TRUE); 
%       }  
%
%
%	if (!xdr_pwr_tUInt32(xdrs, &objp->bsize  )) {
%		return (FALSE);
%	} 
%	if (!xdr_pwr_tUInt32(xdrs, &objp->info_size  )) {
%		return (FALSE);
%	} 
%	for ( i = 0; i < info_size; i++) {
%	  if (!xdr_pwr_sAttrRef(xdrs, &objp->circ_aref[i])) {
%		return (FALSE);
%	  }
%	  if (!xdr_pwr_tUInt32(xdrs, &objp->last_idx[i]  )) {
%		 return (FALSE);
%	  } 
%	  if (!xdr_pwr_tUInt32(xdrs, &objp->first_idx[i]  )) {
%		return (FALSE);
%	  } 
%	  if (!xdr_pwr_tUInt32(xdrs, &objp->offset[i]  )) {
%		return (FALSE);
%	  } 
%	  if (!xdr_pwr_tUInt32(xdrs, &objp->size[i]  )) {
%		return (FALSE);
%	  } 
%	  if (!xdr_pwr_tUInt32(xdrs, &objp->bufsize[i]  )) {
%		return (FALSE);
%	  } 
%	}
%	if (!xdr_opaque(xdrs, (char *)objp->buf, bsize)) {
%		return (FALSE);
%	}
%	return (TRUE);
%
%}
#endif

#ifdef RPC_HDR
%
%
%typedef struct {
%  net_sMessage		hdr;
%  pwr_tUInt32		cidcnt;
%  pwr_tUInt32		attrobjects;
%  pwr_tClassId		cid[1];
%} net_sClassList;
%
%bool_t xdr_net_sClassList(XDR *xdrs, net_sClassList *objp);
%
#elif defined RPC_XDR
%
%bool_t
%xdr_net_sClassList(XDR *xdrs, net_sClassList *objp)
%{
%	int 		cidcnt;
%       int		i;
% 
%	if (xdrs->x_op == XDR_DECODE) {
%		cidcnt = (int) ntohl(objp->cidcnt);
%	} else {
%		cidcnt = objp->cidcnt;
%	}
%
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%
%	if (!xdr_pwr_tUInt32(xdrs, &objp->cidcnt  )) {
%		return (FALSE);
%	} 
%	if (!xdr_pwr_tUInt32(xdrs, &objp->attrobjects  )) {
%		return (FALSE);
%	} 
%	for ( i = 0; i < cidcnt; i++) {
%	  if (!xdr_pwr_tClassId(xdrs, &objp->cid[i]))
%		return (FALSE);
%	}
%	return (TRUE);
%
%}
#endif


#ifdef RPC_HDR
%
%
%typedef struct {
%  net_sMessage 	hdr;		/**< Header */
%  pwr_tStatus          sts;		/**< Status */
%  pwr_tUInt32		listcnt; 	/**< Number of objects found. */  
%  pwr_sAttrRef		classlist[1]; 	/**< List of instances */
%} net_sClassListR;
%
%bool_t xdr_net_sClassListR(XDR *xdrs, net_sClassListR *objp);
%
#elif defined RPC_XDR
%
%bool_t
%xdr_net_sClassListR(XDR *xdrs, net_sClassListR *objp)
%{
%	int 		listcnt;
%	pwr_tStatus 	sts;
%       int		i;
% 
%	if (xdrs->x_op == XDR_DECODE) {
%		listcnt = (int) ntohl(objp->listcnt);
%		sts = (int) ntohl(objp->sts);
%	} else {
%		listcnt = objp->listcnt;
%		sts = objp->sts;
%	}
%
%	if (!xdr_net_sMessage(xdrs, &objp->hdr)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tStatus(xdrs, &objp->sts)) {
%		return (FALSE);
%	}
%
%	if (EVEN(sts)) {
%		listcnt = 0;
%		if (!xdr_pwr_tUInt32(xdrs, (unsigned int *)&listcnt)) {
%				return (FALSE);
%		}
%		return (TRUE); 
%       }  
%
%
%	if (!xdr_pwr_tUInt32(xdrs, &objp->listcnt  )) {
%		return (FALSE);
%	} 
%	for ( i = 0; i < listcnt; i++) {
%	  if (!xdr_pwr_sAttrRef(xdrs, &objp->classlist[i])) {
%		return (FALSE);
%         }
%	}
%	return (TRUE);
%
%}
#endif





#ifdef RPC_HDR
%/*  Max # of gohs in a cache_fetch_response. The message will not be
%    bigger than cvol_cMaxSize bytes.  */
%
%/* Callable functions.  */
%
%void *
%net_Alloc (
%  pwr_tStatus	*sts,
%  qcom_sPut	*put,
%  int		size,
%  net_eMsg	subtype
%);
%
%pwr_tBoolean
%net_Free (
%  pwr_tStatus	*sts,
%  void		*mp
%);
%
%pwr_tBoolean
%net_Send (
%  pwr_tStatus	*sts,
%  qcom_sQid	*tgt,
%  qcom_sPut	*put
%);
%
%pwr_tBoolean
%net_Reply (
%  pwr_tStatus	*sts,
%  qcom_sGet	*get,
%  qcom_sPut	*put,
%  pwr_tSid	sid
%);
%
%pwr_tBoolean
%net_Put (
%  pwr_tStatus	*sts,
%  qcom_sQid	*tgt,
%  void		*mp,
%  net_eMsg	type,
%  unsigned int	id,
%  int		size,
%  pwr_tSid	sid
%);
%
%void *
%net_Receive (
%  pwr_tStatus	*sts,
%  qcom_sGet	*get,
%  int		tmo
%);
%
%void *
%net_Request (
%  pwr_tStatus	 *sts,
%  qcom_sQid	 *tgt,
%  qcom_sPut	 *put,
%  qcom_sGet	 *get,
%  net_eMsg	 gtype,
%  pwr_tBitMask  flags,
%  pwr_tSid	sid
%);
%
%pwr_tBoolean
%net_Connect (
%  pwr_tStatus	*sts,
%  qcom_sAid	*aid,
%  qcom_sQid	*qid,
%  qcom_sQattr	*attr,
%  const char	*name
%);
%
%static const net_sTime      net_cNTime      = {0, 0};
%
%pwr_tTime net_NetTimeToTime( const net_sTime *nt);
%net_sTime net_TimeToNetTime( const pwr_tTime *t);
%pwr_tDeltaTime net_NetTimeToDeltaTime( const net_sTime *nt);
%net_sTime net_DeltaTimeToNetTime( const pwr_tDeltaTime *t);
%net_sTime net_DeltaTimeToNetTime( const pwr_tDeltaTime *t);
%int net_GetTime( net_sTime *nt);
%int net_StringToAddr( char *str, struct in_addr *naddr);
%
%#endif
%
%#if defined __cplusplus
%}
%#endif
#endif
