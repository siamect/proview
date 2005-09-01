/* 
 * Proview   $Id: rt_c_node.c,v 1.6 2005-09-01 14:57:55 claes Exp $
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

/* rt_c_node.c 
   Functions for the class $Node. */

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
    5,5,5,0,0,20,0,5,5,5,
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
	if ( errh_Severity( np->ProcStatus[i]) < errh_Severity(PWR__PTIMEOUT))
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




/* Supervise emon server process */
void
pwrs_Node_SupEmon (
)
{
  int i = errh_eAnix_emon - 1;
  pwr_tTime current_time;
  pwr_tDeltaTime diff;
  static float timeout = 3;

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

  if ( np->ProcStatus[i] != 0 && np->ProcStatus[i] != PWR__PTIMEOUT) {
    clock_gettime( CLOCK_REALTIME, &current_time);
    time_Adiff( &diff, &current_time, &np->ProcTimeStamp[i]);

    if ( time_DToFloat( 0, &diff) > timeout) {
      if ( errh_Severity( np->ProcStatus[i]) < errh_Severity(PWR__PTIMEOUT)) {
	np->ProcStatus[i] = PWR__PTIMEOUT;
	np->SystemStatus = PWR__PTIMEOUT;
      }
    }
  }
}

















