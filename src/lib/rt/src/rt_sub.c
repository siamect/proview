/* rt_sub.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module contains routines to handle subscription.
   Those routines can be called from the GDH API.  */

#if defined(OS_ELN)
# include $vaxelnc
#elif defined(OS_VMS)
# include <stddef.h>
# include <stdlib.h>
# include <descrip.h>
# include <string.h>
# include <stsdef.h>
# include <lib$routines.h>
#endif

#include "pwr.h"
#include "rt_gdh_msg.h"
#include "rt_hash_msg.h"
#include "co_tree.h"
#include "rt_gdb.h"
#include "rt_hash.h"
#include "rt_pool.h"
#include "rt_vol.h"
#include "rt_net.h"
#include "rt_sub.h"
#include "rt_dl.h"


/* Updates all server buffers that are connected to this object.  */

void
sub_UnlinkObject (
  gdb_sObject		*op
)
{
  sub_sServer		*sp;
  pool_sQlink		*sl;
  
  for (
    sl = pool_Qsucc(NULL, gdbroot->pool, &gdbroot->db->subs_lh);
    sl != &gdbroot->db->subs_lh;
    sl = pool_Qsucc(NULL, gdbroot->pool, sl)
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
