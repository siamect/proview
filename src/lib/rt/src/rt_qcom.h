/* 
 * Proview   $Id: rt_qcom.h,v 1.6 2006-01-13 16:32:39 claes Exp $
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
 */

#ifndef rt_qcom_h
#define rt_qcom_h

/* rt_qcom.h -- Queue communication */

/** @addtogroup QCOM */
/*@{*/

#include <rpc/rpc.h>

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_platform_h
# include "co_platform.h"
#endif

#if 0
#pragma builtins
#endif

#ifdef	OS_ELN
# include $vaxelnc
# include $mutex
#elif defined OS_LYNX || defined OS_LINUX
# include <pthread.h>
#endif

#ifdef	OS_ELN
# include unistd
#else
# include <unistd.h>
#endif

#ifndef rt_gdb_msg_h
# include "rt_gdb_msg.h"
#endif

#if 0
#ifndef co_cdh_h
#include "co_cdh.h"
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define qcom_cNQix		0
#define qcom_cNNid		0
#define qcom_cNAix		0
#define qcom_cTmoNone		0
#define qcom_cTmoEternal	-1
#define qcom_cIloopBack		(1<<31 | 0)
#define qcom_cInetEvent		(1<<31 | 100)
#define qcom_cIapplEvent	(1<<31 | 101)
#define qcom_cImhAllHandlers	(1<<31 | 102)
#define qcom_cImhAllOutunits	(1<<31 | 103)
#define qcom_cIhdServer		(1<<31 | 104)
#define qcom_cIhdClient		(1<<31 | 105)
#define qcom_cInacp		(1<<31 | 106)
#define qcom_cIini		(1<<31 | 107)

/** @defgroup QCOM_DS Data Structures
 *  @ingroup QCOM
 *  @{
 */


typedef int qcom_tBus;
typedef int qcom_tQix;
typedef int qcom_tAix;
typedef int qcom_tRid;

typedef enum {
  qcom_eBtype__ = 0,
  qcom_eBtype_system,
  qcom_eBtype_qcom,
  qcom_eBtype_event,
  qcom_eBtype_
} qcom_eBtype;

typedef enum {
  qcom_eStype__ = 0,
  qcom_eStype_linkConnect,
  qcom_eStype_linkDisconnect,
  qcom_eStype_linkActive,
  qcom_eStype_linkStalled,
  qcom_eStype_applConnect,
  qcom_eStype_applDisconnect,
  qcom_eStype_
} qcom_eStype;

typedef enum {
  qcom_eQtype__ = 0,
  qcom_eQtype_private,
  qcom_eQtype_forward,
  qcom_eQtype_broadcast,
  qcom_eQtype_event,
  qcom_eQtype_
} qcom_eQtype;


/**
 * @brief ZZZ Text that appers when the Data structure is listed.
 *
 * More detailed text  
 */
typedef struct {
  qcom_tQix	qix;		/**< Queue index    */
  pwr_tNodeId	nid;		/**< Node identity  */
} qcom_sQid;

static const qcom_sQid qcom_cNQid		= {0, 0};
static const qcom_sQid qcom_cQnetEvent		= {qcom_cInetEvent, 0};
static const qcom_sQid qcom_cQapplEvent		= {qcom_cIapplEvent, 0};
static const qcom_sQid qcom_cQmhAllHandlers	= {qcom_cImhAllHandlers, 0};
static const qcom_sQid qcom_cQmhAllOutunits	= {qcom_cImhAllOutunits, 0};
static const qcom_sQid qcom_cQhdServer		= {qcom_cIhdServer, 0};
static const qcom_sQid qcom_cQhdClient		= {qcom_cIhdClient, 0};
static const qcom_sQid qcom_cQnacp		= {qcom_cInacp, 0};
static const qcom_sQid qcom_cQini		= {qcom_cIini, 0};

//! Application identity
typedef struct {
  qcom_tAix	aix;		/**< Application index */
  pwr_tNodeId	nid;		/**< Node identity */
} qcom_sAid;

static const qcom_sAid qcom_cNAid = {0, 0};

//! Qcom application
typedef struct {
  qcom_sAid	aid;
  pid_t		pid;
} qcom_sAppl;

//! Qcom event
typedef struct {
  qcom_sAid	aid;
  pid_t		pid;
  int		mask;
} qcom_sEvent;

//! Queue attributes
typedef struct {
  qcom_eQtype	type;
  unsigned int	quota;
} qcom_sQattr;

//! Qcom type
typedef struct {
  qcom_eBtype	b;
  qcom_eStype	s;
} qcom_sType;

//! Put data structure.
typedef struct {
  qcom_sQid	reply;
  qcom_sType	type;
  unsigned int	size;
  void		*data;
} qcom_sPut;

//! Get data structure
typedef struct {
  qcom_sAid	sender;
  pid_t		pid;
  qcom_sQid	receiver;
  qcom_sQid	reply;
  qcom_sType	type;
  qcom_tRid	rid;
  unsigned int	maxSize;
  unsigned int	size;
  void		*data;
} qcom_sGet;

//! Node status
typedef union {
  pwr_tBitMask m;
  //! Bitmask representation
  pwr_32Bits (
    pwr_Bits( initiated	, 1),
    pwr_Bits( connected	, 1),
    pwr_Bits( active	, 1),
    pwr_Bits( fill_0	, 5),,,,,

    pwr_Bits( fill_1	, 8),,,,,,,,
    pwr_Bits( fill_2	, 8),,,,,,,,
    pwr_Bits( fill_3	, 8),,,,,,,
  ) b;

#define qcom_mNode__		0
#define qcom_mNode_initiated	pwr_Bit(0)
#define qcom_mNode_connected	pwr_Bit(1)
#define qcom_mNode_active	pwr_Bit(2)

#define qcom_mNode_up		(qcom_mNode_connected|qcom_mNode_active)
#define qcom_mNode_down		(~qcom_mNode_up)
#define qcom_mNode_		(~qcom_mNode__)

} qcom_mNode;


//! Data for a Qcom node.
typedef struct {
  pwr_tNodeId	nid;		//!< node index
  qcom_mNode	flags;		//!< node flags
  char		name[80];       //!< node name
  co_eOS	os;		//!< operating system
  co_eHW	hw;		//!< hardware
  co_eBO	bo;		//!< big/little endian
  co_eFT	ft;		//!< float type
} qcom_sNode;


/** @} */

/** @defgroup QCOM_FC Function Calls
 *  @ingroup QCOM
 *  @{
 */

/* Function prototypes.  */

int			qcom_AidCompare (
  const qcom_sAid	*a1,
  const qcom_sAid	*a2
);

pwr_tBoolean		qcom_AidIsEqual (
  const qcom_sAid	*a1,
  const qcom_sAid	*a2
);

pwr_tBoolean		qcom_AidIsNotEqual (
  const qcom_sAid	*a1,
  const	qcom_sAid	*a2
);

pwr_tBoolean		qcom_AidIsNotNull (
  const qcom_sAid	*a
);

pwr_tBoolean		qcom_AidIsNull (
  const qcom_sAid	*a
);

void*			qcom_Alloc (
  pwr_tStatus		*sts,
  unsigned int		size
);

pwr_tBoolean		qcom_AttachQ (
  pwr_tStatus		*sts,
  const qcom_sQid	*qid
);

pwr_tBoolean		qcom_Bind (
  pwr_tStatus		*sts,
  const qcom_sQid	*myQ,
  const qcom_sQid	*toQ
);

pwr_tBoolean		qcom_CreateQ (
  pwr_tStatus		*sts,
  qcom_sQid		*myQ,
  qcom_sQattr		*attr,
  const char		*qname
);

pwr_tBoolean		qcom_DeleteQ (
  pwr_tStatus		*sts,
  const qcom_sQid	*myQ
);


pwr_tBoolean		qcom_SignalAnd (
  pwr_tStatus		*status,
  const qcom_sQid	*eid,
  int			mask
);

pwr_tBoolean		qcom_SignalOr (
  pwr_tStatus		*status,
  const qcom_sQid	*eid,
  int			mask
);

pwr_tBoolean		qcom_WaitAnd (
  pwr_tStatus		*status,
  const qcom_sQid	*qid,
  const qcom_sQid	*eid,
  int			mask,
  int			tmo
);

pwr_tBoolean		qcom_WaitOr (
  pwr_tStatus		*status,
  const qcom_sQid	*qid,
  const qcom_sQid	*eid,
  int			mask,
  int			tmo
);

pwr_tBitMask		qcom_EventMask (
  pwr_tStatus		*status,
  const qcom_sQid	*eid
);

pwr_tBoolean		qcom_Exit (
  pwr_tStatus		*sts
);

pwr_tBoolean		qcom_Free (
  pwr_tStatus		*sts,
  void			*buffer
);

pwr_tBoolean		qcom_Init (
  pwr_tStatus		*sts,
  qcom_sAid		*aid,
  const char		*aname
);

void*			qcom_Get (
  pwr_tStatus		*sts,
  const qcom_sQid	*myQ,
  qcom_sGet		*get,
  int			tmo_ms
);

qcom_tBus		qcom_MyBus (
  pwr_tStatus		*sts
);

pwr_tBoolean		qcom_MyNode (
  pwr_tStatus		*sts,
  qcom_sNode		*node
);

pwr_tBoolean		qcom_NextNode (
  pwr_tStatus		*sts,
  qcom_sNode		*node,
  pwr_tNodeId		nid
);

pwr_tBoolean		qcom_Put (
  pwr_tStatus		*sts,
  const qcom_sQid	*receiver,
  qcom_sPut		*put
);

int			qcom_QidCompare (
  const qcom_sQid	*q1,
  const qcom_sQid	*q2
);

pwr_tBoolean		qcom_QidIsEqual (
  const qcom_sQid	*q1,
  const qcom_sQid	*q2
);

pwr_tBoolean		qcom_QidIsNotEqual (
  const qcom_sQid	*q1,
  const qcom_sQid	*q2
);

pwr_tBoolean		qcom_QidIsNull (
  const qcom_sQid	*q
);

pwr_tBoolean		qcom_QidIsNotNull (
  const qcom_sQid	*q
);

char *			qcom_QidToString (
  char			*s,
  qcom_sQid		*qid,
  int			prefix
);

pwr_tBoolean		qcom_Reply (
  pwr_tStatus		*sts,
  qcom_sGet		*get,
  qcom_sPut		*put
);

void*			qcom_Request (
  pwr_tStatus		*sts,
  const qcom_sQid	*receiver,
  qcom_sPut		*put,
  const qcom_sQid	*myQ,
  qcom_sGet		*get,
  int			tmo_ms
);

pwr_tBoolean		qcom_Unbind (
  pwr_tStatus		*sts,
  const qcom_sQid	*myQ, 
  const qcom_sQid	*fromQ
);




bool_t 
xdr_qcom_sAid(XDR *xdrs, qcom_sAid *objp);

bool_t 
xdr_qcom_sQid(XDR *xdrs, qcom_sQid *objp);




#ifdef __cplusplus
}
#endif

#endif

/** @} */
/** @} */
