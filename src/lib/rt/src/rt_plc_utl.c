/* rt_plc_utl.c -- <short description>

   PROVIEW/R
   Copyright (C) 1996,99 by Mandator AB.

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
