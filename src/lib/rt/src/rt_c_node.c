/* rt_c_node.c -- <short description>

   PROVIEW/R
   Copyright (C) 1997-98 by Mandator AB.

   Functions for the class $Node.

    */

#include <stdio.h>
#include <string.h>
#include "pwr.h"
#include "pwr_class.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_pwr_msg.h"
#include "pwrs_c_node.h"

static pwr_sNode *np = 0;


/*_*
  @aref node Node
*/

void
pwrs_Node_Exec (
)
{
  int i;
  pwr_tTime current_time;
  pwr_tDeltaTime diff;
  errh_eSeverity severity;
  errh_eSeverity system_severity;
  int new_idx = -1;
  static float timeout[40] = {
    0,0,0,0,3,5,5,0,60,5,
    5,5,5,0,0,0,5,5,5,5,
    5,5,5,5,5,5,5,5,5,5,
    5,5,5,5,5,5,5,5,5,5};

  if ( !np) {
    pwr_tOid oid;
    pwr_tStatus sts;

    sts = gdh_GetNodeObject( 0, &oid);
    if ( ODD(sts))
      gdh_ObjidToPointer( oid, (void **) &np);
    if ( EVEN(sts)) return;
  }

  if ( !np)
    return;

  system_severity = errh_Severity( np->SystemStatus);
  clock_gettime( CLOCK_REALTIME, &current_time);
  for ( i = 0; i < sizeof(np->ProcStatus)/sizeof(np->ProcStatus[0]); i++) {
    if ( np->ProcStatus[i] != 0 && timeout[i] != 0.0) {
      time_Adiff( &diff, &current_time, &np->ProcTimeStamp[i]);

      if ( time_DToFloat( 0, &diff) > timeout[i]) {
	np->ProcStatus[i] = PWR__PTIMEOUT;
      }
      else if ( np->ProcStatus[i] == PWR__PTIMEOUT) {
	np->ProcStatus[i] = (i < errh_cAnix_SrvSize) ? PWR__SRUN : PWR__ARUN;
      }
    }

    severity = errh_Severity( np->ProcStatus[i]);
    if ( np->ProcStatus[i] != 0 && EVEN(np->ProcStatus[i])) {
      if ( severity >= system_severity) {
	new_idx = i;
	system_severity = severity;
      }
    }

  }
  if ( new_idx != -1)
    np->SystemStatus = np->ProcStatus[new_idx];
  else if ( EVEN(np->SystemStatus))
    np->SystemStatus = PWR__RUNNING;
}

















