/* 
 * Proview   $Id: rt_csup.c,v 1.6 2005-09-01 14:57:55 claes Exp $
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

/* stdlib.h on Lynx defines NULL as 0, does not work! */
#define NULL (void*)0

#include <stdlib.h>
#include <float.h>
#include "pwr.h"
#include "pwr_class.h"
#include "pwr_baseclasses.h"
#include "co_time.h"
#include "co_cdh.h"
#include "rt_gdh.h"
#include "rt_gdh_msg.h"
#include "rt_csup_msg.h"
#include "rt_csup.h"
#include "rt_lst.h"
#include "rt_errh.h"


/*_*
  @aref cyclesup CycleSup
  */
lst_sEntry *
csup_Init (
  pwr_tStatus	*status,
  pwr_tObjid	tid,
  float		scanTime
)
{
  lst_sEntry	*lh;
  pwr_tObjid	cid;
  pwr_tObjid	pid;
  csup_sObject	*cp;
  int		tv_sec;
  pwr_tFloat32	max_delay;

  pwr_dStatus(sts, status, CSUP__SUCCESS);

  lh = calloc(1, sizeof(*lh));
  if (lh == NULL)
    pwr_Return(NULL, sts, CSUP__INSVIRMEM);

  lst_Init(NULL, lh, NULL);

  for (
    *sts = gdh_GetClassList(pwr_cClass_CycleSup, &cid);
    ODD(*sts);
    *sts = gdh_GetNextObject(cid, &cid)
  ) {
    cp = calloc(1, sizeof(*cp));
    cp->aref.Objid = cid;
    *sts = gdh_DLRefObjectInfoAttrref(&cp->aref, (void**)&cp->o, &cp->dlid);
    if (EVEN(*sts))
      goto error;

    if ( ODD(gdh_GetParent( cid, &pid)) && cdh_ObjidIsEqual( pid, tid))
      cp->is_owner = 1;

    lst_InsertSucc(NULL, lh, &cp->le, cp);
    max_delay = cp->o->MaxDelay;
    cp->o->DelayLimit.tv_sec = tv_sec = (int)max_delay;
    cp->o->DelayLimit.tv_nsec = (int)((max_delay - (float)tv_sec + FLT_EPSILON) * 1.0e9);   
    // printf("maxdelay: %f, tv_sec: %d, tv_nsec: %d\n", cp->o->MaxDelay, cp->o->DelayLimit.tv_sec, cp->o->DelayLimit.tv_nsec);
    errh_Info("maxdelay: %f, tv_sec: %d, tv_nsec: %d", cp->o->MaxDelay,
      cp->o->DelayLimit.tv_sec, cp->o->DelayLimit.tv_nsec);
  }

  if (lst_IsEmpty(NULL, lh))
    goto error;
    
  return lh;

error:
  csup_Exit(NULL, lh);
  return NULL;
}

int
csup_Exec (
  pwr_tStatus		*status,
  lst_sEntry		*lh,
  pwr_tDeltaTime	*next_start,
  pwr_tDeltaTime	*stop,
  pwr_tTime		*now
)
{
  int action = 0;
  csup_sObject *cp;
  pwr_tDeltaTime nextLimit;

  pwr_dStatus(sts, status, CSUP__SUCCESS);

  if (lh == NULL || next_start == NULL || now == NULL || stop == NULL)
    pwr_Return(NO, sts, CSUP__NULPOINT);

  while ((cp = lst_Succ(NULL, lh, &lh)) != NULL) {
    pwr_sClass_CycleSup *o = cp->o;

    if ( cp->is_owner) {
      if (time_Dcomp(&o->NextLimit, NULL) > 0) {
	if (time_Dcomp(stop, &o->NextLimit) > 0) {
	  if (!o->Delayed) {
	    o->DelayedTime = *now;
	    o->Delayed = TRUE;
	    o->Timely = FALSE;
	  }
	  o->DelayCount++;
	  o->LastDelay = *now;
	  action = MAX(action, o->DelayAction);
	} else if (!o->Timely) {
	  o->Timely = TRUE;
	  o->TimelyTime = *now;
	}
      }
      if ((o->DelayLimit.tv_sec & 1<<31) != (o->DelayLimit.tv_nsec & 1<<31)) {
	// printf("DelayLimit.tv_sec: %d, DelayLimit.tv_nsec: %d\n", o->DelayLimit.tv_sec, o->DelayLimit.tv_nsec);
	errh_Info("DelayLimit.tv_sec: %d, DelayLimit.tv_nsec: %d", o->DelayLimit.tv_sec, o->DelayLimit.tv_nsec);
      }
      if ((next_start->tv_sec & 1<<31) != (next_start->tv_nsec & 1<<31)) {
	// printf("next_start->tv_sec: %d, next_start->tv_nsec: %d\n", next_start->tv_sec, next_start->tv_nsec);
	errh_Info("next_start->tv_sec: %d, next_start->tv_nsec: %d", next_start->tv_sec, next_start->tv_nsec);
      }
      time_Dadd(&nextLimit, next_start, &o->DelayLimit);

      /* If we update the tv_nsec field first it is
	 possible that emon will detect a slip even if it is not. */

      o->NextLimit.tv_sec = nextLimit.tv_sec;
      o->NextLimit.tv_nsec = nextLimit.tv_nsec;

      o->CycleCount++;
    }
    else {
      /* Not owner, check stall delay */
      if ( o->DelayAction == 2) {
	if (time_Dcomp(&o->NextLimit, NULL) > 0 &&
	    time_Dcomp(stop, &o->NextLimit) > 0) {
	  o->DelayCount++;
	  o->LastDelay = *now;
	  action = MAX(action, o->DelayAction);
	}
      }
    }
  }
    
  return action;
}

void
csup_Exit (
  pwr_tStatus *status,
  lst_sEntry *lh
)
{
  csup_sObject	*cp;

  pwr_dStatus(sts, status, CSUP__SUCCESS);

  if (lh == NULL) return;

  while ((cp = lst_Succ(NULL, lh, NULL)) != NULL) {
    lst_Remove(NULL, &cp->le);
    free(cp);
  }
  free(lh);
}
