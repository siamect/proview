/* ini_time.c -- Get time from host node.

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   .  */

#include $kerneldef
#include $kernelmsg
#include $vaxelnc
#include descrip
#include stdio
#include "pwr.h"


/* This section builds a file name from kernal DECnet
   information and executes a command file located on the VMS
   load host.  The command file "PAMSTIME" in the DECnet default
   directory which responds with the current time in ASCII.
   This time string is used to set this system's time. */

pwr_tBoolean
ini_Time (
  pwr_tStatus		*status,
  pwr_tUInt32		*busid
)
{
  long int		i;
  char			pwr_time[50];
  long int		size;
  char			*stime;
  LARGE_INTEGER		now;
  PORT			port;
  MESSAGE		msg;
  $DESCRIPTOR		(pwr_time_dsc, pwr_time);		
  $DESCRIPTOR		(stime_dsc, stime);
  globalref unsigned short ker$gq_host_address[3];
  pwr_dStatus		(sts, status, 1);

  sprintf(pwr_time, "%d.%d::PWR_TIME",
    ((ker$gq_host_address[2]>>10) & 63), (ker$gq_host_address[2] & 1023) );
  pwr_time_dsc.dsc$w_length = strlen(pwr_time);

  ker$create_port(sts, &port, 10);
  if (EVEN(*sts)) 
   return NO;

  for (i=0; i < 2; i++) {
    ker$connect_circuit(sts, &port, NULL, &pwr_time_dsc, TRUE, NULL, NULL);
    if (ODD(*sts)) break;
  }

  if (EVEN(*sts)) {
    errh_Fatal("Failed to open '%s' %m", pwr_time, *sts);
    return NO;
  }

  ker$wait_any(sts, NULL, NULL, &port);
  if (EVEN(*sts)) 
    return NO;

  ker$receive(sts, &msg, &stime, &size, &port, NULL, NULL);
  if (EVEN(*sts)) {
    errh_Fatal("Failed to receive host time message, %m", *sts);
    return NO;
  }

  stime_dsc.dsc$w_length  = 23;
  stime_dsc.dsc$a_pointer = stime;

  now = eln$time_value(&stime_dsc);

  ker$set_time(sts, &now);

  if (EVEN(*sts)) 
    return NO;

  errh_Info("Setting system time to %s", stime);

  ker$receive(sts, &msg, &stime, &size, &port, NULL, NULL);
  if (EVEN(*sts)) {
    errh_Fatal("Failed to receive busid message, %m", *sts);
    return NO;
  }

  stime[size] = '\0';
  *busid = atoi(stime);

  errh_Info("This node has Qcom bus id: %d", *busid);

  ker$delete(NULL, msg);

  return YES;
}
