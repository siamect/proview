/* 
 * Proview   $Id: nmps_plc_runtime.c,v 1.2 2006-01-26 08:15:12 claes Exp $
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

#ifdef OS_ELN
#include stdio
#else
#include <stdio.h>
#endif

#include "pwr.h"
#include "pwr_nmpsclasses.h"
#include "rt_plc.h"
#include "co_time.h"

#define ODD(a)	(((int)(a) & 1) != 0)
#define EVEN(a)	(((int)(a) & 1) == 0)
#define max(Dragon,Eagle) ((Dragon) > (Eagle) ? (Dragon) : (Eagle))
#define min(Dragon,Eagle) ((Dragon) < (Eagle) ? (Dragon) : (Eagle))
#ifndef __ALPHA
#define abs(Dragon) ((Dragon) >= 0 ? (Dragon) : (-(Dragon)))
#endif


/*_*
  RunningTime

  @aref runningtime RunningTime
*/
void RunningTime_exec(
	plc_sThread		*tp,
	pwr_sClass_RunningTime  *object)
{
	pwr_tFloat32	TimeSince;

/* Safeguard reasonable time 10 ms - 30 min */
	TimeSince = *object->ScanTime;
	if (TimeSince <= 0.0) TimeSince = 0.01;
	if (TimeSince > 1800.0) TimeSince = 0.01;

/* Test if New Trip */
	if (object->TripReset)
	{
	  object->OldTripNOfStarts = object->TripNOfStarts;
	  object->OldTripRunHours = object->TripRunHours;
	  object->OldTripRunSeconds = object->TripRunSeconds;
	  object->OldTripHours = object->TripHours;
	  object->OldTripSeconds = object->TripSeconds;
	  object->OldTripUsage = object->TripUsage;

	  object->TripNOfStarts = 0;
	  object->TripRunHours = 0;
	  object->TripRunSeconds = 0.0;
	  object->TripHours = 0;
	  object->TripSeconds = 0.0;

	  clock_gettime(CLOCK_REALTIME, &object->ResetTime);
	  object->TripReset = 0;
	}
/* Update Calendar time */
	object->TotalSeconds += TimeSince;
	if (object->TotalSeconds >= 3600.0)
	{
	  object->TotalSeconds -= 3600.0;
	  object->TotalHours++;
	}
	object->TripSeconds += TimeSince;
	if (object->TripSeconds >= 3600.0)
	{
	  object->TripSeconds -= 3600.0;
	  object->TripHours++;
	}

/* Test if running */
	object->Start = 0;
	if (*object->RunningP)
	{
/* New start ? */
	  if (!object->Running)
	  {
	    object->Start = 1;
	    object->TotalNOfStarts++;
	    object->TripNOfStarts++;
	    clock_gettime(CLOCK_REALTIME, &object->StartTime);
	  } /* End if new start */
/* Update Running Time */
	  object->TripRunSeconds += TimeSince;
	  if (object->TripRunSeconds >= 3600.0)
	  {
	    object->TripRunSeconds -= 3600.0;
	    object->TripRunHours++;
	  }
	  object->TotalRunSeconds += TimeSince;
	  if (object->TotalRunSeconds >= 3600.0)
	  {
	    object->TotalRunSeconds -= 3600.0;
	    object->TotalRunHours++;
	  }
	} /* End if Running */
	object->Running = *object->RunningP;
/* Calculate usage % */
	object->TotalUsage = (object->TotalRunHours * 3600.0 +
		object->TotalRunSeconds) /
		(object->TotalHours * 3600.0 + object->TotalSeconds) * 100.0;
	object->TripUsage = (object->TripRunHours * 3600.0 +
		object->TripRunSeconds) /
		(object->TripHours * 3600.0 + object->TripSeconds) * 100.0;
}













