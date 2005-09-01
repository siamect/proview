/* 
 * Proview   $Id: rt_plc_utl.c,v 1.2 2005-09-01 14:57:56 claes Exp $
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

/* rt_plc_utl.c 
   Routines to synchronize with the PLC job.  */

#include "pwr.h"
#include "rt_ini_event.h"
#include "rt_qcom.h"
#include "rt_plc_utl.h"


/* Wait for the Plc to start.  */

pwr_tStatus
plc_UtlWaitForPlc ()
{
  pwr_tStatus sts = 1;
  qcom_sQid wait_q;

  if (!qcom_CreateQ(&sts, &wait_q, NULL, "plcUtlWaitForPlc"))
    return sts;

  qcom_WaitAnd(&sts, &wait_q, &qcom_cQini, ini_mEvent_newPlcStartDone, qcom_cTmoEternal);

  qcom_DeleteQ(NULL, &wait_q);

  return sts;
}

pwr_tBoolean
plc_UtlIsStartedPlc ()
{

  return (qcom_EventMask(NULL, &qcom_cQini) & ini_mEvent_newPlcStartDone) != 0;
}
