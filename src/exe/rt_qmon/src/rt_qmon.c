/* 
 * Proview   $Id: rt_qmon.c,v 1.11 2006-09-05 12:11:47 claes Exp $
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

/* rt_qmon.c -- Qcom monitor. */

#if defined OS_ELN
# include $vaxelnc
# include errno
#else
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <errno.h>
# include <unistd.h>
# include <sched.h>
# if defined OS_LINUX
#   include <sys/uio.h>
#   include <sys/socket.h>
# else
#   include <uio.h>
#   include <socket.h>
#endif
#endif

#if defined OS_VMS
# include <descrip.h>        /* VMS descriptor stuff */
# include <ucx$inetdef.h>    /* UCX network definitions */
# include <starlet.h>        /* Sys ser calls */
# include <iodef.h>          /* I/O FUNCTION CODE DEFS */
# include <ssdef.h>
#endif
#include "pwr.h"
#include "co_time.h"
#include "co_tree.h"
#include "co_errno.h"
#include "rt_errh.h"
#include "rt_pool.h"
#include "rt_qdb.h"
#include "rt_inet.h"
#include "rt_net.h"
#include "rt_qcom.h"
#include "rt_qcom_msg.h"
#include "rt_lst.h"
#include "rt_que.h"
#include "rt_thread.h"
#include "rt_thread_msg.h"
#include "rt_pwr_msg.h"
#include "rt_ini_event.h"

#if 1
# define MAX_SEGSIZE	(8192 - sizeof(sHead))
#else
# define MAX_SEGSIZE	(1472 - sizeof(sHead))
#endif
#define RTT_RXMIN	500   /* ms */
#define RTT_RXMAX	10000 /* ms */
#define RACK_TMO	1

typedef enum {
  eAction__ = 0,
  eAction_export,
  eAction_import,
  eAction_tmo,
  eAction_
} eAction;

typedef union {
  pwr_tBitMask m;
  pwr_32Bits (
    pwr_Bits( version	, 3),,,
    pwr_Bits( first	, 1),
    pwr_Bits( middle	, 1),
    pwr_Bits( last	, 1),
    pwr_Bits( fill_0	, 2),,

    pwr_Bits( bcast	, 1),
    pwr_Bits( resent	, 1),
    pwr_Bits( fill_1	, 6),,,,,,
    pwr_Bits( fill_2	, 8),,,,,,,,
    pwr_Bits( event	, 8),,,,,,,
  ) b;

#define mSeg__		0
#define mSeg_first	pwr_Bit(3)
#define mSeg_middle	pwr_Bit(4)
#define mSeg_last	pwr_Bit(5)
#define mSeg_single	(mSeg_first|mSeg_middle|mSeg_last)
#define mSeg_sequence	(mSeg_first|mSeg_middle|mSeg_last)
#define mSeg_bcast	pwr_Bit(8)

#define mSeg_		(~mSeg__)

} mSeg;

typedef struct sLink sLink;
typedef struct sHead sHead;
typedef struct sCseg sCseg;
typedef struct sEseg sEseg;
typedef struct sIseg sIseg;
  
struct sHead {
  pwr_tNodeId		nid		pwr_dPacked;	/* Node identitiy. */
  int			birth		pwr_dPacked;	/* Time when link was born. */
  mSeg			flags		pwr_dPacked;
  qdb_sAck		lack		pwr_dPacked;
  qdb_sAck		rack		pwr_dPacked;
};

struct sCseg {
  eAction		action;
  lst_sEntry		le;
};

struct sEseg {
  sCseg			c;
  sHead			head;
  unsigned int		size;
  sLink	 		*lp;
  lst_sEntry		le_bcast;
  lst_sEntry		le_seg;
  unsigned int          id;
  qdb_sBuffer		*bp;
  char			*p;
  int			bytes;
  time_tClock		tmo;
};

struct sIseg {
  sCseg			c;
  sHead			head;
  unsigned int		size;
  int			ts_recv;
  sLink			*lp;
  pwr_tStatus		sts;
  char			buff[MAX_SEGSIZE];
};

typedef union {
  eAction		action;
  sCseg			c;
  sEseg			e;
  sIseg			i;
} uSeg;

struct sLink {
  tree_sNode		tree;
  pwr_tNodeId		nid;
  lst_sEntry		lh_send;
  lst_sEntry		lh_win;
  que_sQue		q_in;
  qdb_sNode		*np;
  thread_s		thread;
  qdb_sBuffer		*bp;
  char			*p;
  sIseg			tmo;
};


#if defined OS_VMS
  typedef struct {
    short		sts;
    short		tsize;
    void*		base;
  } sIosb;

  typedef struct {
    sIosb		*iosb;
    thread_sCond	*cp;
    int			lock_count;
    int			*lock_count_p;
  } sReadAst;

  typedef struct {
    short sin_family;
    short sin_port;
    int   sin_addr;
    char  blkb[8];
  } sSockaddr;

  typedef struct {
    sHead		head;
    qdb_sInfo		info;
    sSockaddr		sa;
     struct {
      int		iov_len;
      char*		iov_base;
    } iov[3];
    struct {
      int		namelen;
      sSockaddr*	name;
      int*		retlen;
    } rsa_d;
   int			sa_retlen;
    struct {
      int		namelen;
      sSockaddr*	name;
    } lsa_d;
    struct {
      int		len;
      void*		base;
    } desc;
  } sMsg;
#else
  typedef struct {
    sHead			head;
    qdb_sInfo			info;
    struct sockaddr_in		sa;
    struct iovec		iov[3];
    struct msghdr		msg;
  } sMsg;
#endif  
 
typedef enum {
  eEvent__ = 0 ,
  eEvent_connect,
  eEvent_down,
  eEvent_user,
  eEvent_ack,
  eEvent_
} eEvent;


struct {
  sHead			head;
  struct sockaddr_in	sa;
#if defined OS_VMS
  short			sock;
  short			fill;
#else
  int			sock;
#endif
  struct {
    lst_sEntry		lh;
    thread_sMutex	mutex;
    int			count;
  } eseg;
  struct {
    lst_sEntry		lh;
    thread_sMutex	mutex;
    int			count;
  } iseg;
  struct {
    int			count;
    tree_sTable		*table;
    thread_sMutex	mutex;
  } links;
  thread_sMutex		bcast;
  thread_sMutex		send_mutex;
  struct {
    thread_s		thread;
    thread_sMutex	mutex;
    thread_sCond	cond;
  } import;
  struct {
    thread_s		thread;
  } export;
  qdb_sLinkInfo		link_info;
#if defined OS_VMS
  int			send_ef;
  int			import_ef;
#endif
  pwr_tStatus		sts;
} l;

static pwr_tStatus qcom_sts = PWR__SRVSTARTUP;

static void		cancel_links();
static pwr_tBoolean     clean_insert (lst_sEntry*, sEseg*, pwr_tBoolean);
static sEseg*		create_connect (sLink*);
static void		create_links();
static void		decode_head(sHead*, sHead*);
static void		decode_info (qdb_sInfo*);
static void		encode_head(sHead*, sHead*);
static void		encode_info (qdb_sInfo*, qdb_sInfo*);
static sEseg*		eseg_alloc(thread_sMutex*);
static sEseg*		eseg_build(qdb_sBuffer*);
static void		eseg_free(sEseg*);
static char*		event_string (eEvent);
static pwr_tBoolean	expired(time_tClock);
static void*		export_thread();
static sLink*		get_link(pwr_tNodeId, sMsg*);
static time_tClock	get_tmo(sLink*, pwr_tBoolean);
static void*		import_thread();
static void		ini_link_info (qdb_sLinkInfo*);
static sIseg*		iseg_alloc();
static void		iseg_free(sIseg*);
static void		iseg_import(sLink*, sIseg*);
static void		lack(sLink*, sIseg*);
static void		link_active(sLink*);
static void		link_connect(sLink*, sIseg*);
static void		link_disconnect(sLink*);
static void		link_import(sLink*, sIseg*);
static void		link_redisconnect(sLink*);
static void		link_send(sLink*);
static void		link_stalled(sLink*);
static void*		link_thread(sLink*);
static pwr_tDeltaTime*	link_tmo (sLink*);
static sLink*		new_link(pwr_tNodeId, sMsg*);
static void		open_socket();
static sEseg*		pending_send (sLink*);
static void		send_ack (sLink*);
static void		set_link_info (sLink*, qdb_sLinkInfo*);
static void		set_rack(sLink*, sIseg*);
static void		set_recvmsg(sIseg*, sMsg*);
static void		seg_send (sLink*, sEseg*);
static void		set_sendmsg(sLink*, sEseg*, sMsg*);
static void		start_rto(sLink*);
static void		update_rtt(sLink*, sIseg*);
static void		window_insert (sLink*, sEseg*);
static void		window_remove (sLink*, sEseg*);
static sEseg*		window_tmo (sLink*);
static void		check_link_status ();
static void		set_status (pwr_tStatus);


int
main (int argc, char *argv[])
{
  pwr_tStatus	sts;
  qcom_sQid	qid = qdb_cQexport;
  qcom_sQid	neth_qid;
  qcom_sQid	my_q = qcom_cNQid;
  pid_t         pid;
  int           count = 0;

  /* Vänta en stund ... */
//  sleep(5);

  /* Wait for scheduler to be set */
  
  pid = getpid();

  while ((sched_getscheduler(pid) == SCHED_OTHER) &&
         (count < 5)) {
    sleep(1);
    count++;
  }
  
  errh_Init("pwr_qmon", errh_eAnix_qmon);
  errh_SetStatus( PWR__SRVSTARTUP);

  if (!qcom_Init(&sts, NULL, "pwr_qmon")) {
    errh_Fatal("qcom_Init, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

#if defined OS_VMS
  qdb->thread_lock.isThreaded = 1;
  qdb->thread_lock.cond_signal = thread_CondSignal;
  qdb->thread_lock.cond_wait = thread_CondWait;
#endif

  if (!qcom_CreateQ(&sts, &my_q, NULL, "events")) {
    errh_Fatal("qcom_CreateQ, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }

  if (!qcom_AttachQ(&sts, &qid)) {
    errh_Fatal("qcom_AttachQ, %m", sts);
    errh_SetStatus( PWR__SRVTERM);
    exit(sts);
  }
  l.head.nid = qdb->my_node->nid;
  l.head.birth = qdb->my_node->birth = time_Clock(NULL, NULL);
  ini_link_info(&l.link_info);

  neth_qid.qix = net_cProcHandler; 
  neth_qid.nid = qcom_cNNid;

  for ( ; ; ) {
    qdb_sQue *qp = NULL;

    qdb_ScopeLock {
      qp = qdb_Que(NULL, &neth_qid, NULL);
    } qdb_ScopeUnlock;

    if (qp != NULL)
      break;

    errh_Info("Waiting for Nethandler to start...");
    sleep(1);    
  }

  do {
#if defined OS_VMS
    sts = lib$get_ef(&l.send_ef);
    pwr_Assert(ODD(sts));
    sts = lib$get_ef(&l.import_ef);
    pwr_Assert(ODD(sts));
#endif
    lst_Init(NULL, &l.eseg.lh, NULL);
    lst_Init(NULL, &l.iseg.lh, NULL);
    open_socket();
    sts = thread_MutexInit(&l.eseg.mutex);
    sts = thread_MutexInit(&l.iseg.mutex);
    sts = thread_MutexInit(&l.links.mutex);
    sts = thread_MutexInit(&l.bcast);
    sts = thread_MutexInit(&l.send_mutex);
    sts = thread_MutexInit(&l.import.mutex);
    sts = thread_CondInit(&l.import.cond);

    l.links.table = tree_CreateTable(&sts, sizeof(pwr_tNodeId), offsetof(sLink, nid),
      sizeof(sLink), 100, tree_Comp_nid);

    create_links();

    thread_Create(&l.import.thread, "import", import_thread, NULL);
    thread_Create(&l.export.thread, "export", export_thread, NULL);

  } while (0);

  check_link_status();
  set_status( PWR__SRUN);

  qcom_WaitAnd(&sts, &my_q, &qcom_cQini, ini_mEvent_terminate, qcom_cTmoEternal);
    
//  sts = thread_Wait(NULL);	/* Wait forever */

  set_status( PWR__SRVTERM);
  errh_Info("pwr_qmon says: I will soon die, %m", sts);
  cancel_links();
  thread_Cancel(&l.import.thread);
  thread_Cancel(&l.export.thread);

  qcom_Exit(&sts);
  errh_Info("pwr_qmon says: I will now die! Goodbye cruel world!");
  exit(QCOM__SUCCESS);
}

static void
cancel_links ()
{
  pwr_tStatus sts;
  sLink *lp;

  for (lp = tree_Minimum(&sts, l.links.table); lp != NULL; lp = tree_Successor(&sts, l.links.table, lp))
    thread_Cancel(&lp->thread);

}

/* Free unsent segments from que that has not been
   sent which will be replaced by new segment  */

static pwr_tBoolean
clean_insert (
  lst_sEntry *le,
  sEseg *esp, 
  pwr_tBoolean pending
)
{
  lst_sEntry *se, *li;
  sEseg *sp, *nsp;
  pwr_tBoolean ret_pend = FALSE;
  pwr_tBoolean first = FALSE;

  li = le;
  if (esp->bp != NULL) {
    if (esp->bp->b.msg_id > 0) {
    
      if ((!pending) && (esp->head.flags.b.first)) {
  
        for (sp = lst_Succ(NULL, le, &se); se != le; sp = nsp) {
	  li = se;
          nsp = lst_Succ(NULL, se, &se);

          if (sp->bp != NULL) {      
            if (sp->bp->b.msg_id == esp->bp->b.msg_id) {
              if (sp->head.flags.b.first)
                first = TRUE;
	      if (first) {
	        lst_Remove(NULL, &sp->c.le);
	        eseg_free(sp);
		li = se;
	      } else {
	        break;
	      }
            } else if (first) {
              break;
	    }
          } else if (first) {
	    break;
	  }
        }
	
        if (!esp->head.flags.b.last && first) ret_pend = TRUE;
	if (!first) li = le;
      }
      else if (pending) {

        for (sp = lst_Succ(NULL, le, &se); se != le; sp = nsp) {
          li = se;
          nsp = lst_Succ(NULL, se, &se);

          if (sp->bp != NULL) {      
            if ((sp->bp->b.msg_id == esp->bp->b.msg_id) &&
	        (sp->bp == esp->bp)) {
              first = TRUE;
	      li = se;
            } else if (first) {
              break;
	    }
          } else if (first) {
	    break;
	  }
        }
	
	if (!first) li = le;
	ret_pend = !esp->head.flags.b.last;
      }
    }
  }
  
  /* Insert new item */
  
  lst_InsertPred(NULL, li, &esp->c.le, esp);
  
  return ret_pend;

}

static sEseg *
create_connect (
  sLink *lp
)
{
  sEseg *sp;

  sp = eseg_alloc(&l.eseg.mutex);
  sp->head.flags.b.event = eEvent_connect;
  sp->lp = lp;
  sp->c.action = eAction_export;

  return sp;
}

static void
create_links ()
{
  pwr_tStatus   sts;
  pool_sQlink	*nl;
  qdb_sNode	*np;
  sLink *lp;

  qdb_ScopeLock {
    for (
      nl = pool_Qsucc(NULL, &qdb->pool, &qdb->g->node_lh);
      nl != &qdb->g->node_lh;
      nl = pool_Qsucc(NULL, &qdb->pool, nl)
    ) {
      np = pool_Qitem(nl, qdb_sNode, node_ll);
      if (np == qdb->my_node) continue;
      if (np == qdb->no_node) continue;
      tree_Insert(&sts, l.links.table, &np->nid);
    }
  } qdb_ScopeUnlock;

  for (lp = tree_Minimum(&sts, l.links.table); lp != NULL; lp = tree_Successor(&sts, l.links.table, lp))
    new_link(lp->nid, NULL);

}

static void
decode_head (
  sHead *tp,
  sHead *sp
)
{

  tp->nid	= ntohl(sp->nid);
  tp->birth	= ntohl(sp->birth);
  tp->flags.m	= ntohl(sp->flags.m);
  tp->lack.seq	= ntohl(sp->lack.seq);
  tp->lack.ts	= ntohl(sp->lack.ts);
  tp->rack.seq	= ntohl(sp->rack.seq);
  tp->rack.ts	= ntohl(sp->rack.ts);
}

static void
decode_info (
  qdb_sInfo	*ip
)
{

  ip->sender.aix   = ntohl(ip->sender.aix);
  ip->sender.nid   = ntohl(ip->sender.nid);
  ip->pid	   = ntohl(ip->pid);
  ip->receiver.qix = ntohl(ip->receiver.qix);
  ip->receiver.nid = ntohl(ip->receiver.nid);
  ip->reply.qix    = ntohl(ip->reply.qix);
  ip->reply.nid    = ntohl(ip->reply.nid);
  ip->type.b	   = ntohl(ip->type.b);
  ip->type.s	   = ntohl(ip->type.s);
  ip->rid	   = ntohl(ip->rid);
  ip->tmo	   = ntohl(ip->tmo);
  ip->size         = ntohl(ip->size);
  ip->flags.m	   = ntohl(ip->flags.m);
  ip->status	   = ntohl(ip->status);
}

static void
encode_head (
  sHead *tp,
  sHead *sp
)
{

  tp->nid	= htonl(sp->nid);
  tp->birth	= htonl(sp->birth);
  tp->flags.m	= htonl(sp->flags.m);
  tp->lack.seq	= htonl(sp->lack.seq);
  tp->lack.ts	= htonl(sp->lack.ts);
  tp->rack.seq	= htonl(sp->rack.seq);
  tp->rack.ts	= htonl(sp->rack.ts);
}

static void
encode_info (
  qdb_sInfo	*tp,
  qdb_sInfo	*sp
)
{

  tp->sender.aix   = htonl(sp->sender.aix);
  tp->sender.nid   = htonl(sp->sender.nid);
  tp->pid	   = htonl(sp->pid);
  tp->receiver.qix = htonl(sp->receiver.qix);
  tp->receiver.nid = htonl(sp->receiver.nid);
  tp->reply.qix    = htonl(sp->reply.qix);
  tp->reply.nid    = htonl(sp->reply.nid);
  tp->type.b	   = htonl(sp->type.b);
  tp->type.s	   = htonl(sp->type.s);
  tp->rid	   = htonl(sp->rid);
  tp->tmo	   = htonl(sp->tmo);
  tp->size         = htonl(sp->size);
  tp->flags.m	   = htonl(sp->flags.m);
  tp->status	   = htonl(sp->status);
}

static sEseg *
eseg_alloc (
  thread_sMutex *mp
)
{
  sEseg *sp;

  sp = lst_RemoveSucc(mp, &l.eseg.lh, NULL);
  if (sp == NULL) {
    sp = (sEseg *)calloc(1, sizeof(*sp));

    lst_Init(NULL, &sp->le_bcast, sp);
    lst_Init(NULL, &sp->le_seg, sp);
    lst_Init(NULL, &sp->c.le, sp);
  }

  return sp;
}

/* Build segments for a buffer.
   If buffer is to be broadcasted then allocate
   for all active links.  */

static sEseg *
eseg_build (
  qdb_sBuffer *bp
)
{
  pwr_tStatus sts;
  int size;
  sEseg *sp;
  sEseg *msp = NULL;
  sEseg *csp;
  sEseg *mcsp;
  sLink *lp;
  char *p;
  int ii;
  pwr_tBoolean bcast = bp->c.flags.b.broadcast;
  
  thread_MutexLock(&l.eseg.mutex);
  if (bcast) thread_MutexLock(&l.links.mutex);
  do {

    if (bcast) {
      for (
	lp = tree_Minimum(&sts, l.links.table);
	lp != NULL;
	lp = tree_Successor(&sts, l.links.table, lp)
      ) {
	if (lp->np->link.flags.b.connected)
	  break;
      }
    } else {
      lp = get_link(bp->b.info.receiver.nid, NULL);
    }

    if (lp == NULL)
      break;

    for (
      msp = NULL, size = bp->b.info.size + sizeof(bp->b.info), ii = 0, p = (char *)&bp->b.info;
      size > 0;
      ii++, p += MAX_SEGSIZE, size -= MAX_SEGSIZE
    ) {
      sp = eseg_alloc(NULL);
      sp->p = p;
      sp->bp = bp;
      sp->lp = lp;
      sp->id = ii;
      sp->size = MIN(size, MAX_SEGSIZE);
      sp->head.flags.b.event = eEvent_user;
      sp->head.flags.b.bcast = bcast;
      if (msp == NULL) {
	sp->head.flags.b.first = 1;
	msp = sp;
	/* todo initiera msp->le_seg.item = msp */
      } else {
	sp->head.flags.b.middle = 1;
	lst_InsertPred(NULL, &msp->le_seg, &sp->le_seg, sp);
      }
    }

    sp->head.flags.b.middle = 0;
    sp->head.flags.b.last = 1;
   
    if (bcast) {

      for (
	lp = tree_Successor(&sts, l.links.table, lp);
	lp != NULL;
	lp = tree_Successor(&sts, l.links.table, lp)
      ) {
	mcsp = NULL;
	sp = msp;
	ii = 0;
	do {
	  csp = eseg_alloc(NULL);
	  csp->p = sp->p;
	  csp->bp = bp;
	  csp->lp = lp;
	  csp->head.flags.m = sp->head.flags.m;
	  csp->size = sp->size;
	  csp->id = ii++;
	  if (mcsp == NULL) {
	    mcsp = csp;
	    /* todo initiera msp->le_seg.item = msp */
	  } else {
	    lst_InsertPred(NULL, &mcsp->le_seg, &csp->le_seg, sp);
	  }
	  lst_InsertPred(NULL, &sp->le_bcast, &csp->le_bcast, csp);
	  sp = lst_Succ(NULL, &sp->le_seg, NULL);
	} while (sp != msp);
      }

    }

  } while (0);

  if (bcast) thread_MutexUnlock(&l.links.mutex);
  thread_MutexUnlock(&l.eseg.mutex);

  return msp;
}

static void
eseg_free (
  sEseg *sp
)
{
  pwr_tBoolean bcast = sp->head.flags.b.bcast;

  if (bcast) thread_MutexLock(&l.bcast); 

    lst_Remove(NULL, &sp->c.le);
    if (lst_IsEmpty(NULL, &sp->le_bcast) && lst_IsEmpty(NULL, &sp->le_seg)) {
      if (sp->bp != NULL) {
	qdb_ScopeLock {
	  qdb_Free(NULL, sp->bp);
	} qdb_ScopeUnlock;
      }
    } else {
      lst_Remove(NULL, &sp->le_bcast);
      lst_Remove(NULL, &sp->le_seg);
    }

  if (bcast) thread_MutexUnlock(&l.bcast); 

  memset(sp, 0, sizeof(*sp));
  lst_Init(NULL, &sp->le_bcast, sp);
  lst_Init(NULL, &sp->le_seg, sp);
  lst_Init(NULL, &sp->c.le, sp);
  lst_InsertPred(&l.eseg.mutex, &l.eseg.lh, &sp->c.le, sp);
}

static char *
event_string (
  eEvent  event
)
{
  static char *es[eEvent_ + 1] = {
    "<<low>>",
    "connect",
    "down",
    "user",
    "ack",
    "<<high>>"
  };

  if (event <= eEvent__)
    return es[eEvent__];
  else if (event >= eEvent_)
    return es[eEvent_];
  else
    return es[event];

}

static pwr_tBoolean
expired (
  time_tClock clock
)
{
  int diff = clock - time_Clock(NULL, NULL);

  return diff <= 0;
}

static void *
export_thread ()
{
  pwr_tStatus sts;
  sEseg *sp;
  sEseg *ssp;
  sEseg *esp;
  qdb_sBuffer *bp;
  qdb_sQue *qp = NULL;

  qdb_ScopeLock {
    qp = qdb_Que(&sts, &qdb->g->qid_export, NULL);
  } qdb_ScopeUnlock;

  while (qdb->g->up) {
    sched_yield();
    qdb_ScopeLock {
      bp = qdb_Get(&sts, qp, qcom_cTmoEternal, NULL, 0);
      if (bp != NULL)
	bp = qdb_DetachBuffer(&sts, bp);
    } qdb_ScopeUnlock;

    if (bp == NULL) continue;
    pwr_Assert(bp->c.type == qdb_eBuffer_base);

    sp = eseg_build(bp);
    if (sp == NULL) {
      qdb_ScopeLock {
	qdb_Free(NULL, bp);
      } qdb_ScopeUnlock;
      continue;
    }

    thread_MutexLock(&l.bcast);

      esp = sp;
      do {
        ssp = esp;
        do {
	  ssp->c.action = eAction_export;
	  que_Put(NULL, &ssp->lp->q_in, &ssp->c.le, ssp);
	  ssp = lst_Succ(NULL, &ssp->le_seg, NULL) ;
        } while (ssp != esp);
        esp = lst_Succ(NULL, &ssp->le_bcast, NULL) ;
      } while (esp != sp);

    thread_MutexUnlock(&l.bcast);
  }

  return NULL;

}

static sLink *
get_link (
  pwr_tNodeId	nid,
  sMsg		*mp
)
{
  pwr_tStatus sts;
  sLink *lp;

  if (nid == qdb->my_node->nid) return NULL;
  if (nid == qdb->no_node->nid) return NULL;

  thread_MutexLock(&l.links.mutex);

  lp = tree_Find(&sts, l.links.table, &nid);
  if (lp == NULL)
    lp = new_link(nid, mp);    

  thread_MutexUnlock(&l.links.mutex);  

  return lp;
}

static time_tClock
get_tmo (
  sLink *lp,
  pwr_tBoolean do_inc
)
{
  int rto;

  rto = (lp->np->link.rtt_rto + 0.5);

  if (rto > lp->np->link.rtt_rxmax) {
    rto = lp->np->link.rtt_rxmax;
    if (lp->np->link.flags.b.active)
      link_stalled(lp);
  } else if (do_inc) {
    lp->np->link.rtt_rto *= 2;
  }

  return (time_tClock) (time_Clock(NULL, NULL) + rto);
}

#if defined OS_VMS
static void
read_ast (
  thread_sCond *cp
)
{

  pthread_cond_signal_int_np(&cp->c);
}
#endif

static void *
import_thread ()
{
  sMsg msg;
  sIseg *sp;
  sLink *last_link = NULL;
  int bytes;
  thread_sMutex *mp = &l.import.mutex;
#if defined OS_VMS
  pwr_tStatus sts;
  int state;
  thread_sCond *cp = &l.import.cond;
  sIosb iosb;
#endif


  thread_MutexLock(mp);

  for (sp = iseg_alloc() ; ; ) {
    set_recvmsg(sp, &msg);
#if defined(OS_VMS)

    memset(&iosb, 0, sizeof(iosb));
    
    sts = sys$qio(l.import_ef, l.sock, IO$_READVBLK, &iosb, read_ast, cp, 0, 0, &msg.rsa_d, 0, 0, &msg.desc);

    if (EVEN(sts)) {
      errh_Error("import, sys$qio, sts = %m", sts);
      continue;
    }

    while ((sts = sys$readef(l.import_ef, &state)) == SS$_WASCLR)
      thread_CondWait(cp, mp);

    pwr_Assert(sts == SS$_WASSET);

    if (ODD(iosb.sts)) {
      bytes = iosb.tsize;
    } else {
      errh_Error("import, sys$qio, iosb.sts %m", (int)iosb.sts);
      sched_yield();
      continue;
    }
#else
    bytes = recvmsg(l.sock, &msg.msg, 0);

    if (bytes == -1 && errno != EHOSTDOWN && errno != EHOSTUNREACH) {
      errh_Error("recvmsg failed %m(%d)", errno_Status(errno), errno);
      sched_yield();
      continue;
    }       
#endif

    sp->size = bytes - sizeof(sp->head);
    sp->ts_recv = time_Clock(NULL, NULL);
    decode_head(&sp->head, &msg.head);
    if (last_link != NULL && sp->head.nid == last_link->nid)
      sp->lp = last_link;
    else
      sp->lp = last_link = get_link(sp->head.nid, &msg);
    if (sp->lp == NULL) {
      errh_Error("get_link, returned NULL");
      continue;   
    }
    sp->c.action = eAction_import;
    sp->lp->np->get.segs++;
    sp->lp->np->get.bytes += bytes;
#if defined OS_VMS
    sp->lp->np->link.sa.sin_family = msg.sa.sin_family;
    sp->lp->np->link.sa.sin_port = msg.sa.sin_port;
    sp->lp->np->link.sa.sin_addr.s_addr = msg.sa.sin_addr;
#else
    sp->lp->np->link.sa = msg.sa;
#endif
    que_Put(NULL, &sp->lp->q_in, &sp->c.le, sp);
    sp = iseg_alloc();
  }

  thread_MutexUnlock(mp);

  return NULL;
}

static void
ini_link_info (
  qdb_sLinkInfo	*lp
)
{
  qdb_sNode	*my_np = qdb->my_node;

  strcpy(lp->name, my_np->name);

  lp->version = ntohl(my_np->version);
  lp->nid     = ntohl(my_np->nid);
  lp->birth   = ntohl(my_np->birth);
  lp->bus     = ntohl(qdb->g->bus);
  lp->os      = ntohl(my_np->os);
  lp->hw      = ntohl(my_np->hw);
  lp->bo      = ntohl(my_np->bo);
  lp->ft      = ntohl(my_np->ft);
}

static sIseg *
iseg_alloc ()
{
  sIseg *sp;

  sp = lst_RemoveSucc(&l.iseg.mutex, &l.iseg.lh, NULL);
  if (sp == NULL) {
    sp = (sIseg *)calloc(1, sizeof(*sp));
    lst_Init(NULL, &sp->c.le, sp);
    sp->size = sizeof(sp->buff);
  }

  return sp;
}

static void
iseg_free (
  sIseg *sp
)
{

  pwr_Assert(!lst_IsLinked(NULL, &sp->c.le));
  memset(sp, 0, sizeof(*sp));
  lst_Init(NULL, &sp->c.le, sp);
  sp->size = sizeof(sp->buff);
  lst_InsertPred(&l.iseg.mutex, &l.iseg.lh, &sp->c.le, sp);

}

static void
iseg_import (
  sLink		*lp,
  sIseg		*sp
)
{
  pwr_tStatus sts;
  int diff;
  qdb_sInfo *ip;
  qdb_sQue *qp;

  if (!lp->np->link.flags.b.connected) {
    return;
  }
    
  diff = sp->head.lack.seq - lp->np->link.rack.seq;
  if (diff != 1) {
    return;
  }

  if (sp->head.flags.b.first) {
    pwr_Assert(lp->bp == NULL);
    ip = (qdb_sInfo *)sp->buff;
    decode_info(ip);
    qdb_ScopeLock {
      lp->bp = qdb_Alloc(&sts, qdb_eBuffer_base, ip->size);
   } qdb_ScopeUnlock;
    lp->bp->c.flags.m |= ip->flags.m & qdb_mBuffer_maskExport;
    lp->p = (char *)&lp->bp->b.info;
  } else if (!sp->head.flags.b.first) {
    pwr_Assert(lp->bp != NULL);
    pwr_Assert(lp->p != NULL);
  }

  memcpy(lp->p, sp->buff, sp->size);
  lp->p += sp->size;

  if (sp->head.flags.b.last) {

    lp->bp->c.flags.b.imported = 1;

    qdb_ScopeLock {
      qp = qdb_Que(&sts, &lp->bp->b.info.receiver, NULL);
      if (qp == NULL) {
	/* To do !!!  Send notification reply if requested. */
	qdb_Free(NULL, lp->bp);
      } else {
	qdb_Put(NULL, lp->bp, qp);
      }
    } qdb_ScopeUnlock;

    lp->bp = NULL;
  }
}

/* An ack message has arrived from a remote node.
   Free the segments in window that have been
   succesfully received at remote node.  */

static void
lack (
  sLink *lp,
  sIseg *isp
)
{
  lst_sEntry *se;
  int diff;
  sEseg *sp, *nsp;

  for (sp = lst_Succ(NULL, &lp->lh_win, &se); se != &lp->lh_win; sp = nsp) {
    nsp = lst_Succ(NULL, se, &se);
    diff = isp->head.rack.seq - sp->head.lack.seq;

    if (diff >= 0) {
      /* This segment is either the acked segment or an older,
	 it can be released.  */

      if (diff == 0 && isp->head.rack.ts == sp->head.lack.ts)
	update_rtt(lp, isp);

      window_remove(lp, sp);

      eseg_free(sp);
    } else {
      /* This is the oldest not acked segment. */
      lp->np->link.lack = sp->head.lack;
      break;
    }
  }
}

static void
link_active (
  sLink *lp
)
{
  pwr_tStatus sts;

  if (lp->np->link.flags.b.active) {
    pwr_Assert(lp->np->link.flags.b.connected);
    return;
  }

  start_rto(lp);
  lp->np->link.flags.b.active = 1;
  lp->np->link.flags.b.connected = 1;
  errh_Info("Active, link to %s (%s)",
    lp->np->name, cdh_NodeIdToString(NULL, lp->np->nid, 0, 0));
  qdb_ScopeLock {
    lp->np->flags.b.active = 1;
    lp->np->flags.b.connected = 1;
    qdb_NetEvent(&sts, lp->np, qcom_eStype_linkActive);
  } qdb_ScopeUnlock;
  check_link_status();
}

static void
link_connect (
  sLink *lp,
  sIseg *sp
)
{
  pwr_tStatus sts;


  lp->np->link.flags.b.active = 1;

  if (lp->np->link.flags.b.connected)
    return;

  lp->np->link.flags.b.connected = 1;
  errh_Info("Connected, link to %s (%s)",
    lp->np->name, cdh_NodeIdToString(NULL, lp->np->nid, 0, 0));
  qdb_ScopeLock {
    lp->np->birth = sp->head.birth;
    clock_gettime(CLOCK_REALTIME, &lp->np->timeup);
    lp->np->flags.b.connected = 1;
    lp->np->flags.b.active = 1;
    set_link_info(lp, (qdb_sLinkInfo *)sp->buff);
    qdb_NetEvent(&sts, lp->np, qcom_eStype_linkConnect);
  } qdb_ScopeUnlock;

  check_link_status();
}

static void
link_disconnect (
  sLink *lp
)
{
  pwr_tStatus sts;
  sEseg *sp;

  if (!lp->np->link.flags.b.connected) {
    pwr_Assert(!lp->np->link.flags.b.active);
    return;
  }

  lp->np->link.pending_rack = NO;
  lp->np->link.flags.b.active = 0;
  lp->np->link.flags.b.connected = 0;
  memset(&lp->np->link.lack, 0, sizeof(lp->np->link.lack));
  memset(&lp->np->link.rack, 0, sizeof(lp->np->link.rack));
  lp->np->link.seq = 0;
  start_rto(lp);
  lp->np->link.rack_tmo = 0;

  /* Empty send list */

  for (
    sp = lst_Succ(NULL, &lp->lh_send, NULL);
    sp != NULL;
    sp = lst_Succ(NULL, &lp->lh_send, NULL)
  ) {
    lst_Remove(NULL, &sp->c.le);
    eseg_free(sp);
  }

  /* Empty window list */

  for (
    sp = lst_Succ(NULL, &lp->lh_win, NULL);
    sp != NULL;
    sp = lst_Succ(NULL, &lp->lh_win, NULL)
  ) {
    lst_Remove(NULL, &sp->c.le);
    eseg_free(sp);
  }

  lp->np->link.win_count = 0;

  errh_Info("Disconnected, link to  %s (%s)",
    lp->np->name, cdh_NodeIdToString(NULL, lp->np->nid, 0, 0));
  qdb_ScopeLock {
    if (lp->bp != NULL) 
      qdb_Free(NULL, lp->bp);
    lp->np->flags.b.active = 0;
    lp->np->flags.b.connected = 0;
    qdb_NetEvent(&sts, lp->np, qcom_eStype_linkDisconnect);
  } qdb_ScopeUnlock;

  lp->bp = NULL;
  lp->p = NULL;
  sp = create_connect(lp);
  lst_InsertSucc(NULL, &lp->lh_send, &sp->c.le, sp);

  check_link_status();
}

static void
link_import (
  sLink *lp,
  sIseg *sp
)
{

  if (!lp->np->link.flags.b.active && lp->np->birth == sp->head.birth)
    link_active(lp);
  else if (lp->np->birth != sp->head.birth && lp->np->link.flags.b.connected)
    link_disconnect(lp);
  else if (lp->np->birth != sp->head.birth && !lp->np->link.flags.b.connected)
    link_redisconnect(lp);

  switch (sp->head.flags.b.event) {
  case eEvent_user:
    if (lp->np->link.flags.b.active) {
      lack(lp, sp);
      iseg_import(lp, sp);
      set_rack(lp, sp);
    }
    break;
  case eEvent_ack:
    if (lp->np->link.flags.b.active) {
      lack(lp, sp);
    }      
    break;
  case eEvent_connect:
    link_connect(lp, sp);
    lack(lp, sp);
    set_rack(lp, sp);
    break;
  default:
    break;
  }

  iseg_free(sp);
}

static void
link_redisconnect (
  sLink *lp
)
{
  sEseg *sp;

  pwr_Assert(!lp->np->link.flags.b.connected);
  pwr_Assert(!lp->np->link.flags.b.active);

  lp->np->link.pending_rack = NO;
  start_rto(lp);
  lp->np->link.rack_tmo = 0;

  sp = lst_Succ(NULL, &lp->lh_win, NULL);
  if (sp == NULL)
    sp = lst_Succ(NULL, &lp->lh_send, NULL);
  pwr_Assert(sp != NULL);
  pwr_Assert(sp->head.flags.b.event == eEvent_connect);

  sp->tmo = 0;

}

static void
link_send (
  sLink *lp
)
{
  sEseg *sp;

  if ((sp = window_tmo(lp)) != NULL) {
    seg_send(lp, sp);
  } else if ((sp = pending_send(lp)) != NULL) {
    window_insert(lp, sp);
    seg_send(lp, sp);
  } else if (lp->np->link.pending_rack && expired(lp->np->link.rack_tmo)) {
    send_ack(lp);
  }
}

static void
link_stalled (
  sLink *lp
)
{
  pwr_tStatus sts;

  if (!lp->np->link.flags.b.active)
    return;

  lp->np->link.flags.b.active = 0;
  errh_Info("Stalled, link to node %s (%s)",
    lp->np->name, cdh_NodeIdToString(NULL, lp->np->nid, 0, 0));
  qdb_ScopeLock {
    lp->np->flags.b.active = 0;
    qdb_NetEvent(&sts, lp->np, qcom_eStype_linkStalled);
  } qdb_ScopeUnlock;
  check_link_status();
}

static void *
link_thread (
  sLink *lp
)
{
  uSeg *sp;
  pwr_tBoolean pending = FALSE;

  for ( ; ; ) {
    sp = que_Get(NULL, &lp->q_in, link_tmo(lp), &lp->tmo);

    switch (sp->action) {
    case eAction_export:
/*      lst_InsertPred(NULL, &lp->lh_send, &sp->c.le, sp); */
      pending = clean_insert(&lp->lh_send, (sEseg *) sp, pending);
      if (pending) continue;
      else break;
    case eAction_import:
      link_import(lp, &sp->i);
      if (pending) continue;
      else break;
    case eAction_tmo:
      /* Nothing to do, all is done in link_send. */
      break;
    default:
      errh_Bugcheck(2, "unknown action");
      break;
    }
    link_send(lp);
  }

  return NULL;
}

static pwr_tDeltaTime *
link_tmo (
  sLink *lp
)
{
  int diff_send = 0;
  int diff_rack = 0;
  time_tClock clock = time_Clock(NULL, NULL);
  time_tClock tmo = 0;
  sEseg *sp;

  if (lst_Succ(NULL, &lp->lh_send, NULL) != NULL &&
      lp->np->link.win_count < lp->np->link.win_max)
    return time_ZeroD(&lp->np->link.timer);
  
  if (lp->np->link.pending_rack) {
    diff_rack = lp->np->link.rack_tmo - clock;
    diff_rack = MAX(0, diff_rack);
  }

  if ((sp = lst_Succ(NULL, &lp->lh_win, NULL)) != NULL) {
    diff_send = sp->tmo - clock;
    diff_send = MAX(0, diff_send);
  }

  if (lp->np->link.pending_rack && sp != NULL) {
    tmo = MIN(diff_rack, diff_send);
  } else if (lp->np->link.pending_rack) {
    tmo = diff_rack;
  } else if (sp != NULL) {
    tmo = diff_send;
  } else {
    lp->np->link.timer.tv_sec = 999999999;
    lp->np->link.timer.tv_nsec = 0;
    return &lp->np->link.timer;
  }

  return time_ClockToD(NULL, &lp->np->link.timer, tmo);
}

static sLink *
new_link (
  pwr_tNodeId	nid,
  sMsg		*mp
)
{
  pwr_tStatus sts;
  sLink *lp;
  sEseg *sp;
  static int tics_per_sec = 0;

  if (nid == qdb->my_node->nid) return NULL;
  if (nid == qdb->no_node->nid) return NULL;

  lp = tree_Insert(&sts, l.links.table, &nid);
  pwr_Assert(lp != NULL);

  if (!tics_per_sec)
    tics_per_sec = sysconf(_SC_CLK_TCK);

  l.links.count++;

  qdb_ScopeLock {
    lp->np = qdb_AddNode(&sts, nid, 0);
  } qdb_ScopeUnlock;

  pwr_Assert(lp->np != NULL);

  que_Create(NULL, &lp->q_in);
  lst_Init(NULL, &lp->lh_send, NULL);
  lst_Init(NULL, &lp->lh_win, NULL);
  lp->np->link.win_max = 1;
  lp->np->link.rtt_rxmax = (RTT_RXMAX * tics_per_sec) / 1000;
  lp->np->link.rtt_rxmin = (RTT_RXMIN * tics_per_sec) / 1000;
  lp->tmo.c.action = eAction_tmo;
                                    
  if (mp != NULL) {
#if defined OS_VMS
    lp->np->sa.sin_family = mp->sa.sin_family;
    lp->np->sa.sin_port = mp->sa.sin_port;
    lp->np->sa.sin_addr.s_addr = mp->sa.sin_addr;
#else
    lp->np->sa = mp->sa;
#endif
  }

  sp = create_connect(lp);
  que_Put(NULL, &lp->q_in, &sp->c.le, sp);
  
  thread_Create(&lp->thread, lp->np->name, link_thread, lp);

  return lp;
}

static void
open_socket ()
{

#if defined OS_VMS
  pwr_tStatus sts;
  short sock_args[2] = {UCX$C_UDP, INET_PROTYP$C_DGRAM};
  struct dsc$descriptor dev = {10, DSC$K_CLASS_S, DSC$K_DTYPE_T, "UCX$DEVICE"};
  sSockaddr sa;
  struct itlst {
    int len;
    sSockaddr *sa;
  } sa_d;
  sIosb iosb;

  sts = sys$assign(&dev, &l.sock, 0, 0);
  if (EVEN(sts)) {
    errh_Error("sys$assign, %m", sts);
    exit(sts);
  }
  
  sa.sin_family = UCX$C_AF_INET;
  sa.sin_addr = UCX$C_INADDR_ANY;
  sa.sin_port = qdb->my_node->sa.sin_port;
  memset(sa.blkb, 0, sizeof(sa.blkb));

  sa_d.len = sizeof(sa);
  sa_d.sa  = &sa;

  sts = sys$qiow(l.send_ef, l.sock, IO$_SETMODE, &iosb, 0, 0, sock_args, 0, &sa_d, 0,0,0);
  if (EVEN(sts)) {
    errh_Error("bind, %m", sts) ;
    exit(sts);
  }
#else
  if ((l.sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    errh_Error("socket failed: %s", strerror(errno));
    exit(errno_ExitErrno(errno));
  }

  l.sa.sin_family = AF_INET;  
  l.sa.sin_addr.s_addr = htonl(INADDR_ANY);
  l.sa.sin_port = qdb->my_node->sa.sin_port;

  if ((bind(l.sock, (struct sockaddr *) &l.sa, sizeof(l.sa))) < 0) {
    errh_Error("bind failed: %s", strerror(errno));
    close(l.sock);
    exit(errno_ExitErrno(errno));
  }
#endif
}

static sEseg *
pending_send (
  sLink *lp
)
{

  if (lp->np->link.win_count >= lp->np->link.win_max)
    return NULL;

  return lst_RemoveSucc(NULL, &lp->lh_send, NULL);
}

static void 
send_ack (
  sLink *lp
)
{
  sHead head;
  sMsg msg;
  int bytes;
#if defined OS_VMS
  pwr_tStatus sts;
  sIosb iosb;
#endif

  head.flags.b.event = eEvent_ack;
  head.flags.m |= mSeg_single;
  head.lack.seq = lp->np->link.seq;
  head.lack.ts  = time_Clock(NULL, NULL);
  head.nid = l.head.nid;
  head.birth = l.head.birth;
  head.rack = lp->np->link.rack;

  set_sendmsg(lp, NULL, &msg);
  encode_head(&msg.head, &head);

  thread_MutexLock(&l.send_mutex);
#if defined OS_VMS
    sts = sys$qiow(l.send_ef, l.sock, IO$_WRITEVBLK, &iosb, 0, 0, 0, 0, &msg.lsa_d, 0, &msg.desc, 0);

    if (EVEN(sts)) {
      errh_Error("ack, sys$qiow, sts %m", sts);
      exit(sts);
    }

    if (ODD(iosb.sts)) {
      bytes = iosb.tsize;
    } else {
      errh_Error("ack, sys$qiow, iosb.sts %m", (int)iosb.sts);
      bytes = -1;
      errno = iosb.tsize;
    }
#else
    bytes = sendmsg(l.sock, &msg.msg, 0);
#endif
  thread_MutexUnlock(&l.send_mutex);

  if (bytes != -1) {
    lp->np->link.pending_rack = NO;
    lp->np->put.segs++;
    lp->np->put.bytes += bytes;
  }

  sched_yield();
}

static void
set_link_info (
  sLink		*lp,  
  qdb_sLinkInfo	*ip
)
{
  qdb_sNode	*np = lp->np;

  strcpy(np->name, ip->name);

  np->version = ntohl(ip->version);
  np->nid     = ntohl(ip->nid);
  np->birth   = ntohl(ip->birth);
  lp->np->link.bus = ntohl(ip->bus);
  np->os      = ntohl(ip->os);
  np->hw      = ntohl(ip->hw);
  np->bo      = ntohl(ip->bo);
  np->ft      = ntohl(ip->ft);
}

static void
set_rack (
  sLink *lp,
  sIseg *sp
)
{
  int diff;

  if (!lp->np->link.flags.b.connected)
    return;

  if (sp->head.lack.seq == 0)
    return;

  diff = sp->head.lack.seq - lp->np->link.rack.seq;
  if (diff == 0 || diff == 1) {
    lp->np->link.rack = sp->head.lack;
  } else if (diff > 1) {
#if 1
    if ((++lp->np->link.err_seq % 20) == 1) {
      errh_Info("%s, %d sequence error %d segments %s : (%d)[%d]\n",
	lp->np->name, lp->np->link.err_seq, diff - 1,
	event_string(sp->head.flags.b.event),
	sp->head.lack.seq, sp->head.rack.seq);
    }
#endif
    return;
  } else {
#if 1
    if ((++lp->np->link.err_red % 20) == 1) {
      errh_Info("%s, %d redundant segment %s : (%d)[%d]\n",
	lp->np->name, lp->np->link.err_red, event_string(sp->head.flags.b.event),
	sp->head.lack.seq, sp->head.rack.seq);
    }
#endif
    return;
  }

  if (sp->head.flags.b.event == eEvent_ack)
    return;

  if (sp->head.flags.b.resent) {
    lp->np->link.pending_rack = YES;
    lp->np->link.rack_tmo = time_Clock(NULL, NULL);
  } else if (lp->np->link.pending_rack) {
    lp->np->link.rack_tmo = time_Clock(NULL, NULL);
  } else {
    lp->np->link.pending_rack = YES;
    lp->np->link.rack_tmo = time_Clock(NULL, NULL) + RACK_TMO; /* lp->rack_tmo; */
  }

}

static void
set_recvmsg (
  sIseg *sp,
  sMsg *mp
)
{

  mp->iov[0].iov_base = (char *) &mp->head;
  mp->iov[0].iov_len  = sizeof(mp->head);
  mp->iov[1].iov_base = (char *)sp->buff;
  mp->iov[1].iov_len  = sizeof(sp->buff);

  memset(&mp->sa, 0, sizeof(mp->sa));

#if defined OS_VMS
  mp->desc.base = (char*) &mp->iov;
  mp->desc.len = sizeof(mp->iov);

  mp->rsa_d.name     = &mp->sa;
  mp->rsa_d.namelen  = sizeof(mp->sa);
  mp->rsa_d.retlen   = &mp->sa_retlen;
#else
  memset(&mp->msg, 0, sizeof(mp->msg));
  mp->msg.msg_iov = mp->iov;
  mp->msg.msg_iovlen = 2;

  mp->msg.msg_name = (void *) &mp->sa;
  mp->msg.msg_namelen = sizeof(mp->sa);

#endif
}

static void 
seg_send (
  sLink *lp,
  sEseg *sp              
)
{
  sMsg msg;
#if defined OS_VMS
  pwr_tStatus sts;
  sIosb iosb;
#endif

  sp->head.lack.ts = time_Clock(NULL, NULL);
  sp->head.nid     = l.head.nid;
  sp->head.birth   = l.head.birth;
  sp->head.rack    = sp->lp->np->link.rack;

  thread_MutexLock(&l.send_mutex);

  set_sendmsg(lp, sp, &msg);
  encode_head(&msg.head, &sp->head);

#if defined OS_VMS
    sts = sys$qiow(l.send_ef, l.sock, IO$_WRITEVBLK, &iosb, 0, 0, 0, 0, &msg.lsa_d, 0, &msg.desc, 0);

    if (EVEN(sts)) {
      errh_Error("seg, sys$qiow, sts = %m", sts);
      exit(sts);
    }

    if (ODD(iosb.sts)) {
      sp->bytes = iosb.tsize;
    } else {
      errh_Error("seg, sys$qiow, iosb.sts = %m", (int)iosb.sts);
      sp->bytes = -1;
      errno = iosb.tsize;
    }
#else
    sp->bytes = sendmsg(l.sock, &msg.msg, 0);
#endif
  thread_MutexUnlock(&l.send_mutex);

  if (sp->bytes == -1) {
    if (errno != EHOSTDOWN && errno != EHOSTUNREACH) {
      errh_Error("sendmsg to node %s (%s) failed\n(%d) %s", sp->lp->np->name, 
	cdh_NodeIdToString(NULL, sp->lp->nid, 0, 0), errno, strerror(errno));
    }
  } else {
    lp->np->link.pending_rack = NO;
    lp->np->put.segs++;
    lp->np->put.bytes += sp->bytes;
  }
  if (lst_IsSucc(NULL, &lp->lh_win, &sp->c.le)) {
    if (!sp->head.flags.b.resent) {
      start_rto(lp);
      sp->head.flags.b.resent = 1;
    }
    sp->tmo = get_tmo(lp, YES);
  } else {
    sp->tmo = get_tmo(lp, NO);
  }

  sched_yield();
}

static void
set_sendmsg (
  sLink *lp,
  sEseg *sp,
  sMsg *mp
)
{
  int i = 0;
  int size = 0;
  char *p;

  pwr_Assert(lp != NULL);

#if defined OS_VMS
  mp->desc.base = &mp->iov;

  mp->iov[i].iov_base = (char *) &mp->head;
  mp->iov[i++].iov_len  = sizeof(mp->head);

  if (sp == NULL) {
    mp->iov[i].iov_base = NULL;
    mp->iov[i].iov_len  = 0;
  } else if (sp->head.flags.b.event == eEvent_connect) {
    mp->iov[i].iov_base = (char *) &l.link_info;
    mp->iov[i++].iov_len = sizeof(l.link_info);
  } else if (sp->bp != NULL) {
    p = (char *) sp->p;
    size = sp->size;
    if (sp->head.flags.b.first) {
      encode_info(&mp->info, (qdb_sInfo *)p);
      mp->iov[i].iov_base = (char *) &mp->info;
      mp->iov[i++].iov_len  = sizeof(mp->info);
      p += sizeof(mp->info);
      size -= sizeof(mp->info);
    }
    mp->iov[i].iov_base = p;
    mp->iov[i++].iov_len = size;
  }

  mp->desc.len = i * sizeof(mp->iov[0]);

  memset(&mp->sa, 0, sizeof(mp->sa));
  mp->sa.sin_family = lp->np->sa.sin_family;
  mp->sa.sin_port   = lp->np->sa.sin_port;
  mp->sa.sin_addr   = lp->np->sa.sin_addr.s_addr;

  mp->lsa_d.name     = &mp->sa;
  mp->lsa_d.namelen  = sizeof(mp->sa);
#else
  memset(&mp->msg, 0, sizeof(mp->msg));
  mp->msg.msg_iov = mp->iov;

  mp->iov[i].iov_base = (char *) &mp->head;
  mp->iov[i++].iov_len  = sizeof(mp->head);

  if (sp == NULL) {
    ;
  } else if (sp->head.flags.b.event == eEvent_connect) {
    mp->iov[i].iov_base = (char *) &l.link_info;
    mp->iov[i++].iov_len = sizeof(l.link_info);
  } else if (sp->bp != NULL) {
    p = (char *) sp->p;
    size = sp->size;
    if (sp->head.flags.b.first) {
      encode_info(&mp->info, (qdb_sInfo *)p);
      mp->iov[i].iov_base = (char *) &mp->info;
      mp->iov[i++].iov_len  = sizeof(mp->info);
      p += sizeof(mp->info);
      size -= sizeof(mp->info);
    }
    mp->iov[i].iov_base = p;
    mp->iov[i++].iov_len = size;
  }    

  mp->msg.msg_iovlen = i;
  mp->msg.msg_name = (void *) &lp->np->sa;
  mp->msg.msg_namelen = sizeof(lp->np->sa);
#endif
}

static void
start_rto (
  sLink *lp
)
{
  double rto;

  rto = lp->np->link.rtt_srtt + (4.0 * lp->np->link.rtt_var);

  if (rto < lp->np->link.rtt_rxmin)
    rto = lp->np->link.rtt_rxmin;
  else if (rto > lp->np->link.rtt_rxmax)
    rto = lp->np->link.rtt_rxmax;

  lp->np->link.rtt_rto = rto;
}

static void
update_rtt (
  sLink *lp,
  sIseg	*sp
)
{
  int diff;
  double delta;

  if (sp->head.rack.seq == 0)
    return;

  diff = sp->ts_recv - sp->head.rack.ts;
  if (diff <= 0)
    return;

  lp->np->link.rtt_rtt = diff;
  delta = lp->np->link.rtt_rtt - lp->np->link.rtt_srtt;
  lp->np->link.rtt_srtt += delta / 8;

  if (delta < 0.0)
    delta = -delta; /* |delta| */

  lp->np->link.rtt_var += (delta - lp->np->link.rtt_var) / 4;
  
}

static void
window_insert (
  sLink *lp,
  sEseg *sp
)
{

  pwr_Assert(!lst_IsLinked(NULL, &sp->c.le));
  lst_InsertPred(NULL, &lp->lh_win, &sp->c.le, sp);
  lp->np->link.win_count++;
  sp->head.lack.seq = ++lp->np->link.seq;
}

static void
window_remove (
  sLink *lp,
  sEseg *sp
)
{

  pwr_Assert(lst_IsLinked(NULL, &sp->c.le));
  lst_Remove(NULL, &sp->c.le);
  lp->np->link.win_count--;
}

static sEseg *
window_tmo (
  sLink *lp
)
{
  sEseg *sp = lst_Succ(NULL, &lp->lh_win, NULL);

  if (sp == NULL || !expired(sp->tmo))
    return NULL;

  return sp;
}

static void
check_link_status(
)
{
  pwr_tStatus sts;
  pwr_tStatus linksts = PWR__SRUN;
  sLink *lp;

  for (lp = tree_Minimum(&sts, l.links.table); lp != NULL; lp = tree_Successor(&sts, l.links.table, lp)) {
    if ( !lp->np->link.flags.b.active)
      linksts = QCOM__DOWN;
  }
  if ( linksts != l.sts) {
    if ( errh_Severity(linksts) >= errh_Severity(qcom_sts))
      errh_SetStatus( linksts);
    else if ( errh_Severity(l.sts) >= errh_Severity(qcom_sts) &&
	      errh_Severity(linksts) < errh_Severity(qcom_sts))
      errh_SetStatus( qcom_sts);
    l.sts = linksts;    
  }
}

static void set_status( 
pwr_tStatus sts
)
{
  if ( sts != qcom_sts) {
    if ( errh_Severity(sts) >= errh_Severity(l.sts))
      errh_SetStatus( sts);
    else if ( errh_Severity(qcom_sts) >= errh_Severity(l.sts) &&
	      errh_Severity(sts) < errh_Severity(l.sts))
      errh_SetStatus( l.sts);
    qcom_sts = sts;    
  }
}

