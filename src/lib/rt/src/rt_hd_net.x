#ifdef RPC_HDR
%#ifndef rt_hd_net_h
%#define rt_hd_net_h
%
%/* rt_hd_net.h -- 
%
%   PROVIEW/R
%   Copyright (C) 1998 by Mandator AB.
%
%     */
%
%#if !defined(__rpc_rpc_h) && !defined(__RPC__RPC_H) && !defined(__RPC_HEADER__)
%# include <rpc/rpc.h>
%#endif
%
%#ifndef co_xdr_h
%# include "co_xdr.h"
%#endif
%
%#include "pwr.h"
%#include "pwr_class.h"
%#include "rt_qcom.h"
%
%#define hd_cAttrNameLen 	160
%#define hd_cNodeNameLen	8
%#define hd_cMsgClass		204
%#define hd_cVersion		5
#endif

enum hd_eMsg {
  hd_eMsg__		= 0,
  hd_eMsg_hello		= 1,
  hd_eMsg_adata		= 2,
  hd_eMsg_sdata		= 3,
  hd_eMsg_ack		= 4,
  hd_eMsg_
};

enum hd_eSdata {
  hd_eSdata__		= 0,
  hd_eSdata_first	= 1,
  hd_eSdata_middle	= 2,
  hd_eSdata_last	= 3,
  hd_eSdata_
};

enum hd_eSource {
  hd_eSource__		= 0,
  hd_eSource_adm	= 1,
  hd_eSource_store	= 2,
  hd_eSource_client	= 3,
  hd_eSource_
};


struct hd_sHead {
  pwr_tBoolean		xdr;
  co_sPlatform		platform;
  pwr_tUInt32		ver; 
  pwr_tTime		birthTime;
  pwr_tUInt32		generation; 
  hd_eSource		source;
  hd_eMsg		type;
  qcom_sQid		qid;
};

struct hd_sAclient {
  pwr_tObjid		oid;
  pwr_tString132	name;
  pwr_sAttrRef		aref;
  pwr_tStatus		sts;
  pwr_tTypeId		tid;
  pwr_tFloat32		deadBand;
  pwr_tObjid		server[3];
};

struct hd_sServer {
  pwr_tObjid		oid;
  pwr_tInt32		column;
};

struct hd_sSclient {
  pwr_tObjid		oid;
  pwr_sAttrRef		aref;
  pwr_tInt32		sql_type;
  pwr_tUInt32		count;
  hd_sServer		server[3];
};

#ifdef RPC_HDR
%struct hd_sAdata {
%  pwr_tObjid		client;
%  pwr_tUInt32		count;
%  pwr_tBoolean		done;
%  /*
%     hd_sAclient	clients[count];
%  */
%};
%typedef struct hd_sAdata hd_sAdata;
%
%bool_t xdr_hd_sAdata();
%
#elif defined RPC_XDR
%
%bool_t
%xdr_hd_sAdata(xdrs, op)
%	XDR *xdrs;
%	hd_sAdata *op;
%{
%	int count;
%	int i;
%	hd_sAclient *cp;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(op->count);
%	} else {
%		count = op->count;
%	}	
%
%	if (!xdr_pwr_tObjid(xdrs, &op->client)) {
%		return (FALSE);
%	}
%
%	if (!xdr_pwr_tUInt32(xdrs, &op->count)) {
%		return (FALSE);
%	}
%
%	if (!xdr_pwr_tBoolean(xdrs, &op->done)) {
%		return (FALSE);
%	}
%
%	for (i = 0, cp = (hd_sAclient *)(op + 1); i < count; i++, cp++) {
%		if (!xdr_hd_sAclient(xdrs, cp))
%			return (FALSE);
%	}
%	return (TRUE);
%}
#endif

#ifdef RPC_HDR
%struct hd_sSdata {
%  pwr_tUInt32		info;
%  pwr_tUInt32		done_count;
%  pwr_tNodeId		nid;
%  pwr_tObjid		client;
%  pwr_tUInt32		count;
%  /*
%     hd_sSclient	clients[count];
%  */
%};
%typedef struct hd_sSdata hd_sSdata;
%
%bool_t xdr_hd_sSdata();
%
#elif defined RPC_XDR
%
%bool_t
%xdr_hd_sSdata(xdrs, op)
%	XDR *xdrs;
%	hd_sSdata *op;
%{
%	int count;
%	int i;
%	hd_sSclient *cp;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(op->count);
%	} else {
%		count = op->count;
%	}	
%
%	if (!xdr_pwr_tUInt32(xdrs, &op->info)) {
%		return (FALSE);
%	}
%
%	if (!xdr_pwr_tUInt32(xdrs, &op->done_count)) {
%		return (FALSE);
%	}
%
%	if (!xdr_pwr_tNodeId(xdrs, &op->nid)) {
%		return (FALSE);
%	}
%
%	if (!xdr_pwr_tObjid(xdrs, &op->client)) {
%		return (FALSE);
%	}
%
%	if (!xdr_pwr_tUInt32(xdrs, &op->count)) {
%		return (FALSE);
%	}
%
%	for (i = 0, cp = (hd_sSclient *)(op + 1); i < count; i++, cp++) {
%		if (!xdr_hd_sSclient(xdrs, cp))
%			return (FALSE);
%	}
%	return (TRUE);
%}
#endif

struct hd_sAck {
  pwr_tNodeId	nid;
  pwr_tObjid	client;
};

struct hd_sHello {
  pwr_tString80	name;
};

#ifdef RPC_HDR
%
%/* Prototypes */ 
%
%pwr_tStatus 
%hd_NetSendMessage(
%  qcom_sQid *qid,
%  co_sPlatform *recPlatform,
%  int subtype,               
%  hd_sHead *hp,
%  unsigned int size
%);
%
%pwr_tStatus 
%hd_NetXdrMessage(
%  XDR *xdrs,
%  int subtype,               
%  hd_sHead *hp
%); 
%
#endif

#ifdef RPC_HDR
%
%#endif
#endif
