/* rt_net.c -- Nethandler communication

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   This module contains routines to handle Qcom calls.
   It is common to both application code and nethandler code.  */

#if defined(OS_ELN)
# include $vaxelnc
# include $mutex
# include $kernelmsg
#elif defined(OS_VMS)
# include <stdio.h>
# include <stddef.h>
# include <descrip.h>
# include <string.h>
# include <lib$routines.h>
# if defined int32
#  error int32 is defined
# endif
# if defined uint32
#  undefine uint32
# endif
#elif defined(OS_LYNX) || defined(OS_LINUX)
# include <stdio.h>
# include <stddef.h>
# include <string.h>
# include <pthread.h>
#endif

#include "pwr.h"
#include "rt_qcom_msg.h"
#include "rt_qcom.h"
#include "rt_gdh_msg.h"
#include "rt_gdb.h"
#include "rt_qdb.h"
#include "rt_errh.h"
#include "rt_net_msg.h"
#include "rt_net.h"

/* The following variables are the interface between the net_*
   routines and the NetThread. The requestor must first
   aquire ownership of the l_mutex mutex, then fill in the
   function and the variables needed for that particular function.
   After this, the worksem is signalled which causes the process
   to start working. The requestor should then wait for the donesem
   before reading the result from the variables. Finally, the
   l_mutex should be released.  */

#if defined (OS_ELN)
# define NET_LOCK	ELN$LOCK_MUTEX(l_mutex)
# define NET_UNLOCK	ELN$UNLOCK_MUTEX(l_mutex)

  static MUTEX		l_mutex;

#elif defined (OS_LYNX) || defined(OS_LINUX)
# define NET_LOCK	pthread_mutex_lock(&l_mutex)
# define NET_UNLOCK	pthread_mutex_unlock(&l_mutex)

  static pthread_mutex_t	l_mutex;
#else
# define NET_LOCK
# define NET_UNLOCK
#endif

void
Count (
  pwr_tBoolean		receive,
  pwr_tNodeId		nid,
  qcom_sType		*type
);

static pwr_tBoolean
ConvertPut (
  pwr_tStatus		*sts,
  qcom_sQid		*qid,
  qcom_sPut		*put,
  void			*data
);

static pwr_tBoolean
ConvertGet (
  pwr_tStatus		*sts,
  qcom_sGet		*get,
  void			*data
);

#if 0
static void
Error (
  qcom_sQid		*remqid,
  pwr_tStatus		status
);
#endif

static pwr_tBoolean
Send (
  pwr_tStatus		*sts,
  qcom_sQid		*tgt,
  qcom_sPut		*put
);

static pwr_tBoolean
Put (
  pwr_tStatus		*sts,
  qcom_sQid		*tgt,
  void			*mp,
  net_eMsg		subtype,
  unsigned int		size
);

static void *
Receive (
  pwr_tStatus		*sts,
  qcom_sGet		*get,
  int			tmo
);

static pwr_tBoolean
Reply (
  pwr_tStatus		*sts,
  qcom_sGet		*get,
  qcom_sPut		*put
);


static void *
Request (
  pwr_tStatus		*sts,
  qcom_sQid		*tgt,
  qcom_sPut		*put,
  qcom_sGet		*get,
  net_eMsg		subtype
);

typedef bool_t (*tFuncXdr)(XDR*, void*); 

static tFuncXdr func_xdr[net_eMsg_] = {
  (tFuncXdr)xdr_net_sError,		/* A network error was detected */

  (tFuncXdr)xdr_net_sId,		/* Nethandler identification */
  (tFuncXdr)xdr_net_sId,		/* Nethandler identification acknowlege */
  (tFuncXdr)xdr_net_sIdAck2,		/* Nethandler idAck acknowlege */

  (tFuncXdr)xdr_net_sVolumes,		/*  */
  (tFuncXdr)xdr_net_sVolumesR,		/*  */

  (tFuncXdr)xdr_net_sSubAdd,		/* Add subscription */
  (tFuncXdr)xdr_net_sSubRemove,		/* Remove subscription */
  (tFuncXdr)xdr_net_sSubMessage,	/* Subscription data transfer */

  (tFuncXdr)xdr_net_sSanAdd,		/* Add subscription */
  (tFuncXdr)xdr_net_sSanRemove,		/* Remove subscription */
  (tFuncXdr)xdr_net_sSanUpdate,		/* Subscription data transfer */

  (tFuncXdr)xdr_net_sNameToObject,	/* Cache inquiry, fetch name */
  (tFuncXdr)xdr_net_sOidToObject,	/* Cache inquiry, fetch objid */
  (tFuncXdr)xdr_net_sObjectR,		/* response */

  (tFuncXdr)xdr_net_sGetObjectInfo,
  (tFuncXdr)xdr_net_sGetObjectInfoR,
  (tFuncXdr)xdr_net_sSetObjectInfo,
  (tFuncXdr)xdr_net_sSetObjectInfoR,

  NULL, /*xdr_net_sFlush,*/		/* Cache flush request */

  (tFuncXdr)xdr_net_sCreateObject,
  (tFuncXdr)xdr_net_sDeleteObject,
  (tFuncXdr)xdr_net_sMoveObject,
  (tFuncXdr)xdr_net_sRenameObject,

  (tFuncXdr)xdr_net_sNodeUp,		/* A node is up */
  NULL, /*xdr_net_sNodeDown*/		/* A node is down */

  (tFuncXdr)xdr_net_sGetCclass,
  (tFuncXdr)xdr_net_sGetCclassR,
  (tFuncXdr)xdr_net_sGetGclass,
  (tFuncXdr)xdr_net_sGetGclassR

};

void
Count (
  pwr_tBoolean		receive,
  pwr_tNodeId		nid,
  qcom_sType		*type
)
{
  pwr_tStatus		sts;
  gdb_sNode		*np = hash_Search(&sts, gdbroot->nid_ht, &nid);
  int                   msgtype = type->s;


  if (type->b != net_cMsgClass) 
    return;
  if (np == NULL) 
    return;

  if ((msgtype >= net_eMsg_) || (msgtype <= net_eMsg__)) {
    if (msgtype != net_eMsg_volumes7)
      return;
    msgtype = net_eMsg_volumes;
  }

  if (receive)
    np->rxmsg[msgtype]++;
  else 
    np->txmsg[msgtype]++;
}

static pwr_tBoolean
ConvertPut (
  pwr_tStatus		*sts,
  qcom_sQid		*qid,
  qcom_sPut		*put,
  void			*data
)
{
  pwr_tStatus		lsts;
  XDR			xdrs;
  qdb_sNode		*np;
  tFuncXdr              fXdr;

  if (put->type.b != net_cMsgClass)
    pwr_Return(FALSE, sts, NET__XDRFAILED);
  np = hash_Search(&lsts, &qdb->nid_ht, &qid->nid);
  if (np == NULL) pwr_Return(FALSE, sts, lsts);

  /* This is a simple way to fix the net_sGvolume difference between 
   * Neth version 7 and later. If this is needed for more messages 
   * then a generic solution must be implemented.
   */
  if (put->type.s == net_eMsg_volumes7) {
    put->type.s = net_eMsg_volumes;
    fXdr = xdr_net_sVolumes7;
  }
  else {
    if ((int)put->type.s <= (int)net_eMsg__ || (int)put->type.s >= (int)net_eMsg_)
      pwr_Return(FALSE, sts, NET__NOSUCHQCOMSUBT);
    fXdr = func_xdr[(int)put->type.s];
  }
  

  if (np == qdb->my_node || np->bo == qdb->my_node->bo) {
    if (put->data != data) memcpy(put->data, data, put->size);
    pwr_Return(TRUE, sts, NET__SUCCESS);
  }

  xdrmem_create(&xdrs, (char *) put->data, put->size, XDR_ENCODE);
  if(!(fXdr)(&xdrs, (char *) data))
    pwr_Return(FALSE, sts, NET__XDRFAILED);

  pwr_Return(TRUE, sts, NET__SUCCESS);
}

static pwr_tBoolean
ConvertGet (
  pwr_tStatus		*sts,
  qcom_sGet		*get,
  void			*data
)
{
  pwr_tStatus		lsts;
  XDR			xdrs;
  qdb_sNode		*np;
  tFuncXdr              fXdr;
  gdb_sNode             *gnp;
  pwr_tUInt32           netver;

  if (get->type.b != net_cMsgClass)
    pwr_Return(FALSE, sts, NET__XDRFAILED);
  np = hash_Search(&lsts, &qdb->nid_ht, &get->sender.nid);
  if (np == NULL) pwr_Return(FALSE, sts, lsts);

  if ((int)get->type.s <= (int)net_eMsg__ || (int)get->type.s >= (int)net_eMsg_)
    pwr_Return(FALSE, sts, NET__NOSUCHQCOMSUBT);


  /* This is a simple way to fix the net_sGvolume difference between 
   * Neth version 7 and later. If this is needed for more messages 
   * then a generic solution must be implemented.
   */
  fXdr = func_xdr[(int)get->type.s];
  if (get->type.s == net_eMsg_volumes) {
    gdb_ScopeLock {
      gnp = hash_Search(&lsts, gdbroot->nid_ht, &np->nid);
      netver = gnp->netver;
    } gdb_ScopeUnlock;

    if (netver == 7) {
      get->type.s = net_eMsg_volumes7;
      fXdr = xdr_net_sVolumes7;
    }
  }

  if (np == qdb->my_node || np->bo == qdb->my_node->bo) {
    if (get->data != data) memcpy(data, get->data, get->size);
    pwr_Return(TRUE, sts, NET__SUCCESS);
  }

  xdrmem_create(&xdrs, (char *) data, get->size, XDR_DECODE);    
  
  if(!(fXdr)(&xdrs, (char *) get->data))
    pwr_Return(FALSE, sts, NET__XDRFAILED);

  pwr_Return(TRUE, sts, NET__SUCCESS);
}

#if 0
static void
Error (
  qcom_sQid		*remqid,
  pwr_tStatus		status
)
{
  net_sError		err;
  pwr_tStatus		sts;
  qcom_sQid		tgt;
  qcom_sPut		put;

  if (gdbroot->my_qid.qix == net_cProcHandler) {
    errh_Error("error\n%m", status);
    return;
  }

  if (gdbroot->db->nethandler.qix == 0 && gdbroot->db->nethandler.nid == qcom_cNNid) {
    errh_Error("Neterror detected, but nethandler is not running\n%m", status);
    return;
  }

  tgt.qix	= net_cProcHandler;
  tgt.nid	= qcom_cNNid;

  err.sts	= status;
  err.remqid    = *remqid;

  put.type.b	= net_cMsgClass;
  put.type.s	= net_eMsg_error;
  put.reply	= qcom_cNQid;
  put.data  	= (char *)&err;
  put.size	= sizeof(err);

  qcom_Put(&sts, &tgt, &put);

  if (EVEN(sts))
    errh_Error("Error: reporting qcom neterror:\n%m", sts);
  else
    Count(0, remqid->nid, &put.type);

}
#endif

static pwr_tBoolean
Reply (
  pwr_tStatus		*sts,
  qcom_sGet		*get,
  qcom_sPut		*put
)
{

  pwr_Assert((put->size == 0 && put->data == NULL)
    || (put->size != 0 && put->data != NULL));

  put->type.b = net_cMsgClass;
  put->reply  = gdbroot->my_qid;

  if (!ConvertPut(sts, &get->reply, put, put->data))
    return FALSE;

  if (!qcom_Reply(sts, get, put))
    return FALSE;

  Count(0, get->reply.nid, &put->type);

  return TRUE;
}

static pwr_tBoolean
Send (
  pwr_tStatus		*sts,
  qcom_sQid		*tgt,
  qcom_sPut		*put
)
{

  pwr_Assert((put->size == 0 && put->data == NULL)
    || (put->size != 0 && put->data != NULL));

  put->type.b = net_cMsgClass;
  put->reply  = gdbroot->my_qid;

  if (!ConvertPut(sts, tgt, put, put->data))
    return FALSE;

  if (!qcom_Put(sts, tgt, put))
    return FALSE;

  Count(0, tgt->nid, &put->type);

  return TRUE;
}

static pwr_tBoolean
Put (
  pwr_tStatus		*sts,
  qcom_sQid		*tgt,
  void			*mp,
  net_eMsg		subtype,
  unsigned int		size
)
{
  pwr_tStatus		lsts;
  qcom_sPut		put;

  put.reply  = gdbroot->my_qid;
  put.type.b = net_cMsgClass;
  put.type.s = subtype;
  put.size   = size;
  put.data   = qcom_Alloc(&lsts, size);

  if (put.data == NULL)
    pwr_Return(FALSE, sts, NET__NULLDATA);

  if (!ConvertPut(sts, tgt, &put, mp)) {
    qcom_Free(NULL, put.data);
    return FALSE;
  }

  if (!qcom_Put(sts, tgt, &put)) {
    qcom_Free(NULL, put.data);
    return FALSE;   
  }

  Count(0, tgt->nid, &put.type);

  return TRUE;
}

static void *
Receive (
  pwr_tStatus		*sts,
  qcom_sGet		*get,
  int			tmo
)
{
  void			*p;

  memset(get, 0, sizeof(*get));

  p = qcom_Get(sts, &gdbroot->my_qid, get, tmo);
  if (p == NULL)
    return NULL;

  if (get->type.b == net_cMsgClass) {
    Count(1, get->sender.nid, &get->type);
    if (!ConvertGet(sts, get, p)) {
      qcom_Free(NULL, p);
      return NULL;
    }
  }

  return p;
}

static void *
Request (
  pwr_tStatus		*sts,
  qcom_sQid		*tgt,
  qcom_sPut		*put,
  qcom_sGet		*get,
  net_eMsg		subtype
)
{
  void			*gmp;
  qcom_sGet		lget;

  pwr_Assert((put->size == 0 && put->data == NULL)
    || (put->size != 0 && put->data != NULL));

  if (get == NULL) get = &lget;

  memset(get, 0, sizeof(*get));

  put->type.b = net_cMsgClass;
  put->reply  = gdbroot->my_qid;

  if (!ConvertPut(sts, tgt, put, put->data))
    return NULL;

  gmp = qcom_Request(sts, tgt, put, &gdbroot->my_qid, get, net_cSendRcvTmo); 
  if (gmp == NULL) 
    return NULL;

  Count(0, tgt->nid, &put->type);

  if (
    get->type.b != net_cMsgClass ||
    get->type.s != subtype
  ) {
    qcom_Free(NULL, gmp);
    pwr_Return(NULL, sts, QCOM__WEIRD);
  }

  if (!ConvertGet(sts, get, gmp)) {
    qcom_Free(NULL, gmp);
    return NULL;
  }

  Count(1, get->sender.nid, &get->type);

  return gmp;
}

void *
net_Alloc (
  pwr_tStatus		*status,
  qcom_sPut		*put,
  int			size,
  net_eMsg		subtype
)
{
  net_sMessage		*mp;
  pwr_dStatus		(sts, status, NET__SUCCESS);

  NET_LOCK;
  mp = qcom_Alloc(sts, size);
  NET_UNLOCK;

  if (mp == NULL)
    return mp;

  put->reply  = gdbroot->my_qid;
  put->type.b = net_cMsgClass;
  put->type.s = subtype;
  put->size   = size;
  put->data   = mp;

  mp->nid  = gdbroot->db->nid;

  return mp;
}

pwr_tBoolean
net_Free (
  pwr_tStatus		*status,
  void			*mp
)
{
  pwr_tBoolean		ok;
  pwr_dStatus		(sts, status, NET__SUCCESS);

  NET_LOCK;
  ok = qcom_Free(sts, mp);
  NET_UNLOCK;

  return ok;
}

pwr_tBoolean
net_Send (
  pwr_tStatus		*status,
  qcom_sQid		*tgt,
  qcom_sPut		*put
)
{
  void			*mp = put->data;
  pwr_tBoolean		ok;
  pwr_dStatus		(sts, status, NET__SUCCESS);

  ((net_sMessage *)mp)->msn = 0;
  ((net_sMessage *)mp)->nid = gdbroot->db->nid;

  NET_LOCK;
  ok = Send(sts, tgt, put);
  NET_UNLOCK;

  return ok;
}

/* Same as net_Send, but used when replying to a request.
   Gives possibility to control msn.  */

pwr_tBoolean
net_Reply (
  pwr_tStatus		*status,
  qcom_sGet		*get,
  qcom_sPut		*put
)
{
  pwr_tBoolean		ok;
  pwr_dStatus		(sts, status, NET__SUCCESS);

  ((net_sMessage *)put->data)->msn = ((net_sMessage *)get->data)->msn;
  ((net_sMessage *)put->data)->nid = gdbroot->db->nid;

  NET_LOCK;
  ok = Reply(sts, get, put);
  NET_UNLOCK;

  return ok;
}

pwr_tBoolean
net_Put (
  pwr_tStatus		*status,
  qcom_sQid		*tgt,
  void			*mp,
  net_eMsg		type,
  int			size
)
{
  pwr_tBoolean		ok;
  pwr_dStatus		(sts, status, NET__SUCCESS);

  ((net_sMessage *)mp)->msn = 0;
  ((net_sMessage *)mp)->nid = gdbroot->db->nid;

  NET_LOCK;
  ok = Put(sts, tgt, mp, type, size);
  NET_UNLOCK;

  return ok;
}

void *
net_Receive (
  pwr_tStatus		*status,
  qcom_sGet		*get,
  int			tmo
)
{
  void			*p;
  pwr_dStatus		(sts, status, NET__SUCCESS);

  gdb_AssumeUnlocked;

  NET_LOCK;
    p = Receive(sts, get, tmo);
  NET_UNLOCK;

  return p;
}

void *
net_Request (
  pwr_tStatus		*status,
  qcom_sQid		*tgt,
  qcom_sPut		*put,
  qcom_sGet		*get,
  net_eMsg		gtype
)
{
  void			*p;
  pwr_dStatus		(sts, status, NET__SUCCESS);

  gdb_AssumeUnlocked;

  ((net_sMessage *)put->data)->msn = gdbroot->db->rqgen++;
  ((net_sMessage *)put->data)->nid = gdbroot->db->nid;

  NET_LOCK;
  p = Request(sts, tgt, put, get, gtype);
  NET_UNLOCK;

  return p;
}

pwr_tBoolean
net_Connect (
  pwr_tStatus		*status,
  qcom_sAid		*aid,
  qcom_sQid		*qid,
  qcom_sQattr		*attr,
  const char		*name
)
{
  pwr_tBoolean		ok;
  pwr_dStatus		(sts, status, NET__SUCCESS);

  /* Initialize. This routine should be called exactly once... */

#if defined OS_LYNX || defined OS_LINUX
  {
    pthread_mutexattr_t mattr;

# if defined OS_LYNX && defined PWR_LYNX_30
    pthread_mutexattr_create(&mattr);
    if (pthread_mutex_init(&l_mutex, mattr) == -1)
# else
    pthread_mutexattr_init(&mattr);
    if (pthread_mutex_init(&l_mutex, &mattr) == -1)
# endif
      pwr_Return(FALSE, sts, NET__MUTEXINIT);


# if defined OS_LYNX && defined PWR_LYNX_30
    pthread_mutexattr_delete(&mattr);
# else
    pthread_mutexattr_destroy(&mattr);
# endif


  }
#elif defined OS_ELN 
  ELN$CREATE_MUTEX(l_mutex, NULL);
#endif

  NET_LOCK;
  if (!qcom_Init(sts, aid, name))
    ok = FALSE;
  else
    ok = qcom_CreateQ(sts, qid, attr, "neth");
  NET_UNLOCK;

  return ok;
}
