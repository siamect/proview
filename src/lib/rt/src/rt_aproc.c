/* rt_proc_cmn.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996-98 by Mandator AB.

   Process routines common for all platforms.   */

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

  clock_gettime( CLOCK_REALTIME, &t);
  proc_np->ProcTimeStamp[anix-1] = t;
  return PROC__SUCCESS;
}






