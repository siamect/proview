#ifndef rt_plc_dirlink_h
#define rt_plc_dirlink_h

/* rt_plc_dirlink.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996,99 by Mandator AB.

   This module contains all routines to handle direct linking
   for the PLC-program.  */

pwr_tStatus plc_rtdbref (
  struct plc_rtdbref (*LinkArray)[], 
  plc_sThread *tp
);

void plc_GetPointerToObject (
  pwr_tAddress	*Pointer,
  char	*Name,
  pwr_tSubid	*Subid,
  pwr_tObjid	*Objid,
  pwr_tInt32	Type,
  pwr_tInt32	Size
);

#endif
