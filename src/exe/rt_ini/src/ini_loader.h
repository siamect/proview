#ifndef ini_loader_h
#define ini_loader_h

/* ini_loader.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   <Description>.  */

#ifdef	OS_ELN
pwr_tStatus
ini_PlcLoad (
  pwr_tUInt32		plcversion,
  char			*plcfile,
  int			debugflag
);

pwr_tStatus
ini_PlcStart (
  pwr_tUInt32		plcversion
);

#endif

#ifdef  OS_VMS
pwr_tStatus
ini_PlcStart (
  pwr_tUInt32		plcversion,
  char			*plcfile,
  pwr_tUInt32		busid
);
#endif

void ini_StartApplications ();

#endif
