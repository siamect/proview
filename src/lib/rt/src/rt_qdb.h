/* 
 * Proview   $Id: rt_qdb.h,v 1.11 2007-11-22 15:10:23 claes Exp $
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

#ifndef rt_qdb_h
#define rt_qdb_h

/* rt_qdb.h -- Queue communication database

   This include file contains the internal datastructures and
   data entities in QDB.  */

#if defined(OS_VMS) || defined(OS_ELN)
# pragma builtins
#endif

#ifdef	OS_ELN
# include $vaxelnc
# include $mutex
# include unistd
# include socket
#elif defined OS_LINUX
# include <unistd.h>
# include <sys/socket.h>
#else
# include <unistd.h>
# include <socket.h>
#endif

#if defined OS_LYNX || defined OS_LINUX
# include <netinet/in.h>
# include <pthread.h>
# include <signal.h>
#elif defined OS_ELN
# include in
#else
# include <in.h>
#endif

#ifndef rt_qcom_msg_h
# include "rt_qcom_msg.h"
#endif

#ifndef rt_qdb_msg_h
# include "rt_qdb_msg.h"
#endif

#ifndef pwr_h
# include "pwr.h"
#endif

#ifndef co_cdh_h
#include "co_cdh.h"
#endif

#ifndef co_time_h
#include "co_time.h"
#endif

#ifndef rt_sect_h
#include "rt_sect.h"
#endif

#ifndef rt_pool_h
#include "rt_pool.h"
#endif

#ifndef rt_hash_h
#include "rt_hash.h"
#endif

#ifndef rt_qcom_h
#include "rt_qcom.h"
#endif

#ifndef rt_inet_h
#include "rt_inet.h"
#endif

#ifndef rt_sync_h
#include "rt_sync.h"
#endif

/* Version of the runtime database datastructures. Changing this
   makes qcom_Init bugcheck if the structure versions
   does not match!  */

#define	qdb_cVersion		1	    
#define qdb_cIloopBack		(1<<31 | 0)
#define qdb_cIimport		(1<<31 | 1)
#define qdb_cIexport		(1<<31 | 2)
#define qdb_cImonitor		(1<<31 | 3)


static const qcom_sQid qdb_cQloopBack	= {qdb_cIloopBack, 0};
static const qcom_sQid qdb_cQimport	= {qdb_cIimport, 0};
static const qcom_sQid qdb_cQexport	= {qdb_cIexport, 0};
static const qcom_sQid qdb_cQmonitor	= {qdb_cImonitor, 0};

#if defined OS_VMS || defined OS_ELN
#  define	qdb_cNameDatabase	"pwr_qdb"

#  define	qdb_cNamePool		"pwr_qpool"
#  define	qdb_cNameDbLock		"pwr_qdb_lock"
#elif defined OS_LYNX
#  define	qdb_cNameDatabase	"/pwr_qdb"

#  define	qdb_cNamePool		"/pwr_qpool"
#  define	qdb_cNameDbLock		"/pwr_qdb_lock"

#  define	qdb_cSigMsg		SIGRTMIN
#elif defined OS_LINUX
#  define	qdb_cNameDatabase	"/tmp/pwr_qdb"

#  define	qdb_cNamePool		"/tmp/pwr_qpool"
#  define	qdb_cNameDbLock		"/tmp/pwr_qdb_lock"

#  define	qdb_cSigMsg		SIGRTMIN
#endif

#define qdb_cMin_nodes		10
#define qdb_cMin_queues		50
#define qdb_cMin_appls		20
#define qdb_cMin_sbufs		200
#define qdb_cSize_sbuf		500
#define qdb_cMin_mbufs		500
#define qdb_cSize_mbuf		1000
#define qdb_cMin_lbufs		10
#define qdb_cSize_lbuf		30000
#define qdb_cMin_pool_isize	600000
#define qdb_cMaxSegSize		1456

#define qdb_cQix_ReservedMin	100
#define qdb_cQix_ReservedMax	199

/* General database lock to keep consistency.
 
   After a 'qdb_Lock', the database is guaranteed to be locked, regardless
   of the previous locking state. After a 'qdb_Unlock' the database is
   unlocked, also regardless of the previous state!  */

#define	qdb_LockOwned (qdb->g->lock_owner == qdb->my_pid)

#define	qdb_LockGlobal do {\
  if (qdb_LockOwned) errh_Bugcheck(QDB__LOCKCHECK, "qdb_Lock was taken");\
  sect_Lock(NULL, &qdb->lock, &qdb->g->lock);\
  qdb->g->lock_owner = qdb->my_pid;\
  qdb->lock_count++;\
} while (0)

#define qdb_UnlockGlobal do {\
  if (!qdb_LockOwned) errh_Bugcheck(QDB__LOCKCHECK, "qdb_Lock was not taken");\
  qdb->g->lock_owner = 0;\
  sect_Unlock(NULL, &qdb->lock, &qdb->g->lock);\
} while (0)

#define	qdb_LockLocal	  sync_MutexLock(&qdb->thread_lock.mutex)
#define	qdb_UnlockLocal	  sync_MutexUnlock(&qdb->thread_lock.mutex)

#define	qdb_Lock   do {qdb_LockLocal; qdb_LockGlobal;} while (0)
#define	qdb_Unlock do {qdb_UnlockGlobal; qdb_UnlockLocal;} while (0)

#define qdb_ScopeLock	qdb_Lock;do
#define qdb_ScopeUnlock	while(0);qdb_Unlock

#define	qdb_AssumeLocked do {if (!qdb_LockOwned) errh_Bugcheck(QDB__LOCKCHECK, "qdb_AssumeLocked");} while (0)
#define	qdb_AssumeUnlocked do {if (qdb_LockOwned) errh_Bugcheck(QDB__LOCKCHECK, "qdb_AssumeUnlocked");} while (0)

#define qdb_cCookie 550715339

typedef struct {
  unsigned int	qix : 32;
  pwr_Endian_4(
    pwr_Bits(nid_0, 8),
    pwr_Bits(nid_1, 8),
    pwr_Bits(nid_2, 8),
    pwr_Bits(nid_3, 8)
  )
} qdb_mQid;

typedef union {
  qcom_sQid		pwr;
  qdb_mQid		q;
} qdb_uQid;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( failIfAdded	, 1),

    pwr_Bits( fill		, 31),,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define qdb_mAdd__		0
#define qdb_mAdd_failIfAdded	pwr_Bit(0)
#define qdb_mAdd_		(~qdb_mAdd__)

} qdb_mAdd;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( multipleGet	, 1),

    pwr_Bits( fill		, 31),,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
  ) b;

#define qdb_mGet__		0
#define qdb_mGet_multipleGet	pwr_Bit(0)
#define qdb_mGet_		(~qdb_mGet__)

} qdb_mGet;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( ignoreStall	, 1),
    pwr_Bits( fill_0		, 7),,,,,,,

    pwr_Bits( fill_1		, 8),,,,,,,,
    pwr_Bits( fill_2		, 8),,,,,,,,
    pwr_Bits( fill_3		, 8),,,,,,,
  ) b;

#define qdb_mLocal__		0
#define qdb_mLocal_ignoreStall	pwr_Bit(0)
#define qdb_mLocal_		(~qdb_mLocal__)

} qdb_mLocal;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( initiated	, 1),
    pwr_Bits( connected	, 1),
    pwr_Bits( active	, 1),
    pwr_Bits( fill_0	, 5),,,,,

    pwr_Bits( fill_1	, 8),,,,,,,,
    pwr_Bits( fill_2	, 8),,,,,,,,
    pwr_Bits( fill_3	, 8),,,,,,,
  ) b;

#define qdb_mNode__		0
#define qdb_mNode_initiated	pwr_Bit(0)
#define qdb_mNode_connected	pwr_Bit(1)
#define qdb_mNode_active	pwr_Bit(2)

#define qdb_mNode_up		(qdb_mNode_connected|qdb_mNode_active)
#define qdb_mNode_down		(~qdb_mNode_up)
#define qdb_mNode_		(~qdb_mNode__)

} qdb_mNode;

typedef enum {
  qdb_eBuffer__	= 0,
  qdb_eBuffer_base,
  qdb_eBuffer_segment,
  qdb_eBuffer_reference,
  qdb_eBuffer_
} qdb_eBuffer;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( source	, 1),
    pwr_Bits( target	, 1),
    pwr_Bits( broadcast	, 1),
    pwr_Bits( segmented	, 1),
    pwr_Bits( remote	, 1),
    pwr_Bits( imported	, 1),
    pwr_Bits( fill_0	, 2),,

    pwr_Bits( request	, 1),
    pwr_Bits( reply	, 1),
    pwr_Bits( fill_1	, 6),,,,,,

    pwr_Bits( fill_2	, 8),,,,,,,,
    pwr_Bits( fill_3	, 8),,,,,,,
  ) b;

#define qdb_mBuffer__		0
#define qdb_mBuffer_source	pwr_Bit(0)
#define qdb_mBuffer_target	pwr_Bit(1)
#define qdb_mBuffer_broadcast	pwr_Bit(2)
#define qdb_mBuffer_segmented	pwr_Bit(3)
#define qdb_mBuffer_remote	pwr_Bit(4)
#define qdb_mBuffer_imported	pwr_Bit(5)

#define qdb_mBuffer_request	pwr_Bit(8)
#define qdb_mBuffer_reply	pwr_Bit(9)

#define qdb_mBuffer_		(~qdb_mBuffer__)

#define qdb_mBuffer_maskExport	(qdb_mBuffer_request|qdb_mBuffer_reply|qdb_mBuffer_broadcast)

} qdb_mBuffer;

typedef enum {
  qdb_eQue__	= 0,
  qdb_eQue_private,
  qdb_eQue_forward,
  qdb_eQue_
} qdb_eQue;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( broadcast	, 1),
    pwr_Bits( system	, 1),
    pwr_Bits( event	, 1),
    pwr_Bits( fill_0	, 5),,,,,

    pwr_Bits( reply	, 1),
    pwr_Bits( fill_1	, 7),,,,,,,
    pwr_Bits( fill_2	, 8),,,,,,,,
    pwr_Bits( fill_3	, 8),,,,,,,
  ) b;

#define qdb_mQue__		0
#define qdb_mQue_broadcast	pwr_Bit(0)
#define qdb_mQue_system		pwr_Bit(1)
#define qdb_mQue_event		pwr_Bit(2)

#define qdb_mQue_reply		pwr_Bit(8)

#define qdb_mQue_		(~qdb_mQue__)

} qdb_mQue;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( events	, 1),
    pwr_Bits( states	, 1),
    pwr_Bits( linkmsg	, 1),
    pwr_Bits( fill_0	, 5),,,,,

    pwr_Bits( fill_1	, 8),,,,,,,,
    pwr_Bits( fill_2	, 8),,,,,,,,
    pwr_Bits( fill_3	, 8),,,,,,,
  ) b;

#define qdb_mLog__		0
#define qdb_mLog_events		pwr_Bit(0)
#define qdb_mLog_states		pwr_Bit(1)
#define qdb_mLog_linkmsg	pwr_Bit(2)

#define qdb_mLog_		(~qdb_mLog__)

} qdb_mLog;

typedef struct {
  pwr_tUInt32		idx;
  pwr_tUInt32		bytes;
  pwr_tUInt32		segs;
  pwr_tUInt32		bufs;
  pwr_tUInt32		error;
  pwr_tUInt32		timer;
  pwr_tUInt32	 	timer_max;
} qdb_sCount;

typedef struct {
  int			version		pwr_dPacked;	/* Qcom protocol version */
  pwr_tNodeId		nid		pwr_dPacked;	/* node index */
  int			birth		pwr_dPacked;
  char			name[80];    			/* node name  */
  struct sockaddr_in	sa		pwr_dPacked;
  qcom_tBus		bus		pwr_dPacked;	/* buss number */
  co_eOS		os		pwr_dPacked;	/* operating system */
  co_eHW		hw		pwr_dPacked;	/* hardware */
  co_eBO		bo		pwr_dPacked;	/* big/little endian */
  co_eFT		ft		pwr_dPacked;	/* float type */
} qdb_sLinkInfo;


typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( connected	, 1),
    pwr_Bits( active	, 1),
    pwr_Bits( fill_0	, 6),,,,,,

    pwr_Bits( fill_1	, 8),,,,,,,,
    pwr_Bits( fill_2	, 8),,,,,,,,
    pwr_Bits( fill_3	, 8),,,,,,,
  ) b;

#define qdb_mLink__		0

#define qdb_mLink_connected	pwr_Bit(0)
#define qdb_mLink_active	pwr_Bit(1)

#define qdb_mLink_		(~qdb_mLink__)

} qdb_mLink;

typedef struct {
  int			seq		pwr_dPacked;	/* Sequence number. */
  int			ts		pwr_dPacked;	/* Time stamp. */
} qdb_sAck;

typedef struct {
  qdb_mLink		flags;
  int			win_count;
  int			win_max;
  int			rtt_rxmax;
  int			rtt_rxmin;
  float			rtt_rtt;
  float			rtt_srtt;
  float			rtt_var;
  float			rtt_rto;
  qdb_sAck		lack;
  qdb_sAck		rack;
  int			seq;
  pwr_tBoolean		pending_rack;
  time_tClock		rack_tmo;
  qcom_tBus		bus;
  struct sockaddr_in	sa;
  pwr_tDeltaTime	timer;
  int			err_red;
  int			err_seq;
} qdb_sLink;

typedef char qdb_tQname[32];

typedef struct {
  pwr_tBoolean	isThreaded;
  thread_sCond	cond;
  thread_sMutex	mutex;
  int		(*cond_signal)(thread_sCond*);
  int		(*cond_wait)(thread_sCond*, thread_sMutex*);
} qdb_sLock;

#if defined OS_VMS

  typedef struct {
    struct {
      unsigned short	condition;
      unsigned short	reserved;
      unsigned int	id;
    } enq;
    qdb_tQname		name;
    int			nlen;
    int			waiting;
  } qdb_sQlock;

#elif defined OS_ELN

  typedef struct {
    AREA		id;
    qdb_tQname		name;
    int			nlen;
    int			waiting;
  } qdb_sQlock;

#elif defined OS_LYNX || defined OS_LINUX

  typedef struct {
    int			pid;
    int			waiting;
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
  } qdb_sQlock;

#endif

typedef struct {
  qcom_sAid		aid;
  pid_t			pid;

  pool_sQlink		aix_htl;	/* Link in aix-to-appl hash table.  */
  pool_sQlink		pid_htl;	/* Link in aix-to-appl hash table.  */
  pool_sQlink		appl_ll;	/* Link in application list.  */
  
  qcom_tRid		rid;
  char			name[80];

  unsigned long		ef;

  pool_sQlink		que_lh;		/* Head of list of queues owned by this application.  */

  pool_sQlink		out_lh;		/* que of buffer to be sent */

  unsigned int		call_count;
  unsigned int		alloc_count;
  unsigned int		alloc_quota;

  unsigned int		free_count;
  unsigned int		get_count;
  unsigned int		put_count;
  unsigned int		request_count;
  unsigned int		reply_count;

  /* Queue template.  */
  unsigned int		in_qouta;
} qdb_sAppl;

typedef struct {
  qcom_sAid		sender		pwr_dPacked;
  pid_t			pid		pwr_dPacked;
  qcom_sQid		receiver	pwr_dPacked;
  qcom_sQid		reply		pwr_dPacked;
  qcom_sType		type		pwr_dPacked;
  qcom_tRid		rid		pwr_dPacked;
  unsigned int		size		pwr_dPacked;
  int			tmo		pwr_dPacked;
  pwr_tStatus		status		pwr_dPacked;
  qdb_mBuffer		flags		pwr_dPacked;
} qdb_sInfo;

typedef struct {
  qdb_mBuffer		flags;
  qdb_eBuffer		type;
  pool_sQlink		ll;		/* link in list of buffers */
} qdb_sCbuffer;

typedef struct {
  qdb_sCbuffer		c;		/* common for all buffers */
  pool_sQlink		ref_lh;		/* head of list of reference buffers */
  pool_sQlink		seg_lh;		/* head of list of segments */
  unsigned int		size;
  unsigned int		cookie;
  unsigned int		seg_size;
  unsigned int          msg_id;
  unsigned int          prio;
  pool_tRef		noderef;	/* pool reference to the node block of an external node */
  qdb_sInfo		info;
} qdb_sBbuffer;

typedef struct {
  qdb_sCbuffer		c;		/* common for all buffers */
  pool_sQlink		ref_ll;		/* link in list of reference buffers */
  pool_tRef		src;		/* pool reference of source buffer */
} qdb_sRbuffer;

typedef struct {
  qdb_sCbuffer		c;		/* common for all buffers */
  pool_sQlink		seg_ll;		/* link in list of segments  */
  pool_tRef		base;		/* pool reference of base buffer */
  unsigned int		size;		/* size of this segment */
} qdb_sSbuffer;

typedef union {
  qdb_sCbuffer		c;
  qdb_sBbuffer		b;
  qdb_sRbuffer		r;
  qdb_sSbuffer		s;
} qdb_sBuffer;

typedef struct {
  qcom_tQix		srcQix;		/* source, forwarding queue */
  pool_tRef		srcQ;
  pool_sQlink		src_ll;

  qcom_tQix		tgtQix;		/* target, queue to forward to */
  pool_tRef		tgtQ;
  pool_sQlink		tgt_ll;
} qdb_sQbond;

typedef struct {
  qcom_tQix		qix;
  pool_sQlink		qix_htl;
  qcom_sAid		aid;
  pool_tRef		aref;
  qdb_eQue		type;
  qdb_mQue		flags;
  qdb_sQlock		lock;

  qcom_tRid		rid;
  char			name[80];

  pool_sQlink		que_ll;		/* queues owned by one application */
  pool_sQlink		src_lh;		/* queues to forward to */
  pool_sQlink		tgt_lh;		/* queues to be forwarded from */

  int			mask;
  int			or_event;
  pool_sQlink		eve_lh;		/* event queue list header  */
  pool_sQlink		eve_ll;		/* event queue list link  */

  pool_sQlink		in_lh;		/* buffers to be read */
  unsigned int		in_lc;
  unsigned int		in_quota;
  pool_sQlink		rep_lh;		/* reply list */
  pool_sQlink		read_lh;	/* buffers already read, but not freed */

  unsigned int		get_count;
  unsigned int		request_count;
} qdb_sQue;

typedef struct {
  pwr_tNodeId		nid;		/* node identity */
  pwr_tBoolean		initiated;	/* are values valid */
  pwr_tUInt32		birth;		/* current incarnation identity */
  pool_sQlink		nid_htl;	/* link in nid-to-node hash table */
  pool_sQlink		node_ll;	/* link in list of all links */
  pool_sQlink		own_lh;		/*  */
  pwr_tUInt32		own_lc;		/* number of owned volumes */
  char			name[80];	/* ascii name of node (nul-terminated)
					   zero-length means empty slot */
  int			version;	/* Qcom protocol version */

  co_eOS		os;		/* operating system */
  co_eHW		hw;		/* hardware */
  co_eBO                bo;             /* byte order */
  co_eFT                ft;             /* float type */

  qdb_sLink		link;		/* Qmon link information. */
  pwr_tBoolean		up;		/* communication is up/down */
  qdb_mNode		flags;
  pwr_tUInt32		upcnt;		/* # of times up */
  pwr_tTime		timeup;		/* Most recent time link came up */
  pwr_tTime		timedown;	/* Most recent time link went down */

  qdb_sCount		get;
  qdb_sCount		put;
  pwr_tUInt32		thrown_bufs;
  pwr_tUInt32		thrown_segs;
  pwr_tUInt32		lost_segs;
  
  /* broadcast */

  pool_sQlink		bcb_lh;		/* broadcast buffer list header */
  qcom_tRid		bc_snd_id;
  qcom_tRid		bc_rcv_id;

  /* The following attributes may only be used by rt_qimp.  */

  qdb_sBuffer		*bp;

  pwr_tBoolean		in_arp;
  struct arpreq		arp;

  struct sockaddr_in	sa;

  void			*mon;		/* Monitor private data. */
} qdb_sNode;

typedef struct {
  pwr_tNodeId		nid;		/* Node index for this node */
  qcom_tBus		bus;
  pwr_tUInt32		nodes;		/* Number of  */
  pwr_tUInt32		queues;		/* Number of  */
  pwr_tUInt32		appls;		/* Number of  */
  pwr_tUInt32		sbufs;
  pwr_tUInt32		mbufs;
  pwr_tUInt32		lbufs;
  pwr_tUInt32		size_sbuf;
  pwr_tUInt32		size_mbuf;
  pwr_tUInt32		size_lbuf;
  pwr_tUInt32		pool_isize;	/* GDHpool initial size */
  pwr_tUInt32		pool_esize;	/* GDHpool extendsize */
} qdb_sInit;

/* The global database header.  */

typedef struct {
  sect_sMutex		lock;		/* Database lock */
  qcom_tAix		lock_owner;	/* Owner of db lock */
  pwr_tUInt32		version;	/* Qdb structure revision.  */

  qdb_sInit		eval_init;

  hash_sGtable		nid_ht;		/* node id hash table.  */
  hash_sGtable		qix_ht;		/* queue ix hash table.  */
  hash_sGtable		aix_ht;		/* application ix hash table.  */
  hash_sGtable		pid_ht;		/* application process id hash table.  */

  unsigned int		call_count;

  qcom_tRid		rid;		/* Request generation number */

  qcom_sAid		aid;		/* next free application id */
  qcom_sQid		qid;		/* Next free queue id.  */
  pwr_tNodeId		nid;		/* Nid of this node. */
  qcom_tBus		bus;

  pool_sQlink		appl_lh;	/* Head of list of applications.  */
  pool_sQlink		node_lh;	/* Head of list of nodes.  */

  pool_sQlink		small_lh;	/*  */
  unsigned int		small_lc;
  pool_sQlink		medium_lh;	/*  */
  unsigned int		medium_lc;
  pool_sQlink		big_lh;		/*  */
  unsigned int		big_lc;

  qdb_mLog		log;		/* Mask that controls logging */

  qcom_sQid		qid_import;
  qcom_sQid		qid_export;
  qcom_sQid		qid_monitor;

  pwr_tBoolean		up;		/* we are up */
  int			tmo_export;
  int			qmon_sleep;
  int			qmon_count;
} qdb_sGlobal;


/* Job local QCOM data, pointed to by gQcom, the root of all tables.  */

typedef struct {
  qdb_mLocal		flags;
  qdb_sLock		thread_lock;
  sect_sHead		sect;		/* section header for global database */
  sect_sHead		lock;		/*   */
  pool_sHead		pool;		/* pool for database */
  hash_sTable		nid_ht;		/* node id hash table.  */
  hash_sTable		qix_ht;		/* queue idx hash table.  */
  hash_sTable		aix_ht;		/* application idx hash table.  */
  hash_sTable		pid_ht;		/* application process id hash table.  */

  qdb_sGlobal		*g;		/* global database root */

  qdb_sNode		*my_node;	/* pointer to local node */
  qdb_sNode		*no_node;	/* pointer to unknown node */

  pwr_tUInt32		lock_count;	/* # of times this job locked the DB */

  pid_t			my_pid;		/* process id of this job */
  qcom_tAix		my_aix;
  qcom_sAid		my_aid;
  pwr_tNodeId		my_nid;

  qdb_sAppl		*ap;		/* my application */
  qdb_sQue		*exportque;	/* the export que */
} qdb_sLocal;

/* The root of all data, the only `global' variable...  */

#if defined (OS_ELN)
  extern noshare qdb_sLocal	*qdb;
#else
  extern qdb_sLocal	*qdb;		/* root of all data in database */
#endif

qdb_sAppl*	qdb_AddAppl	(pwr_tStatus*, pwr_tBoolean);
qdb_sNode*	qdb_AddNode	(pwr_tStatus*, pwr_tUInt32, pwr_tBitMask);
qdb_sQbond*	qdb_AddBond	(pwr_tStatus*, qdb_sQue*, qdb_sQue*);
qdb_sQue*	qdb_AddQue	(pwr_tStatus*, qcom_tQix);
qdb_sBuffer*	qdb_Alloc	(pwr_tStatus*, qdb_eBuffer, unsigned int);
void		qdb_ApplEvent	(pwr_tStatus*, qdb_sAppl*, qcom_eStype);
qdb_sQue*	qdb_AttachQue	(pwr_tStatus*, qdb_sQue*, qdb_sAppl*);
qdb_sBuffer*	qdb_CopyBuffer	(pwr_tStatus*, qdb_sBuffer*);
void*		qdb_CopyBufferData (pwr_tStatus*, qdb_sBuffer*, qcom_sGet*);
qdb_sBuffer*	qdb_DetachBuffer (pwr_tStatus*, qdb_sBuffer*);
qdb_sLocal*	qdb_CreateDb	(pwr_tStatus*, qdb_sInit*);
void		qdb_UnlinkDb	();
qdb_sQlock*	qdb_CreateQlock (pwr_tStatus*, qdb_sQue*);
void		qdb_DeleteQlock (pwr_tStatus*, qdb_sQue*);
qdb_sBuffer*	qdb_Deque	(pwr_tStatus*, qdb_sQue*, int);
pwr_tBoolean	qdb_Enque	(pwr_tStatus*, qdb_sBuffer*, qdb_sQue*);
void		qdb_Eput	(pwr_tStatus*, qdb_sQue*);
pwr_tBoolean	qdb_Signal	(pwr_tStatus*, qdb_sQue*);
int		qdb_Wait	(pwr_tStatus*, qdb_sQue*, qdb_sQue*, int);
void		qdb_Free	(pwr_tStatus*, qdb_sBuffer*);
qdb_sBuffer *	qdb_Get		(pwr_tStatus*, qdb_sQue*, int, qcom_sGet*, pwr_tBitMask);
qdb_sQbond *	qdb_GetBond	(pwr_tStatus*, qdb_sQue*, qdb_sQue*);
void		qdb_GetInfo	(qcom_sGet*, qdb_sBuffer*);
qdb_sLocal *	qdb_MapDb	(pwr_tStatus*);
void		qdb_NetEvent	(pwr_tStatus*, qdb_sNode*, qcom_eStype);
void		qdb_NodeInfo	(pwr_tStatus*, qcom_sNode*, qdb_sNode*);
void		qdb_Platform	(qdb_sNode*);
void		qdb_Put		(pwr_tStatus*, qdb_sBuffer*, qdb_sQue*);
void		qdb_RemoveAppl	(pwr_tStatus*, qdb_sAppl*);
void		qdb_PutInfo	(qdb_sBuffer*, qcom_sPut *, const qcom_sQid*, qcom_tRid);
pwr_tBoolean	qdb_RemoveQue	(pwr_tStatus*, qdb_sQue*);
qdb_sQue *	qdb_Que		(pwr_tStatus*, const qcom_sQid*, qdb_sNode**);
void *		qdb_Request	(pwr_tStatus*, qdb_sBuffer*, qdb_sQue*, qdb_sQue*, int, qcom_sGet*, pwr_tBitMask);

void		qdb_DumpPool ();
#endif
