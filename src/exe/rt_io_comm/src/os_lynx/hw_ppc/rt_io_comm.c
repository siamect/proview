/* 
 * Proview   $Id: rt_io_comm.c,v 1.2 2005-09-01 14:57:48 claes Exp $
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

/* rt_io_comm.c -- Runs the analog I/O

   Runs the analog I/O if there is a IOHandler object and it's
   attribute CycleTimeBus > 0.0

*/
#include <errno.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#include "pwr_baseclasses.h"
#include "co_time.h"
#include "rt_errh.h"
#include "rt_gdh.h"
#include "rt_io_base.h"
#include "rt_io_msg.h"


static pwr_sNode	*node = NULL;

static pwr_tBoolean 	aiOK = 0;
static pwr_tBoolean 	aoOK = 0;

#if DIGITAL_IO
static pwr_tBoolean 	coOK = 0;
static pwr_tBoolean 	diOK = 0;
static pwr_tBoolean 	doOK = 0;
#endif

static pwr_tBoolean RunAnalogIO(unsigned int *cycleTime);





int main(int argc, char **argv)
{
  pwr_tStatus 		sts;
  pwr_tObjid		od;
  unsigned int		cycleTime; /* ms */

  pwr_tTime		currentTime, lastScan, nextScan;
  pwr_tDeltaTime	scanTime, waitTime;
  
  errh_Init("rt_io_comm");

  sts = gdh_Init();
  if (EVEN(sts)) {
    errh_Fatal("Failed to init GDH, reason:\n%m", sts);
    exit(EXIT_FAILURE);
  }

  sts = gdh_GetNodeObject(0, &od);
  if (EVEN(sts)) {
    errh_Fatal("Failed to get Node object, reason:\n%m", sts);
    exit(EXIT_FAILURE);
  }

  sts = gdh_ObjidToPointer(od, (pwr_tAddress *)&node);
  if (EVEN(sts)) {
    errh_Fatal("Failed to get pointer to Node object, reason:\n%m", sts);
    exit(EXIT_FAILURE);
  }

  if (node->IOSimulFlag) {
    errh_Info("IOSimulFlag is set. I/O will not be initiated.");
    exit(EXIT_SUCCESS);
  }

  if (!RunAnalogIO(&cycleTime)) {
    errh_Info("No I/O will be handled by rt_io_comm. Exiting...");
    exit(EXIT_SUCCESS);
  }

  sts = io_Init(node);
  if (EVEN(sts)) {
    errh_Fatal("Failed to init I/O, reason:\n%m", sts);
    exit(EXIT_FAILURE);
  }

  if (node->NumberOfAI > 0) {
    sts = io_AiInit();
    if (ODD(sts))
      aiOK = 1;
    else if (sts != IO__NOCHANNELS) {
      errh_Fatal("Failed to init Ai, reason:\n%m", sts);
      exit(EXIT_FAILURE);
    }
  }


  if (node->NumberOfAO > 0) {
    sts = io_AoInit();
    if (ODD(sts))
      aoOK = 1;
    else if (sts != IO__NOCHANNELS) {
      errh_Fatal("Failed to init Ao, reason:\n%m", sts);
      exit(EXIT_FAILURE);
    }
  }


#if DIGITAL_IO /* Digital IO is handled by the PLC */

  if (node->NumberOfCO > 0) {
    sts = io_CoInit();
    if (ODD(sts))
      coOK = 1;
    else if (sts != IO__NOCHANNELS) {
      errh_Fatal("Failed to init Co, reason:\n%m", sts);
      exit(EXIT_FAILURE);
    }
  }


  if (node->NumberOfDI > 0) {
    sts = io_DiInit();
    if (ODD(sts))
      diOK = 1;
    else if (sts != IO__NOCHANNELS) {
      errh_Fatal("Failed to init Di, reason:\n%m", sts);
      exit(EXIT_FAILURE);
    }
  }

  if (node->NumberOfDO > 0) {
    sts = io_DoInit();
    if (ODD(sts))
      doOK = 1;
    else if (sts != IO__NOCHANNELS) {
      errh_Fatal("Failed to init Do, reason:\n%m", sts);
      exit(EXIT_FAILURE);
    }
  }

#endif

  clock_gettime(CLOCK_REALTIME, &lastScan);
  scanTime.tv_sec = cycleTime / 1000;
  scanTime.tv_nsec = (cycleTime % 1000) * 1000000;

  for (;;) {

    if (node->IOReadWriteFlag) {
      if (aiOK) {
        sts = io_AiRead();
      }

      if (aoOK) {
        sts = io_AoWrite();
      }

#if DIGITAL_IO
      if (coOK) {
        sts = io_CoRead();
      }

      if (diOK) {
        sts = io_DiRead();
      }

      if (doOK) {
        sts = io_DoWrite();
      }
#endif

    }

    clock_gettime(CLOCK_REALTIME, &currentTime);
    time_Aadd(&nextScan, &lastScan, &scanTime);
    if (time_Acomp(&currentTime, &nextScan) < 0) { 
      time_Adiff(&waitTime, &nextScan, &currentTime);
      nanosleep((struct timespec *)&waitTime, NULL);
    }

    lastScan = nextScan;

  }

}


/*
 * RunAnalogIO
 *
 * If there isn't any IOHandler object or if the attribute
 * CycleTimeBus <= 0, the PLC shall handle analog IO
 */
static pwr_tBoolean RunAnalogIO(unsigned int *cycleTime)
{
  pwr_tStatus		sts;
  pwr_sClass_IOHandler  *ioHandler;
  pwr_tObjid		ioObjid;

  sts = gdh_GetClassList(pwr_cClass_IOHandler, &ioObjid);
  if (EVEN(sts))
    return FALSE;

  sts = gdh_ObjidToPointer(ioObjid, (void **)&ioHandler);
  if (EVEN(sts))
    return FALSE;

  if (ioHandler->CycleTimeBus <= 0.0)
    return FALSE;

  /* Cycle time in ms */
  *cycleTime = 1000 * ioHandler->CycleTimeBus;

  return TRUE;
}

