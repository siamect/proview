/* rt_subs.c -- Object data subscription, server side.

   PROVIEW/R
   Copyright (C) 1997 by Comator Process AB.

   This module contains routines to handle subscription.
   Those routines can NOT be called in a job context other than
   that of the nethandler.  */

#if defined(OS_ELN)
# include $vaxelnc
#else
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
#endif

#include "pwr.h"
#include "co_cdh.h"
#include "rt_gdh_msg.h"
#include "rt_pool.h"
#include "rt_gdb.h"
#include "rt_sub.h"
#include "rt_net.h"
#include "rt_vol.h"
#include "rt_subs.h"


/* Delete a server entry. Database must be locked by caller.  */

void
subs_DeleteServer (
  sub_sServer		*sp
)
{
  pwr_tStatus		sts;
  sub_sBuffer		*bp;
  gdb_sObject		*op;

  gdb_AssumeLocked;

  bp = pool_Address(NULL, gdbroot->pool, sp->br);

  /* Disconnect from buffer. If the number of servers connected
     to the buffer reaches zero, the buffer gets disposed when
     it times out the next time.  */

  pool_Qremove(NULL, gdbroot->pool, &sp->bufsubs_ll);
  bp->bufsubs_lc -= 1;
  bp->totsize -= sp->aref.Size;

  /* Now give the subsrv entry back to the pool.  */

  pool_Qremove(NULL, gdbroot->pool, &sp->subs_ll);
  gdbroot->db->subs_lc--;
  pool_Qremove(NULL, gdbroot->pool, &sp->nodsubs_ll);

  /* Remove from server table.  */

  hash_Remove(&sts, gdbroot->subs_ht, sp);

  /* Decrement subcounter for referenced object */

  op = vol_OidToObject(&sts, sp->aref.Objid, gdb_mLo_owned, vol_mTrans_alias, cvol_eHint_none);
  if (op != NULL) {
    op->u.n.subcount--;
  }

  pool_Free(NULL, gdbroot->pool, sp);
}

/* Updates all server buffers that are connected to this object.  */

void
subs_UnlinkObject (
  gdb_sObject		*op
)
{
  sub_sServer		*sp;
  pool_sQlink		*sl;
  
  gdb_AssumeLocked;

  for (
    sl = pool_Qsucc(NULL, gdbroot->pool, &gdbroot->db->subs_lh);
    sl != &gdbroot->db->subs_lh;
  ) {
    sp = pool_Qitem(sl, sub_sServer, subs_ll);
    if (cdh_ObjidIsEqual(sp->aref.Objid, op->g.oid)) {
      sp->data	= pool_cNRef;
      sp->sts	= GDH__NOSUCHOBJ;

      op->u.n.subcount--;
    }
  }

  pwr_Assert(op->u.n.subcount == 0);

}
