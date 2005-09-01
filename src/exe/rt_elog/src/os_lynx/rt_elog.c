/* 
 * Proview   $Id: rt_elog.c,v 1.2 2005-09-01 14:57:48 claes Exp $
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
#include "pwr_class.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_ds_msg.h"
#include "rt_gdh_msg.h"
#include "co_cdh.h"
#include "pwrb_c_asup.h"

#define PRINTint(a, b) printf(#b": %i\n", a->b);
#define PRINTflo(a, b) printf(#b": %f\n", a->b);
#define PRINTtim(a, b) printf(#b": %l.%i\n", a->b.tv_sec, a->b.tv_nsec);
#define PRINToid(a, b) printf(#b": %i:%i\n", a->b.vid, a->b.oix);
#define PRINTstr(a, b) printf(#b": %*s\n", sizeof(a->b)-1, a->b);
#define PRINTatr(a, b) printf(#b": %i.%i\n", a->b.Objid.vid, a->b.Objid.oix);

main ()
{
  pwr_tStatus		sts;
  pwr_sClass_ASup	*ap;
  char                  buff[256];
  char                  *s = buff;

  errh_Init("Test");

  sts = gdh_Init();
  if (!(sts & 1)) {
    errh_Info("error\n%m", sts);   
    exit(1);
  }

  while (printf("\nASup: "), s = gets(s)) {
    sts = gdh_NameToPointer(s, (void **)&ap);
    if (!(sts & 1)) {
      errh_Info("error\n%m", sts);   
      continue;
    }
    PRINTflo(ap, InP);
    PRINTint(ap, In);
    PRINTint(ap, ControlP);
    PRINTint(ap, Control);
    PRINTint(ap, Action);
    PRINTint(ap, Acked);
    PRINTint(ap, Blocked);
    PRINTstr(ap, Description);
    PRINTint(ap, DetectOn);
    PRINTstr(ap, DetectText);
    PRINTstr(ap, ReturnText);
    PRINTint(ap, EventType);
    PRINTint(ap, EventPriority);
    PRINTint(ap, EventFlags);
    PRINTatr(ap, Attribute);
    PRINTint(ap, AlarmStatus);
    PRINTint(ap, AlarmCheck);
    PRINTint(ap, DetectCheck);
    PRINTint(ap, DetectSend);
    PRINTtim(ap, DetectTime);
    PRINTint(ap, ReturnCheck);
    PRINTint(ap, ReturnSend);
    PRINTtim(ap, ReturnTime);
    PRINTtim(ap, AckTime);
    PRINToid(ap, AckOutunit);
    PRINTflo(ap, ActualValue);
    PRINTflo(ap, CtrlLimit);
    PRINTflo(ap, Hysteres);
    PRINTint(ap, High);
    PRINTstr(ap, Unit);
    PRINTint(ap, TimerFlag);
    PRINTint(ap, TimerNext);
    PRINTint(ap, TimerCount);
    PRINTint(ap, TimerDO);
    PRINTflo(ap, TimerTime);
    PRINTint(ap, TimerDODum);
    PRINToid(ap, TimerObjDId);
    PRINTint(ap, TimerAcc);
    PRINTflo(ap, TimerMin);
    PRINTflo(ap, TimerMax);
  }
}
