/* rt_cvols.c -- Cached volumes, server calls.

   PROVIEW/R
   Copyright (C) 1997-98 by Mandator AB.

   .  */

#if defined(OS_ELN)
# include $vaxelnc
#else
# include <stdio.h>
# include <string.h>
#endif

#include "pwr.h"
#include "rt_errh.h"
#include "rt_gdh_msg.h"
#include "rt_gdb.h"
#include "rt_cvols.h"
#include "rt_net.h"


cvol_sNotify *
cvols_InitNotify (
  gdb_sObject		*op,
  cvol_sNotify		*nmp,
  net_eMsg		subtype
)
{
  unsigned int		size = 0;

  gdb_AssumeLocked;

  switch (subtype) {
  case net_eMsg_createObject:
    size = sizeof(net_sCreateObject);
    break;
  case net_eMsg_deleteObject:
    size = sizeof(net_sDeleteObject);
    break;
  case net_eMsg_moveObject:
    size = sizeof(net_sMoveObject);
    break;
  case net_eMsg_renameObject:
    size = sizeof(net_sRenameObject);
    break;
  default:
    errh_Bugcheck(2, "cvols_InitNotify");
  }

  memset(nmp, 0, size);

  nmp->subtype = subtype;
  nmp->size = size;
  nmp->msg.hdr.oid = op->g.oid;

  return nmp;
}

/* Notify nodes that have cache mounted this object.  */

void
cvols_Notify (
  cvol_sNotify		*nmp
)
{
  pwr_tStatus		lsts;
  pool_sQlink		*nl;
  gdb_sNode		*np;
  qcom_sQid		tgt;
  qcom_sPut		put;

  gdb_AssumeLocked;

  if (gdbroot->db->nethandler.qix == 0)
    return;

  tgt.qix = net_cProcHandler;

  gdb_Unlock;

    /* Send to all nodes that have mounted affected volume.

       NOTA BENE !!!
	  In this version we send to all known nodes. */

    for (
      nl = pool_Qsucc(NULL, gdbroot->pool, &gdbroot->db->nod_lh);
      nl != &gdbroot->db->nod_lh;
      nl = pool_Qsucc(NULL, gdbroot->pool, nl)
    ) {
      np = pool_Qitem(nl, gdb_sNode, nod_ll);
      if (np == gdbroot->my_node || np == gdbroot->no_node) continue;
      if (!np->flags.b.active) continue; /* Todo !!! What happens with consistency ? */

      tgt.nid = np->nid;

      if (!net_Put(&lsts, &tgt, &nmp->msg, nmp->subtype, nmp->size))
	errh_Warning("Notify: error %x sending subtype %s to node %s", lsts, "Notify", np->name); 
    }

    /* Submit to the NETH ACP to check if any subscriptions are affected.  */

    put.type.b	= net_cMsgClass;
    put.type.s	= nmp->subtype;
    put.reply	= qcom_cNQid;
    put.data  	= (char *)&nmp->msg;
    put.size	= nmp->size;
    nmp->msg.hdr.hdr.xdr = 0;
    nmp->msg.hdr.hdr.msn = 0;
    nmp->msg.hdr.hdr.nid = gdbroot->my_node->nid;

    qcom_Put(&lsts, &qcom_cQnacp, &put);

  gdb_Lock;

}
