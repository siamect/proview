/* rt_subcm.c -- Object data subscription, client monitor functions.

   PROVIEW/R
   Copyright (C) 1997-98 by Mandator AB.

   This module contains routines to handle subscription.
   Those routines can NOT be called in a job context other than
   that of the nethandler.  */

#ifndef OS_ELN
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
#endif


#if defined(OS_ELN)
# include $vaxelnc
#elif defined (OS_VMS)
# include <lib$routines.h>
#endif

#include "pwr.h"
#include "co_cdh.h"
#include "co_time.h"
#include "rt_gdh_msg.h"
#include "rt_errh.h"
#include "rt_pool.h"
#include "rt_gdb.h"
#include "rt_sub.h"
#include "rt_net.h"
#include "rt_ndc.h"
#include "rt_vol.h"
#include "rt_subc.h"
#include "rt_subcm.h"
#include "rt_qcom.h"


/* Check if any client timeouts have occurred.
   The database should be locked on entry.  */

void
subcm_CheckTimeout ()
{
  pool_sQlink		*cl;
  sub_sClient		*cp;
  pwr_tTime		curtim;
  pwr_tTime		restim;
  pwr_tDeltaTime	tmotim;
  pwr_tInt32		n;
  pwr_tInt32		i;

  gdb_AssumeLocked;

  gdbroot->db->tmocnt++;  /* Statistics */

  clock_gettime(CLOCK_REALTIME, &curtim);

  /* Calculate # of remote object clients to process this time */

  n = gdbroot->db->subt_lc * gdbroot->db->tmotime / gdbroot->db->tmolap;
  if (n == 0) n = MIN(1, gdbroot->db->subt_lc);

  for (i=0; i<n; i++) {

    /* Temporarily remove the first entry from the queue for processing.  */

    cl = pool_QremoveSucc(NULL, gdbroot->pool, &gdbroot->db->subt_lh);
    cp = pool_Qitem(cl, sub_sClient, subt_ll);

    /* Find out if the client has timed out */

    if (!cp->old) {
      /* cp->tmo in 10ths of seconds */
      tmotim.tv_sec = cp->tmo / 10;
      tmotim.tv_nsec = (cp->tmo - tmotim.tv_sec * 10) * 100000000;
      time_Aadd(&restim, &cp->lastupdate, &tmotim);       

      /* curtim >= tmotim + lastupdate */
      if (time_Acomp(&curtim, &restim) >= 0) 
	subc_SetOld(cp);
    } /* If it was old already */

    /* Reinsert the entry last in the queue.  */
    
    pool_QinsertPred(NULL, gdbroot->pool, cl, &gdbroot->db->subt_lh);
  }
}

/* Handles the reception of a subdata message.
   Database must NOT be locked.  */

void
subcm_Data (
  qcom_sGet		*get
)
{
  pwr_tStatus		sts;
  pwr_tTime		curtim;
  pool_tRef		mr;
  sub_sMessage		*mp;
  pwr_tUInt32		refcount = 0;
  pwr_tInt32		i;
  sub_sClient		*cp;
  pool_tRef		dr;
  net_sSubData		*dp;
  void			*adrs;
  gdb_sNode		*np;
  net_sSubRemove	*rp = NULL;
  qcom_sQid		tgt;
  gdb_sCclass		*ccp;
  ndc_sRemoteToNative	*tbl;
  

  clock_gettime(CLOCK_REALTIME, &curtim);

  gdb_AssumeUnlocked;

  tgt.qix = net_cProcHandler;

  gdb_ScopeLock {

    /* Allocate space for, and store the message in the pool.  */

    mp = pool_Alloc(&sts, gdbroot->pool, sizeof(*mp) - sizeof(mp->msg) + get->size);
    if (mp == NULL) break;
    mr = pool_ItemReference(NULL, gdbroot->pool, mp);

    memcpy(&mp->msg, get->data, get->size);
    pool_Qinit(NULL, gdbroot->pool, &mp->subm_ll);
    pool_QinsertPred(NULL, gdbroot->pool, &mp->subm_ll, &gdbroot->db->subm_lh);
    gdbroot->db->subm_lc++;
    np = hash_Search(&sts, gdbroot->nid_ht, &mp->msg.hdr.nid);

    /* Walk through every entry in the message buffer.  */

    for (
      i=0,
      dp = (net_sSubData *)&mp->msg.subdata,
      dr = mr + offsetof(sub_sMessage, msg.subdata);

      i < mp->msg.count;

      i++,
      dr += offsetof(net_sSubData, data) + dp->size,
      dp = (net_sSubData *)((pwr_tUInt32)&dp->data + dp->size)
    ) {

      cp = hash_Search(&sts, gdbroot->subc_ht, &dp->sid);
      if (cp == NULL) {
	if (rp == NULL) {
	  rp = pool_Alloc(&sts, gdbroot->pool, sizeof(*rp)
	    + ((mp->msg.count - 1) * sizeof(rp->sid[0])));
	  if (rp == NULL)
	    continue;
	  tgt.nid = mp->msg.hdr.nid;
	}
	rp->sid[rp->count++] = dp->sid;
	continue;
      }

      /* If the client has an earlier reference to a message, then
	 remove that reference.
	 This can cause the old message to get disposed of.  */

      subc_RemoveFromMessage(cp);

      /* Now start filling in the client according to the new
	 information we just received...  */

      memcpy(&cp->lastupdate, &curtim, sizeof(curtim));
      cp->sts = dp->sts;
      cp->count++;

      if (ODD(dp->sts)) {
	cp->old	    = FALSE;	/* There is new, fresh, real data!  */
	cp->submsg  = mr;
	cp->subdata = dr;
	refcount++;

	if (1 || mp->msg.hdr.xdr) {
          if (cp->cclass == pool_cNRef)
            ndc_ConvertData(&sts, np, &cp->aref, dp->data, dp->data, dp->size, ndc_eOp_decode);
          else {
            cp->old = TRUE;
            ccp = pool_Address(&cp->sts, gdbroot->pool, cp->cclass);
            if (ccp != NULL) {
              tbl = pool_Address(&cp->sts, gdbroot->pool, ccp->rnConv);
              if (tbl != NULL) {   
                ndc_ConvertRemoteData(&cp->sts, np, ccp, &cp->raref, dp->data, dp->data, dp->size, ndc_eOp_decode);
                if (ODD(cp->sts))
                  cp->old = FALSE;
              }
            }
          }
          
	  /* !!! Todo !!! Error handling. */
	}

	/* If the userdata field contains a valid pool_tRef, then
	   copy the data. This poolref is resolved in RTDB!  */

	if (cp->userdata != pool_cNRef) {
	  adrs = pool_Address(NULL, gdbroot->rtdb, cp->userdata);
	  if (adrs != NULL) {
            if (cp->cclass == pool_cNRef)
              memcpy(adrs, dp->data, MIN(dp->size, cp->usersize));
            else if (!cp->old) {
              ndc_ConvertRemoteToNativeTable(&cp->sts, ccp, tbl, &cp->raref, &cp->aref, adrs, dp->data, cp->usersize);
              if (EVEN(cp->sts))
                cp->old = TRUE;
            }
	  }
	}
      }
    }

    /* Fill in count as the # of references in the 'message' header. This
       is the # of references that have to be removed before the 'message'
       can be returned to pool. If there are no references to the 'message'
       just dispose it!  */

    if (refcount != 0) {
      mp->msg.count = refcount;
    } else {
      pool_Qremove(NULL, gdbroot->pool, &mp->subm_ll);
      gdbroot->db->subm_lc--;
      pool_Free(NULL, gdbroot->pool, mp);
    }
  } gdb_ScopeUnlock;

  if (rp != NULL) {
    net_Put(NULL, &tgt, rp, net_eMsg_subRemove, pwr_Offset(rp, sid[rp->count]));
#if 0
    errh_Info("Removed %d subscriptions", rp->count - 1);
#endif
  }
}

/* Move all clients from the indicated node to the unknown node.  */

void
subcm_FlushNode (
  pwr_tStatus		*sts,
  gdb_sNode		*np
)
{

  subc_ActivateList(&np->subc_lh, pwr_cNObjid);

}
