/* rt_io_utl.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   Routines to synchronize with the IO job.  */

pwr_tStatus io_StopIO(
  int Areaindex,
  PORT *IOjob_portp
);

pwr_tStatus io_TerminateIO(
  int Areaindex,
  PORT *IOjob_portp
);

pwr_tStatus
io_StartIoComm(
  PORT *IOjob_portp
);
