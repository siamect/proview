#ifndef rt_bck_load_h
#define rt_bck_load_h

/* rt_bck_load.h -- <short description>

   PROVIEW/R
   Copyright (C) 1996 by Comator Process AB.

   This module contains the code for reading and
   restoring the information in the backup data file.  */

/* This routine opens the backup datafile, reads the
   information and loads it into rtdb.
   This should be done before bck_Init is invoked.  */

pwr_tStatus bck_LoadBackup ();

#endif
