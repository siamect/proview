#ifndef rt_plc_proc_h
#define rt_plc_proc_h
/* rt_plc_proc.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996,99 by Mandator AB.

   */

/* Initialize the PLC.
   At this state only direct linking can be done.
   The part of the initialization which effects parameters
   in the object body must wait until the old PLC program has
   stopped. 
   First argument = 1 -> Do the direct link part.
   First argument = 0 -> It is ok to change object parameters.

   The part of the initialization which effects parameters
   in the object body can now be done, since the old PLC program has
   stopped. 
   First argument (DirectLink) = 2 -> Do the I/O direct link part.
   First argument (DirectLink) = 0 -> It is ok to change object parameters.  */


typedef struct plc_proctbl plc_sProctbl;

struct plc_proctbl {
  pwr_tObjid	thread;
  void		(*init)(int, plc_sThread*);
  void		(*exec)(int, plc_sThread*);
};
#endif
