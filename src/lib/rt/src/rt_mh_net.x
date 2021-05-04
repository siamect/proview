#ifdef RPC_HDR
%/* 
% * Proview   $Id: rt_mh_net.x,v 1.5 2008-01-24 09:58:41 claes Exp $
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
%#ifndef rt_mh_net_h
%#define rt_mh_net_h
%
%#if !defined(__rpc_rpc_h) && !defined(__RPC__RPC_H) && !defined(__RPC_HEADER__)
%# include <rpc/rpc.h>
%#endif
%
%#ifndef co_xdr_h
%# include "co_xdr.h"
%#endif
%
%#ifndef rt_net_h
%#include "rt_net.h"
%#endif
%
%#ifndef rt_mh_h
%# include "rt_mh.h"
%#endif
%
%#ifndef rt_qcom_h
%# include "rt_qcom.h"
%#endif
%
%#define mh_cMsgClass      	201
%#define mh_cVersion		5
%#define mh_cProcHandler	111
%#define mh_cProcAllHandlers	qcom_cQmhAllHandlers
%#define mh_cProcAllOutunits	qcom_cQmhAllOutunits
%#define mh_cSendRcvTmo		200
%
%/* Bit masks */
%
%#define mh_mEventStatus_NotRet	(1 << 0)
%#define mh_mEventStatus_NotAck	(1 << 1)
%#define mh_mEventStatus_Block	(1 << 2) 
%
#endif

#ifndef RPC_HDR
%#pragma GCC diagnostic ignored "-Waddress-of-packed-member"
%#pragma GCC diagnostic ignored "-Wunused-variable"
#endif



typedef pwr_tUInt32 mh_mEventStatus;

%/** 
% * @ingroup MSGH_DS
% * @brief Type of event that the message handler recognizes.
% */
enum mh_eEvent {
  mh_eEvent__		= 0,    
  mh_eEvent_Ack		= 1,
  mh_eEvent_Block	= 2,
  mh_eEvent_Cancel	= 3,
  mh_eEvent_CancelBlock	= 4,
  mh_eEvent_Missing	= 5,
  mh_eEvent_Reblock	= 6,
  mh_eEvent_Return	= 7,
  mh_eEvent_Unblock	= 8,
  mh_eEvent_InfoSuccess	= 32,
  mh_eEvent_Alarm	= 64,
  mh_eEvent_MaintenanceAlarm = 128,
  mh_eEvent_SystemAlarm	= 256,
  mh_eEvent_UserAlarm1	= 512,
  mh_eEvent_UserAlarm2	= 1024,
  mh_eEvent_UserAlarm3	= 2048,
  mh_eEvent_UserAlarm4	= 4096,
  mh_eEvent_Info	= 8192,
  mh_eEvent_   		= 8193
};

%/** 
% * @ingroup MSGH_DS
% * @brief Event prio
% *
% * This enumeration defines the priority of the event. 
% * This affects how the message handler treats the generated message. 
% * For A and B priorities the alarm window displays number of alarms, 
% * number of unacknowledged alarms, identities of the alarms, and associated 
% * message texts. For C and D priorities, only number of alarms and number of 
% * unacknowledged alarms are shown. 
% */
enum mh_eEventPrio {
  mh_eEventPrio__ = 0,
%  /// Priority A, the highest priority. Alarm messages of this priority are
%  /// shown in the upper part of the alarm window. 
  mh_eEventPrio_A = 67,
%  /// Priority B. These messages are shown in the lower part of the alarm
%  /// window.
  mh_eEventPrio_B = 66,
%  /// Priority C.
  mh_eEventPrio_C = 65,
%  /// Priority D. This is the lowest priority. 
  mh_eEventPrio_D = 64,
  mh_eEventPrio_  = 63
};

%/** 
% * @ingroup MSGH_DS
% * @brief Type of supervision data.
% * 
% * This enumeration is used to indicate what kind of data is supplied 
% * in an application message (See mh_sApplMessage).
% */
enum mh_eSupType {
  mh_eSupType__		= 0,
%  /// Digital supervision data
  mh_eSupType_Digital	= 1,
%  /// Analog supervision data
  mh_eSupType_Analog	= 2,
  mh_eSupType_None	= 3,
%  /// ZZZ
  mh_eSupType_Link	= 4,
%  /// ZZZ
  mh_eSupType_Cycle	= 4,
  mh_eSupType_		= 5
};

enum mh_eMsg {
  mh_eMsg__			= 0,
  mh_eMsg_ApplConnect		= 1,
  mh_eMsg_ApplDisconnect	= 2,
  mh_eMsg_ApplGetMsgInfo	= 3,
  mh_eMsg_ApplMessage		= 4,
  mh_eMsg_ApplReply		= 5,
  mh_eMsg_ApplReturn		= 6,

  mh_eMsg_Event			= 8,

  mh_eMsg_HandlerDisconnect	= 10,
  mh_eMsg_HandlerHello		= 11,
  mh_eMsg_HandlerSync		= 12,
  mh_eMsg_HandlerAlarmStatus	= 13,

  mh_eMsg_OutunitAck		= 15,
  mh_eMsg_OutunitBlock		= 16,
  mh_eMsg_OutunitDisconnect	= 18,
  mh_eMsg_OutunitHello		= 19,
  mh_eMsg_OutunitInfo		= 20,
  mh_eMsg_OutunitSync		= 21,
  mh_eMsg_OutunitClear		= 22,
  mh_eMsg_OutunitAlarmReq	= 23,

  mh_eMsg_ProcDown		= 24,

  mh_eMsg_StopScanSup		= 25,
  mh_eMsg_StartScanSup		= 26,

  mh_eMsg_Sync			= 27,

  mh_eMsg_                      = 28
};

enum mh_eOutunitType{
  mh_eOutunitType__	    = 0,    
  mh_eOutunitType_Operator  = 1,
  mh_eOutunitType_Printer   = 2,
  mh_eOutunitType_File	    = 3,
  mh_eOutunitType_Terminal  = 4,
  mh_eOutunitType_Logger    = 5,
  mh_eOutunitType_Post      = 6,
  mh_eOutunitType_SevHistEvents = 7,
  mh_eOutunitType_  	    = 8
};

enum mh_eSource {
  mh_eSource__		  = 0,
  mh_eSource_Scanner	  = 1,
  mh_eSource_Application  = 2,
  mh_eSource_Handler	  = 3,
  mh_eSource_Outunit	  = 4,
  mh_eSource_Self	  = 5,
  mh_eSource_Pcm	  = 6,
  mh_eSource_	          = 7
};

%/**
% * @ingroup MSGH_DS
% * @brief Defines a bit pattern. 
% */
enum mh_mEventFlags {
%  /// Setting this flag enables a return message associated with this message
%  /// to be shown in the event list. 
  mh_mEventFlags_Return		= 0x01,
%  /// Setting this flag enables an acknowledgement message associated with
%  /// this message to be shown in the event list. 
  mh_mEventFlags_Ack		= 0x02,
  mh_mEventFlags_Bell		= 0x04,
  mh_mEventFlags_Force		= 0x08,
  mh_mEventFlags_InfoWindow	= 0x10,
  mh_mEventFlags_Returned	= 0x20,
  mh_mEventFlags_NoObject	= 0x40
};

%/**
% * @ingroup MSGH_DS
% * @brief Data describing supervision of an analog value 
% */
struct mh_sASupInfo {
%  /// The value of the supervised attribute at the time when the message was
%  /// generated. 
  pwr_tFloat32		ActualValue;
%  /// The limit, at which a message will be generated.
  pwr_tFloat32		CtrlLimit;
%  /// How much the supervised value will have to change before a return
%  /// message will be issued. 
  pwr_tFloat32		Hysteres;
%  /// Indicates whether the message will be generated when the limit is
%  /// exceeded (TRUE) or the value falls below it (FALSE).
  pwr_tBoolean		High;
%  /// A string representing the unit of the value. 
  pwr_tString16		Unit;
};

%/**
% * @ingroup MSGH_DS
% *
% * @brief Data describing supervision of a digital value 
% */
struct mh_sDSupInfo {
%  /// The value of the supervised attribute at the time when the message was
%  /// generated.
  pwr_tBoolean		ActualValue;
%  /// Indicates whether a high or a low value generates a message.
  pwr_tBoolean		High;
};

#ifdef RPC_HDR
%/**
% * @ingroup MSGH_DS 
% * @brief Data describing supervision data.
% */
%struct mh_uSupInfo {
%	mh_eSupType SupType;
%	union { mh_sASupInfo A;
%		mh_sDSupInfo D;
%  /// Union that contains either mh_sASupInfo or mh_sDSupInfo.
%	} mh_uSupInfo_u;
%};
%
%typedef struct mh_uSupInfo mh_uSupInfo;
%bool_t xdr_mh_uSupInfo(XDR *xdrs, mh_uSupInfo *objp);
%
#elif defined RPC_XDR
%bool_t xdr_mh_uSupInfo(XDR *xdrs, mh_uSupInfo *objp)
%{
%       int SupType; 
%
%       if (xdrs->x_op == XDR_ENCODE) 
%         	SupType = objp->SupType;
%       else  
%         	SupType = (int)ntohl(objp->SupType);
%
%	if (!xdr_mh_eSupType(xdrs, &objp->SupType)) {
%		return (FALSE);
%	}
%
%	switch (SupType) {
%	case mh_eSupType_Analog:
%		if (!xdr_mh_sASupInfo(xdrs, &objp->mh_uSupInfo_u.A)) {
%			return (FALSE);
%		}
%		break;
%	case mh_eSupType_Digital:
%		if (!xdr_mh_sDSupInfo(xdrs, &objp->mh_uSupInfo_u.D)) {
%			return (FALSE);
%		}
%		break;
%	default:
%		return (TRUE);
%	}
%	return (TRUE);
%}
#endif

struct mh_sHead {
  pwr_tBoolean		xdr;
  co_sPlatform		platform;
  pwr_tUInt32		ver; 
  mh_eSource		source;
  net_sTime		birthTime;
  mh_eMsg		type;
  qcom_sQid		qid;
  pwr_tNodeIndex	nix;
  pwr_tObjid		outunit;
  pwr_tObjid		aid;
  pwr_tUInt32		ackGen;
  pwr_tUInt32		blockGen;
  pwr_tUInt32		selGen;  
  pwr_tUInt32		eventGen;
  pwr_tUInt32		eventIdx;
};

struct mh_sOutunitAck {
  pwr_tUInt32		ackGen;
  pwr_tUInt32		targetIdx;
};

struct mh_sOutunitBlock {
  pwr_tUInt32		blockGen;
  pwr_tObjid		object;
  pwr_tObjid		outunit;
  mh_eEventPrio		prio;
  net_sTime		time;
};

struct mh_sSelL {
  pwr_tString80		objName;
  pwr_tUInt32		len;
};

struct mh_sEventId {
  pwr_tNodeIndex	Nix;
  net_sTime		BirthTime;
  pwr_tUInt32		Idx;
};

struct mh_sMsgInfo {
  mh_sEventId		Id;
  pwr_tObjid		Object_V3;
  pwr_tObjid		Outunit;
  pwr_tObjid		SupObject_V3;
  mh_mEventFlags	EventFlags;
  net_sTime		EventTime;
  pwr_tString80		EventName_V3;
  mh_eEvent		EventType;
  mh_eEventPrio		EventPrio;
};

struct mh_sAck {
  mh_sMsgInfo		Info;
  mh_sEventId		TargetId;
  net_sTime		DetectTime;
  pwr_tObjid		Outunit;
  mh_uSupInfo		SupInfo;
  pwr_sAttrRef		Object;
  pwr_sAttrRef	 	SupObject;
  pwr_tAName		EventName;
};

struct mh_sBlock {
  mh_sMsgInfo		Info;
  mh_sEventId		TargetId;
  net_sTime		DetectTime;
  pwr_tObjid		Outunit;
  mh_mEventStatus	Status;
  pwr_sAttrRef		Object;
  pwr_sAttrRef	 	SupObject;
  pwr_tAName		EventName;
};

struct mh_sMessage {
  mh_sMsgInfo		Info;
  pwr_tString80		EventText;
  mh_mEventStatus	Status;
  mh_uSupInfo		SupInfo;
  pwr_sAttrRef		EventSound;
  pwr_tString256	EventMoreText;
  pwr_sAttrRef		Object;
  pwr_sAttrRef	 	SupObject;
  pwr_tAName		EventName;
  pwr_tString40		Receiver;
};

struct mh_sReturn {
  mh_sMsgInfo		Info;
  pwr_tString80		EventText;
  mh_sEventId		TargetId;
  net_sTime		DetectTime;
  mh_uSupInfo		SupInfo;
  pwr_sAttrRef		Object;
  pwr_sAttrRef	 	SupObject;
  pwr_tAName		EventName;
};

struct mh_sOutunitAlarmReq {
  pwr_tNodeIndex	Nix;
  pwr_tUInt32		Count;		/* # of entries */
  pwr_tUInt32		Idx[20];
};

#ifdef RPC_HDR
%struct mh_sOutunitInfo {
%  pwr_tUInt16		type;
%  pwr_tUInt16		selEventType;
%  pwr_tUInt32		selGen;
%  pwr_tUInt32		selSize;
%  /* Here comes select list if needed */
%};
%typedef struct mh_sOutunitInfo mh_sOutunitInfo;
%
%bool_t xdr_mh_sOutunitInfo(XDR *xdrs, mh_sOutunitInfo *op);
%
#elif defined RPC_XDR
%
%bool_t xdr_mh_sOutunitInfo(XDR *xdrs, mh_sOutunitInfo *op)
%{
%	int selSize;
%	int i;
%	mh_sSelL *selL;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		selSize = (int) ntohl(op->selSize);
%	} else {
%		selSize = op->selSize;
%	}	
%
%	if (!xdr_pwr_tUInt16(xdrs, &op->type)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt16(xdrs, &op->selEventType)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &op->selGen)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &op->selSize)) {
%		return (FALSE);
%	}
%
%	for (i = 0, selL = (mh_sSelL *)(op + 1); i < selSize; i++, selL++ ) {
%		if (!xdr_mh_sSelL(xdrs, selL))
%			return (FALSE);
%	}
%	return (TRUE);
%}
#endif


#ifdef RPC_HDR
%
%/* Prototypes */ 
%
%pwr_tStatus 
%mh_NetSendMessage(
%  qcom_sQid *qid,
%  co_sPlatform *recPlatform,
%  int prio,
%  int subtype,               
%  unsigned int id,
%  mh_sHead *hp,
%  unsigned int size
%);
%
%pwr_tStatus 
%mh_NetXdrMessage(
%  XDR *xdrs,
%  int subtype,               
%  mh_sHead *hp
%); 
%
#endif
#ifdef RPC_HDR
%
%#endif
#endif

struct mh_sAlarmSts {
  pwr_tUInt32		Idx;
  mh_mEventStatus	Status;
};

#ifdef RPC_HDR
%
%typedef struct {
%  pwr_tNodeIndex	Nix;
%  pwr_tUInt32		Count;		/* # of entries */
%  mh_sAlarmSts         Sts[1];		/* Specifications (dynamic) */
%} mh_sAlarmStatus;
%
%bool_t xdr_mh_sAlarmStatus(XDR *xdrs, mh_sAlarmStatus *objp);
%
#elif defined RPC_XDR
%
%bool_t xdr_mh_sAlarmStatus(XDR *xdrs, mh_sAlarmStatus *objp)
%{
%	int count;
%
%	if (xdrs->x_op == XDR_DECODE) {
%		count = (int) ntohl(objp->Count);
%	} else {
%		count = objp->Count;
%	}
%
%	if (!xdr_pwr_tUInt32(xdrs, &objp->Nix)) {
%		return (FALSE);
%	}
%	if (!xdr_pwr_tUInt32(xdrs, &objp->Count)) {
%		return (FALSE);
%	}
%	if (!xdr_vector(xdrs, (char *)objp->Sts, count, sizeof(mh_sAlarmSts), (xdrproc_t)xdr_mh_sAlarmSts)) {
%		return (FALSE);
%	}
%	return (TRUE);
%}
#endif

