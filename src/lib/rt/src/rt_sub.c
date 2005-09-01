/* 
 * Proview   $Id: rt_sub.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* rt_sub.c 
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
