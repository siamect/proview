/* 
 * Proview   $Id: rt_aproc.c,v 1.2 2005-09-01 14:57:55 claes Exp $
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

#include "pwr.h"              
#include "co_cdh.h"
#include "co_time.h"
#include "pwr_class.h"
#include "rt_gdh.h"
#include "rt_errh.h"
#include "rt_aproc.h"
#include "rt_proc_msg.h"

static pwr_sNode *proc_np = 0;


pwr_tStatus aproc_RegisterObject( 
  pwr_tOid oid
)
{
  pwr_tStatus sts;
  errh_eAnix anix = errh_Anix();
  
  if ( !anix)
    return PROC__ANIX;

  if ( !proc_np) {
    pwr_tOid noid;

    sts = gdh_GetNodeObject( 0, &noid);
    if ( EVEN(sts)) return sts;

    sts = gdh_ObjidToPointer( noid, (void **)&proc_np);
    if ( EVEN(sts)) return sts;
  }
  proc_np->ProcObject[anix-1] = oid;
  return PROC__SUCCESS;
}

pwr_tStatus aproc_TimeStamp()
{
  pwr_tStatus sts;
  pwr_tTime t;
  errh_eAnix anix = errh_Anix();
  
  if ( !anix)
    return PROC__ANIX;

  if ( !proc_np) {
    pwr_tOid noid;

    sts = gdh_GetNodeObject( 0, &noid);
    if ( EVEN(sts)) return sts;

    sts = gdh_ObjidToPointer( noid, (void **)&proc_np);
    if ( EVEN(sts)) return sts;
  }

  time_GetTime( &t);
  proc_np->ProcTimeStamp[anix-1] = t;
  return PROC__SUCCESS;
}






